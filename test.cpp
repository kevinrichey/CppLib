#include "test.h"

#include <utility>
#include <exception>
#include <cstring>
#include <tuple>
#include <limits>

namespace kwr {

Assert::Assert(bool check, const char* message)
{
    if (on && !check) fail(message);
}

bool Assert::on = true;
std::function<void(const char*)> Assert::fail = FilePrintFormat("Assertion failed!  %s\n", stderr);

void println(const char* str)
{
    printf("%s\n", str);
}

void SourcePoint::print() const
{
    printf("%s:%d: ", filename, line);
}

// Unit Testing
// ===============

void TestFailure::print() const
{
    printf("failure: %s", message);
}

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

// Memory
// ====================

AbortNewHandler::~AbortNewHandler() 
{ 
    std::set_new_handler(old_handler); 
}

void AbortNewHandler::handler()
{    
    printf("Out of memory!\n");
    std::terminate();
}

TerminateHandler::~TerminateHandler()
{
    std::set_terminate(old_handler);
}

void TerminateHandler::handler()
{    
    printf("Program Terminate. Backtrace:\n");
    Trace::printBacktrace();
    abort();
}

// String
// ======================================================================

int String::compare(String that) const 
{
    return std::strcmp(str.data, that.str.data); 
}

bool operator==(String left, String right)
{
    return !left.compare(right);
}


// Tracing and Assertions
// =======================

Trace::Trace(SourcePoint point, String messagep) :
  source(point),
  message(messagep)
{
    print();
}

void Trace::print()
{
    source.print();
    printf("%s\n", message.cstr());
}

int Trace::printBacktrace()
{
    Trace* trace = gstack<Trace>(); 
    while (trace) {
        trace->print();
        trace = trace->next;
    }
}


} // kwr


int main(int argc, char* argv[])
{ 
    kwr::AbortNewHandler  newAborter;
    kwr::TerminateHandler terminator;

    kwr::Assert::fail = kwr::ThrowException<kwr::Failure>();

    printf("Run Tests\n");

    // Turn of trace printing
    //kwr_Trace("main()");

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
        printf("Assertion failure!  %s\n", failure.what());
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

kwr_TestCase(AssertTest)
{
    //kwr::Assert(false, "Testing assertions");
}

kwr::String getString() { return kwr::String("hello"); }

void passString(kwr::String str) { kwr::Assert(str == "world", "passed string 'world'"); }

kwr_TestCase(StringTest)
{
    // Default constructor
    kwr::String nullstr;
    test(nullstr.empty(), "Null string is false");
    test(nullstr.length() == 0, "Null string length 0");

    // Literal string constructor
    kwr::String literal("xyzzy");
    test(!literal.empty(), "Literal string not empty");
    test(literal.length() == 5, "Literal string size");
    test(literal == "xyzzy", "Literal string value");

    // Copy constructor
    kwr::String copy(literal);
    test(copy == "xyzzy", "Copy ctor value = xyzzy");
    test(copy.cstr() == literal.cstr(), "Copy ctr points to same memory");

    // Copy assignment operator
    nullstr = literal;
    test(nullstr == "xyzzy", "Copy assignment");
    test(nullstr.cstr() == literal.cstr(), "Copy assignment points to same memory");

    // Return value
    kwr::String hello( getString() );
    test(hello == "hello", "String return value");

    // Implicit constructor
    passString("world");

    // Implicit assignment
    kwr::String implicit;
    implicit = "implied";
    test(implicit == "implied", "Implicit literal str assingment");
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

kwr_TestCase(NewHandler)
{
    //kwr::AbortNewHandler handler;
    //int* p = new int[1000000000000];
}

class Thing {
  public:
    Thing() { ++count; }
    ~Thing() { --count; }
    static int count;
};

int Thing::count = 0;

template <class Type>
class Object {

  public:

    Object() = default;
    explicit Object(Type* p) : pointer(p) {}

    bool  operator!() const { return !pointer; }
    Type* operator->() { return pointer; }
    Type& operator*(){ return *pointer; }
    Type* get() { return pointer; }
    Type* release() { return std::exchange(pointer, nullptr); }

    Object<Type>& operator=(Type* p)
    {
        return Object<Type>(p).swap(*this);
    }

    Object<Type>& swap(Object<Type>& that)
    {
        std::swap(pointer, that.pointer);
        return *this;
    }

    ~Object() { delete pointer; }
    
    // Turn off copying
    Object(const Object<Type>&) = delete;
    Object<Type>& operator=(const Object<Type>&) = delete;

  private:

    Type* pointer {};
};

kwr_TestCase(Object)
{
    // Default constructot
    Object<Thing> nullobject;
    test(!nullobject, "Default object is empty");

    // Assignment operator

    // Init constructor & destructor
    test(!Thing::count, "count starts at zero");
    {
        Object<Thing> thing( new Thing );
        test(Thing::count == 1, "one count");
    }
    test(!Thing::count, "count zero again");

}

