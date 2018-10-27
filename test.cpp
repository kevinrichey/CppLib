#include <cstdio>
#include <utility>
#include <exception>
#include <functional>
#include <cstring>
#include <tuple>

namespace kwr {

// Simple Primitives
// ==================

#define kwr_Str(x)       #x
#define kwr_NumStr(num)  kwr_Str(num)
#define kwr_Concat(a,b)  a##b

template<class T, size_t N>
constexpr size_t size(T (&)[N]) { return N; }

struct SourcePoint {
    const char* filename;
    int line;

    void print() const;
};

void SourcePoint::print() const
{
    printf("%s:%d: ", filename, line);
}

// Global stack of objects.
//
// Add a "next" member to your class:
//    Type* next = gstack<T>(this);
// Remove in destructor:
//    ~Type() { gstack(next); }
// Access first element:
//    auto top = gstack<T>();
template <typename Type>
Type* gstack(Type* me = nullptr)
{
    static Type* top = nullptr;
    Type* result = top;
    if (me) top = me;
    return result;
}

// Unit Testing
// ===============

class TestFailure {
  public:
    void print() const;
    const char* const message;
};

void TestFailure::print() const
{
    printf("failure: %s", message);
}

class TestCase {

  public:

    virtual void run() = 0;
    TestCase* next() { return nextTest; }
    void print() const;

    TestCase(TestCase&) = delete;
    TestCase& operator=(const TestCase&) = delete;

  protected:

    TestCase(const char* sourceFile, int sourceLine, const char* testName);
    void test(bool condition, const char* label);
    virtual ~TestCase() { gstack(nextTest); }

  private:

    TestCase* nextTest = gstack(this);

    SourcePoint source;
    const char* const name;
};

TestCase::TestCase(const char* sourceFile, int sourceLine, const char* testName) : 
  source({ sourceFile, sourceLine }),
  name(testName)
{}

void TestCase::test(bool condition, const char* label)
{
    if (!condition) throw TestFailure { label };
}

void TestCase::print() const
{
    source.print();
    printf("Test case %s ", name);
}

#define kwr_TestCase(name) \
class kwr_TestCase_##name : public kwr::TestCase { \
    public: kwr_TestCase_##name() : TestCase( __FILE__, __LINE__, #name ) {} \
    void run() override; \
} kwr_TestCase_##name##_object; void kwr_TestCase_##name::run()

// Simple Data Types
// ====================

class AbortNewHandler {
    static void handler();
    std::new_handler old_handler = std::set_new_handler(handler);

  public:
    ~AbortNewHandler() { std::set_new_handler(old_handler); }
};

void AbortNewHandler::handler()
{    
    printf("Out of memory!\n");
    abort();
}

template <typename T>
struct Memory {
    int size {};
    T* ptr {};

    Memory() = default;
    Memory(int initSize, T* initPtr) : size(initSize), ptr(initPtr) {}
    Memory(int initSize) : Memory(initSize, new T[initSize]) {}
    ~Memory() { delete[] ptr; }

    Memory(const Memory<T>&) = delete;
    Memory(const Memory<T>&&) = delete;
    Memory<T>& operator=(const Memory<T>&) = delete;
    Memory<T>& operator=(const Memory<T>&&) = delete;
};


class String {
   
  public:

    String() = default;
    explicit String(size_t strSize, char* initPtr);
    explicit String(size_t strSize);
    explicit String(const char* const initCstr);

    String(const String& that);
    String(String&& that);
    String& operator=(const String& that);
    String& operator=(String&& that);

    void swap(String& that);
    void ncopy(const char* fromCstr);

    const char* cstr() const { return str; }
    bool empty() const { return !str || !*str; } 
    size_t size() const;
    size_t length() const;

    ~String();

  private:

    size_t strsize = 0;
    char* str  = nullptr;
};

String::String(size_t initSize, char* initPtr) :
  strsize(initSize),
  str(initPtr)
{
}
  
String::String(size_t init_size) :
  String(init_size, new char[init_size])
{
    *str = '\0';
}

String::String(const char* const initCstr) :
  String(std::strlen(initCstr)+1)
{
    ncopy(initCstr);
}

String::String(const String& that) :
  String(that.str)
{
}

String::String(String&& that) :
  String(that.strsize, that.str)
{
    String().swap(that);
}

String& String::operator=(const String& that)
{
    // If big enough, copy chars into current array
    if (strsize >= that.strsize) {
        ncopy(that.str);
    }
    else {
        String(that).swap(*this);
    }

    return *this;
}

String& String::operator=(String&& that)
{
    String mover( std::move(that) );
    swap(mover);
    return *this;
}

