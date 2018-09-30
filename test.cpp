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

  protected:
    SourceLine source;
    static int level;
};

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

    printf("%s:%d:\tL%d\t%s %s\n", 
           source.file, source.line, level,
           "begin", scope);
}

ScopeTrace::~ScopeTrace()
{
    printf("%s::\tL%d\t%s %s\n", 
           source.file, level,
           "end", scope);

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
    printf("%s:%d:\tL%d\t%s %s\n", 
           source.file, source.line, level,
           "checkpoint", message);
}

class Watch : public Trace
{
  public:
    template <typename T>
    Watch(SourceLine sourceline, const char* name, const T& value) :
      Trace(sourceline)
    {
        printf("%s:%d:\tL%d\t%s %s = %d\n", 
               source.file, source.line, level,
               "trace", name, value);
    }
};

class Failure
{
};

class TracePrinter
{
  public:
    TracePrinter() =default;
    virtual ~TracePrinter() =0;
    virtual void print(SourceLine source, int level, const char* expression) =0;
    const char* category {};
};

TracePrinter::~TracePrinter() {}
void TracePrinter::print(SourceLine source, int level, const char* expression) {}

class TraceStreamPrinter : public TracePrinter {
  public:
    void print(SourceLine source, int level, const char* expression) override
    {
        printf("%s:%d:\tL%d\t%s %s\n", 
               source.file, source.line, level,
               category, expression);
    }
};

class IFunctoid {
  public:
    virtual void func() =0;
    virtual ~IFunctoid() =0;
};

IFunctoid::~IFunctoid() {}

class NullFunctoid : public IFunctoid {
  public:
    void func() override;
};

void NullFunctoid::func() {}

template <typename FunctImp> 
class WrapFunctoid : public IFunctoid {
  public:
    explicit WrapFunctoid(FunctImp* imp) : functoid(imp) {}
    void func() override { functoid->func(); }

  private:
    FunctImp* functoid;
};

class Functoid : public IFunctoid {

    IFunctoid* functoid {};

  protected:

    explicit Functoid(IFunctoid* fn) : functoid(fn) {}

  public:

    Functoid() : functoid(new NullFunctoid()) {}

    template <typename Type>
    explicit Functoid(Type* obj) : functoid(new WrapFunctoid<Type>(obj)) {}

    explicit Functoid(const Functoid&) =delete;
    Functoid& operator=(const Functoid&) =delete;

    template <typename Type>
    Functoid& operator=(Type* obj) 
    {
        Functoid(obj).swap(*this);
        return *this;
    }

    void swap(Functoid& that)
    {
        IFunctoid* oid = that.functoid;
        that.functoid = functoid;
        functoid = oid;
    }

    void func() override { functoid->func(); }
    void operator()() { func(); }

    ~Functoid() override { delete functoid; }
};




class ThrowingFunctoid {
  public:
    void func() 
    {
        throw Failure();
    }
};

class Assert : public Trace {
  public:
    Assert(SourceLine sourceline, bool condition, const char* expression) :
      Trace (sourceline)
    {
        if (!condition) {
            printer->print(source, level, expression);
            fail();
        }
    }

    static void set(TracePrinter* newPrinter);
    static TracePrinter* printer;

    static Functoid fail;
};

Functoid Assert::fail;

void Assert::set(TracePrinter* newPrinter)
{
    newPrinter->category = "assert failure";
    printer = newPrinter;
}

TracePrinter* Assert::printer {};

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
    kwr::TraceStreamPrinter assertPrinter;
    kwr::Assert::set(&assertPrinter);
    
    kwr::ThrowingFunctoid thrower;
    kwr::Assert::fail = &thrower;

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

