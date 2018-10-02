#include <cstdio>

namespace kwr {

struct SourceLine 
{
    const char* file;
    int line;
};

#define kwr_Source  {__FILE__,__LINE__}

class Trace 
{
  public:
    explicit Trace(SourceLine sourceline) : source(sourceline) {}
    virtual void print(const char* category, const char* message);

  protected:
    SourceLine source;
    static int level;
};

void Trace::print(const char* category, const char* message)
{
    printf("%s:%d:\tL%d\t%s %s\n", 
           source.file, source.line, level,
           category, message);
}

int Trace::level = 0;

class ScopeTrace : public Trace 
{
  public:
    ScopeTrace(SourceLine sourceline, const char* scopename);
    ~ScopeTrace();

  private:
    const char* scope;
};

ScopeTrace::ScopeTrace(SourceLine sourceline, const char* scopename) :
  Trace  (sourceline),
  scope  (scopename)
{
    ++level;
    print("begin", scope);
}

ScopeTrace::~ScopeTrace()
{
    print("end", scope);
    --level;
}

class Checkpoint : public Trace
{
  public:
    Checkpoint(SourceLine sourceline, const char* message);
};

Checkpoint::Checkpoint(SourceLine sourceline, const char* message) :
  Trace (sourceline)
{
    print("checkpoint", message);
}

class Watch : public Trace
{
  public:
    template <typename T>
    Watch(SourceLine sourceline, const char* name, const T& value) :
      Trace(sourceline)
    {
        char text[256];
        size_t length = sizeof(text)/sizeof(char);
        snprintf(text, length, "%s = %d", name, value);
        print("watch", text);
    }
};

class Failure
{
  public:
    SourceLine source;
    int level;
    const char* category;
    const char* message;
};

class Assert : public Trace {
  public:
    Assert(SourceLine sourceline, bool condition, const char* expression) :
      Trace (sourceline)
    {
        if (!condition) {
            print("assert-fail", expression);
            fail("assert-fail", expression);
        }
    }

    virtual void fail(const char* category, const char* message) const;
};

void Assert::fail(const char* category, const char* message) const
{
    throw Failure { source, level, category, message };
};

#define kwr_Assert(condition)  kwr::Assert  kwr_Assert_Trace##__LINE__ ( kwr_Source, (condition), (#condition) )

} // kwr

void puke()
{
    kwr::ScopeTrace pukescope( kwr_Source, "puke");

    int x = 100;
    kwr::Watch watchx( kwr_Source, "x", x);
    kwr_Assert(x == 1);
}

int main(int argc, char* argv[])
{
    kwr::ScopeTrace appstart( kwr_Source, "test.exe" );

    kwr::Checkpoint checkpoint1( kwr_Source, "I'm here");

    try {
        kwr::ScopeTrace tryscope( kwr_Source, "try block");
        puke();
        kwr::Checkpoint checkpoint2( kwr_Source, "Exception not thrown");
    }
    catch (kwr::Failure& failure) {
        kwr::Checkpoint checkcatch( kwr_Source, "failure catch block");
    }

	  return 0;
}