void String::swap(String& that)
{
    std::swap(strsize, that.strsize);
    std::swap(str, that.str);
}

void String::ncopy(const char* fromCstr)
{
    std::strncpy(str, fromCstr, strsize-1);
    str[strsize] = '\0';
}

size_t String::size() const
{
    return strsize;
}

size_t String::length() const
{
    return str? std::strlen(str) : 0;
}

bool operator==(const String& left, const String& right)
{
    return strcmp(left.cstr(), right.cstr()) == 0;
}

bool operator==(const String& left, const char* right)
{
    return !std::strcmp(left.cstr(), right);
}

bool operator==(const char* left, const String& right)
{
    return !std::strcmp(left, right.cstr());
}

String::~String()
{
    delete[] str;
}

// Tracing and Assetions
// =======================

struct TracePoint {
    SourcePoint source;
    const char* category;
    const char* message;

    void print() const;
};

void TracePoint::print() const
{
    source.print();
    printf("%-5s %s\n", category, message);
}

class Trace {

  protected:

    Trace(const char* filename, int line, const char* category, const char* message) :
      point( { filename, line, category, message} )
    {}

  public:

    Trace(const char* filename, int line, const char* message); 
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

Trace::Trace(const char* filename, int line, const char* message) :
  Trace(filename, line, "trace", message)
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

void println(const char* str = "")
{
    printf("%s\n", str);
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
    ScopeTrace(const char* filename, int line, const char* scopename);
    ~ScopeTrace();
};

ScopeTrace::ScopeTrace(const char* filename, int line, const char* scopename) :
  Trace(filename, line, "begin", scopename)
{
    print();
}

ScopeTrace::~ScopeTrace()
{
    point.category = "end";
    print();
}

#define kwr_Scope(name)  kwr::ScopeTrace  KWR_TRACE_NAME(scope,__LINE__) ( __FILE__, __LINE__, (name) )

template <typename T, size_t N>
watch(const char* filename, int line, const T& value, const char (&name)[N])
{
    char text[20+N];
    size_t textlen = 20+N-1;
    snprintf(text, textlen, "%s = %d", name, value);
    TracePoint { filename, line, "watch", text }.print();
}

#define kwr_Watch(var)  kwr::watch( __FILE__, __LINE__, (var), (#var) )


class Failure 
{
  public:
    void print() const;

  private:
    TracePoint backtrace[100];
    size_t count { Trace::backtrace(backtrace, size(backtrace)) };
};

void Failure::print() const
{
    const TracePoint* end = backtrace + count;
    for (const TracePoint* point = backtrace; point != end; ++point) {
        point->print();
    }
}

class Assert : public Trace {

  public:

    Assert(const char* filename, int line, bool condition, const char* expression) :
      Trace (filename, line, "assert!", expression)
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

// Containers

template <typename Type>
class Array {

  public:

    Array() = default;
    Array(size_t initSize, Type* initData) : datasize(initSize), data(initData) {}
    explicit Array(size_t initSize) : Array(initSize, new Type[initSize]) {}

    Array(const Array<Type>&) = delete;
    Array<Type>& operator=(const Array<Type>&) = delete;

    Array(Array<Type>&& that) : Array(that.size(), that.data) {
        that.nullify();
    }

    Array<Type>& operator=(Array<Type>&& that) {
        Array<Type> mover( std::move(that) );
        swap(mover);
        return *this;
    }

    void swap(Array<Type>& that) {
        std::swap(datasize, that.datasize);
        std::swap(data, that.data);
    }

    void nullify() {
        datasize = 0;
        data = nullptr;
    }

    const Type* ptr() const { return data; }
    size_t size() const { return datasize; }
    Type* begin() { return data; }
    Type* end() { return data+size(); }

    Type& operator[](size_t index) { 
        kwr_Assert(index < size());
        return data[index]; 
    }

    ~Array() { delete[] data; }

  private:

    size_t datasize = 0;
    Type* data  = nullptr;

};

template <typename Type>
class Count {

  public:

    typedef Type ItemType;

    Count(Type first, Type last) :
      head(first), tail(last)
    {}

    int size() const { return tail - head + 1; }
    Type get() const { return head; }
    void next() { ++head; }

  private:

    Type head {};
    Type tail {};
};

class Fibonacci {

  public:

    typedef int ItemType;

    Fibonacci(int count) : end(count) {}

    int size() const { return end; }
    int get() const { return a; }
    void next() { 
        std::tie(a,b) = std::make_tuple(b, a+b);
        --end;
    }

  private:
    
