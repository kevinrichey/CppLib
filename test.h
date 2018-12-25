#ifndef INCLUDE_KWR_TEST_H
#define INCLUDE_KWR_TEST_H

#include <cstdio>
#include <new>
#include <functional>
#include <exception>

namespace kwr {

// ====================================
// Primitives

#define kwr_Str(x)       #x
#define kwr_NumStr(num)  kwr_Str(num)
#define kwr_Concat(a,b)  a##b

template <typename T>
struct Memory {
    int size {};
    T*  data {};
};

template <typename Type>
class Constant {
  public:
    explicit Constant(const Type& v) : value(v) {}
    Type operator()(const Type&) { return value; }

  private:
    Type value;
};

template <typename Type>
class Identity {
  public:
    Type& operator()(const Type& value) { return std::forward(value); }
};

template <class ObjectT, typename ParamT>
class NewObject {
  public:
    NewObject(const ParamT& p) : param(p) {}
    ObjectT* operator()() { return new ObjectT(param); }

  private:
    ParamT param;
};

template <class ObjectT>
class NewObject<ObjectT, void> {
  public:
    ObjectT* operator()() { return new ObjectT(); }

    template <typename ParamT>
    ObjectT* operator()(const ParamT& param) { return new ObjectT(std::forward(param)); }
};

template <typename Type>
class DeleteObject {
  public:
    Type* operator()(Type *object)
    {
        delete object;
        return nullptr;
    }
};

template <typename Type>
class NewArray {
  public:
    Memory<Type> operator()(int size) const 
    {
        return Memory<Type> { size, new Type[size] };
    }
};

template <typename Type>
class DeleteArray {
  public:
    Memory<Type> operator()(Memory<Type> array)
    {
        delete[] array.data;
        return Memory<Type> {};
    }
};

template <typename Except>
class ThrowException {
  public:
    void operator()(const char* message)
    {
        throw Except(message);
    }
};

class FilePrintFormat {
  public:

    FilePrintFormat(const char* format, FILE* stream = stdout) :
        out(stream), formatstr(format) {}

    template <typename T>
    void operator()(T t) { fprintf(out, formatstr, t); }

  private:
    FILE* out;
    const char* formatstr;
};

// Print string followed by new-line.
void println(const char* str = "");

class Assert {
  public:
    Assert(bool check, const char* message);

    static bool on;
    static std::function<void(const char*)> fail;
};

class Failure {
  public:
    Failure(const char* pMessage = "Failure") : message(pMessage) {}
    const char* what() const { return message; }

  private:
    const char* message;
};

// Location in source code: filename and line number.
struct SourcePoint {
    const char* filename;
    int line;

    void print() const;
};

#define kwr_FLine  kwr::SourcePoint{ __FILE__, __LINE__ }

// Get size of fixed-length array.
template<class T, size_t N>
constexpr size_t size(T (&)[N]) { return N; }

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


//========================================
// Function Call Sementics

// Pass ownership of pointer to/from function
template <typename Type>
class PassOwner {
  public:
    explicit PassOwner(Type& v) : ptr(v.release()) {}
    typename Type::Pointer get() { return ptr; }

  private:
    typename Type::Pointer ptr;
};

template <typename T>
constexpr PassOwner<T> give(T &t) { return PassOwner<T>(t); }


// Unit Testing
// ===============

class TestFailure {
  public:
    void print() const;
    const char* const message;
};

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

#define kwr_TestCase(name) \
class kwr_TestCase_##name : public kwr::TestCase { \
    public: kwr_TestCase_##name() : TestCase( __FILE__, __LINE__, #name ) {} \
    void run() override; \
} kwr_TestCase_##name##_object; void kwr_TestCase_##name::run()


// Simple Data Types
// ====================

class TerminateHandler {
    static void handler();
    std::terminate_handler old_handler = std::set_terminate(handler);
  public:
    ~TerminateHandler();
};

class AbortNewHandler {
    static void handler();
    std::new_handler old_handler = std::set_new_handler(handler);

  public:
    ~AbortNewHandler();
};

template <typename Type>
struct Buffer : public Memory<Type> {
    Buffer() = default;
    Buffer(Memory<Type> mem) : Memory<Type>({ mem }) {}
    Buffer(int initSize) : Buffer({ initSize, new Type[initSize] }) {}
    ~Buffer() { delete Memory<Type>::data; }

    Memory<Type> release() { 
        return Memory<Type> { 
            std::exchange(Memory<Type>::size, 0),
            std::exchange(Memory<Type>::data, nullptr) 
        };
    }
};

class String {
   
  public:

    String() = default;

    template<size_t N>
    constexpr String(const char (&literal)[N]) : 
      str({ N-1, const_cast<char*>(literal) })
    {}

    String(const String&) = default;
    String& operator=(const String&) = default;

    constexpr char* cstr() const { return str.data; }
    constexpr int length() const { return str.size; }
    bool empty() const { return !str.data || !*str.data; } 
    int compare(String that) const;

    // Todo
    // char operator[](int i);
    // operator!
    // find

  private:

    Memory<char> str;

};

bool operator==(String left, String right);

// =======================
// Tracing 

class Trace {

  public:

    Trace(SourcePoint point, String message_p); 
    void print();
    ~Trace() { gstack<Trace>(next); }

    static int printBacktrace();

  protected:

    SourcePoint source;
    String message;

  private:

    Trace* next = gstack<Trace>(this);

};

#define KWR_TRACE_NAME(trace,line)  kwr_Concat(trace, line)
#define kwr_Trace(message)     kwr::Trace  KWR_TRACE_NAME(trace,__LINE__)  ( kwr_FLine, (message) )

// ======================================================================
// Containers

template <typename Type>
class Array {

  public:

    Array() = default;
    explicit Array(int initSize) : buffer(initSize) {}

    Array(const Array<Type>&) = delete;
    Array<Type>& operator=(const Array<Type>&) = delete;

    Array(Array<Type>&& that) : buffer( that.buffer.release() ) {}

    Array<Type>& operator=(Array<Type>&& that) {
        return Array<Type>( std::move(that) ).swap(*this);
    }

    Array<Type>& swap(Array<Type>& that) {
        std::swap(buffer.size, that.buffer.size);
        std::swap(buffer.data, that.buffer.data);
        return *this;
    }

    Type* data() { return buffer.data; }
    constexpr int size() const { return buffer.size; }
    Memory<Type> get() { return buffer; }
    Memory<Type> release() { return std::exchange(buffer, Memory<Type>()); }

    Type* begin() { return buffer.data; }
    Type* end() { return buffer.data + buffer.size; }

    Type& operator[](int index) { 
        Assert(0 <= index && index < buffer.size, "Array index out of bounds.");
        return buffer.data[index]; 
    }

    ~Array() = default;

  private:

    Buffer<Type> buffer;

};

// ======================================================================
// Sequences

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

#endif
