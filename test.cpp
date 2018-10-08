#include <cstdio>
#include <utility>
#include <exception>
#include <functional>

namespace kwr {

#define kwr_Concat(a,b)  a##b

// Global stack of objects.
//
// Add a "next" member to your class:
//    Type* next = gstack<Type>(this);
// Then reset the stack in destructor:
//    ~Type() { gstack(next); }
template <typename Type>
Type* gstack(Type* me = nullptr)
{
    static Type* top = nullptr;
    Type* result = top;
    if (me) top = me;
    return result;
}

struct TracePoint {
    const char* file;
    int line;
    const char* category;
    const char* message;

    void print();
};

void TracePoint::print()
{
    printf("%s:%d:\t%-5s\t%s\n", file, line, category, message);
}

class Trace {

  protected:

    Trace(const char* file, int line, const char* category, const char* message) :
      point( { file, line, category, message} )
    {}

  public:


    Trace(const char* file, int line, const char* message); 
    virtual void print();
    ~Trace() { gstack<Trace>(next); }

    static size_t backtrace(TracePoint* points, size_t length);

    typedef std::function<void(TracePoint&)> PrinterType;
    static void set(PrinterType newPrinter) { printer = newPrinter; }
    static void defaultPrinter(TracePoint& trace);
    static void nullPrinter(TracePoint& trace);

  protected:

    TracePoint point {};

  private:

    Trace* next = gstack<Trace>(this);
    static PrinterType printer;

};

Trace::PrinterType Trace::printer { Trace::defaultPrinter };

Trace::Trace(const char* file, int line, const char* message) :
  Trace(file, line, "trace", message)
{
    print();
}

void Trace::print()
{
    printer(point);
}

size_t Trace::backtrace(TracePoint* points, size_t length)
{
    Trace* trace = gstack<Trace>(); 
    size_t count = 0;
    while (trace && length--) {
        *points++ = trace->point;
        trace = trace->next;
        ++count;
    }
    return count;
}

void Trace::defaultPrinter(TracePoint& trace)
{
    trace.print();
}

void Trace::nullPrinter(TracePoint& trace) {}

#define KWR_TRACE_NAME(trace,line)  kwr_Concat(trace, line)
#define kwr_Trace(message)   kwr::Trace  KWR_TRACE_NAME(trace,__LINE__) ( __FILE__, __LINE__, (message) )

class ScopeTrace : public Trace 
{
  public:
    ScopeTrace(const char* file, int line, const char* scopename);
    ~ScopeTrace();
};

ScopeTrace::ScopeTrace(const char* file, int line, const char* scopename) :
  Trace(file, line, "begin", scopename)
{
    print();
}

ScopeTrace::~ScopeTrace()
{
    point.category = "end";
    print();
}

#define kwr_Scope(name)  kwr::ScopeTrace  KWR_TRACE_NAME(scope,__LINE__) ( __FILE__, __LINE__, (name) )

template<class T, size_t N>
constexpr size_t size(T (&)[N]) { return N; }

template <typename T, size_t N>
watch(const char* file, int line, const T& value, const char (&name)[N])
{
    char text[20+N];
    size_t textlen = 20+N-1;
    snprintf(text, textlen, "%s = %d", name, value);
    TracePoint { file, line, "watch", text }.print();
}

#define kwr_Watch(var)  kwr::watch( __FILE__, __LINE__, (var), (#var) )


class Failure 
{
  public:
    Failure() : count( kwr::Trace::backtrace(backtrace, 100) ) {}

    TracePoint backtrace[100];
    size_t count;
};

class Assert : public Trace {

  public:

    Assert(const char* file, int line, bool condition, const char* expression) :
      Trace (file, line, "assert!", expression)
    {
        if (!condition) {
            print();
            fail();
        }
    }

    virtual void fail() const;
};

void Assert::fail() const
{
    throw Failure();
};

#define kwr_Assert(condition)  kwr::Assert  KWR_TRACE_NAME(assert,__LINE__) ( __FILE__, __LINE__, (condition), (#condition) )

class TestFailure {
  public:
    TracePoint trace;
};

class TestCase {

  public:

    virtual void run() = 0;
    TestCase* next() { return nextTestCase; }
    void print(const char* message);

  protected:

    TestCase(const char* file, int line, const char* testname);
    void test(bool condition, const char* label);
    virtual ~TestCase() { gstack(nextTestCase); }

  private:

    TestCase* nextTestCase = gstack(this);
    const char* file;
    int line;
    const char* name;
};

TestCase::TestCase(const char* testfile, int testline, const char* testname) : 
  file(testfile),
  line(testline),
  name(testname) 
{}

void TestCase::test(bool condition, const char* label)
{
    if (!condition) throw TestFailure { file, line, name, label };
}

void TestCase::print(const char* message)
{
    TracePoint { file, line, name, message }.print();
}

#define kwr_TestCase(name) \
class kwr_TestCase_##name : public kwr::TestCase { \
  public: kwr_TestCase_##name() : TestCase( __FILE__, __LINE__, #name ) {} \
          void run() override; } kwr_TestCase_##name##_object; void kwr_TestCase_##name::run()

} // kwr


int main(int argc, char* argv[])
{ 
    printf("Run Tests\n");

    // Turn off trace printing for now
    kwr::Trace::set(kwr::Trace::nullPrinter);

    kwr::TestCase* test = kwr::gstack<kwr::TestCase>();

    do try {
        test->run();
    }
    catch (kwr::TestFailure& failure) {
        printf("Test Failure:\n");
        failure.trace.print();
    }
    catch (kwr::Failure& failure) {
        test->print("Assert Failure. Backtrace:");
        for (int i = 0; i < failure.count; ++i) {
            printf("\t");
            failure.backtrace[i].print();
        }
    }
    catch (std::exception& except) {
        printf("Unhandled exception:\n");
        test->print(except.what());
    }
    while (test = test->next());

	  return 0;
}


kwr_TestCase(PassingTest)
{
    test(true, "nothinng");
}

kwr_TestCase(FailingTest)
{
    test(false, "just false");
}

kwr_TestCase(AssertingTest)
{
    kwr_Assert(1==2);
}