    int a = 0;
    int b = 1;
    int end;

};


} // kwr


int main(int argc, char* argv[])
{ 
    printf("Run Tests\n");

    // Turn off trace printing for now
    kwr::Trace::set(kwr::Trace::nullPrinter);
    kwr_Scope("main()");


    kwr::TestCase* test = kwr::gstack<kwr::TestCase>();
    int count = 0;
    int failures = 0;

    do try {
        ++count;
        test->run();
    }
    catch (kwr::TestFailure& failure) {
        ++failures;
        test->print();
        failure.print();
        kwr::println();
    }
    catch (kwr::Failure& failure) {
        ++failures;
        test->print();
        printf("assertion failure, backtrace:\n");
        failure.print();
    }
    catch (std::exception& except) {
        ++failures;
        test->print();
        printf("exception: %s\n", except.what());
    }
    while (test = test->next());

    printf("%s!  Tests: %d, Failures: %d\n", failures? "Failed": "Success", count, failures);

	  return failures? 1 : 0;
}


kwr_TestCase(StringTest)
{
    kwr::String copy("hello");

    kwr::String nullstr;
    test(nullstr.empty(), "Null string is false");
    test(nullstr.length() == 0, "Null string length 0");
    test(nullstr.size() == 0, "Null string size 0");
    nullstr = copy;
    test(nullstr == "hello", "null string assignment");

    kwr::String empty(8);
    test(empty.empty(), "Empty string is false (no value)");
    test(empty.length() == 0, "Empty string length 0");
    test(empty.size() == 8, "Empty string has capacity 8");

    kwr::String literal("xyzzy");
    test(!literal.empty(), "Litearl string not empty");
    test(literal.length() == 5, "Literal string size");
    test(literal.size() == 6, "Literal string capacity");
    test(literal == "xyzzy", "Literal string value");

    kwr::String s2("zork");
    s2 = copy;
    test(s2 == "hello", "String copy & compare");

    kwr::String bigstr(100);
    test(bigstr.size() == 100, "Capacity 100");
    test(bigstr.length() == 0, "Size 0");

    bigstr = copy;
    test(bigstr.size() == 100, "Capacity still 100");
    test(bigstr.length() == 5, "Size now 5");
    test(bigstr == "hello", "Big string copy");
}

kwr::Array<double> makeArray()
{
    kwr::Array<double> doubles(10);
    double x = 0.0;
    for (auto& d : doubles) {
        d = (x += 0.1);
    }
    return doubles;
}

kwr_TestCase(ArrayTest)
{
    kwr::Array<int> zero;
    test(zero.size() == 0, "Empty array has zero size");

    kwr::Array<int> hundred(100);
    test(hundred.size() == 100, "Array size");

    int i=0;

    for (auto& num : hundred) {
        num = i++;
    }

    for (int i = 0; i < 100; ++i) {
        test(hundred[i] == i, "Array index");
    }

    kwr::Array<double> doubles(5);

    doubles = makeArray();
    test(doubles.size() == 10, "Move ctr, size 10");

    double x = 0.0;
    for (auto d : doubles) {
        test(d == (x += 0.1), "Move ctr, members");
    }
}

template <typename T>
kwr::Array<typename T::ItemType> array(T sequence)
{
    kwr::Array<int> numbers(sequence.size());

    int i = 0;
    while (sequence.size()) {
        numbers[i++] = sequence.get();
        sequence.next();
    }

    return numbers;
}

kwr_TestCase(CountTest)
{
    kwr::Count<int> ten(1, 10);
    test(ten.size() == 10, "Counter size 10");

    auto numbers = array( kwr::Count<int>(1,11) );
    test(numbers.size() == 11, "10 numbers array");
    test(numbers[0] == 1, "First is 1");
    test(numbers[10] == 11, "Last is 11");

    auto fib = array( kwr::Fibonacci(20) );
    test(fib.size() == 20, "20 fibonacci numbers");
    test(fib[0] == 0, "fib[0] == 0");
    test(fib[1] == 1, "fib[1] == 1");
    test(fib[2] == 1, "fib[2] == 1");
    test(fib[3] == 2, "fib[3] == 2");
    test(fib[4] == 3, "fib[4] == 3");
    test(fib[5] == 5, "fib[5] == 5");
    test(fib[6] == 8, "fib[6] == 8");
    test(fib[7] == 13, "fib[7] == 13");

}

template <typename Type>
struct SizedArray {
    size_t size;
    Type   data[1];
};

kwr_TestCase(Memory)
{
    kwr::Memory<float> mem(10);

}

kwr_TestCase(NewHandler)
{
    //kwr::AbortNewHandler handler;
    //int* p = new int[1000000000000];
}

