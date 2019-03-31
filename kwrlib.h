#ifndef KWR_HEADER_KWRLIB_H
#define KWR_HEADER_KWRLIB_H

#include <type_traits>
#include <algorithm>
#include <utility>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>

namespace kwr {

#define kwr_Str(x)       #x
#define kwr_NumStr(num)  kwr_Str(num)
#define kwr_Concat(a,b)  a##b
#define kwr_UniqueName(name, num)  kwr_Concat(name, num)

extern bool on;
extern bool off;

//============================================================
// Object Attributes

class Uncopyable {
  protected:
    constexpr Uncopyable() = default;
    ~Uncopyable() = default;

    Uncopyable(const Uncopyable&) = delete;
    Uncopyable& operator=(const Uncopyable&) = delete;
};

class StackOnly {
    static void *operator new     (size_t) = delete;
    static void *operator new[]   (size_t) = delete;
    static void  operator delete  (void*)  = delete;
    static void  operator delete[](void*)  = delete;
};

// Other names?  Polymorphic, Heavy, Object, HeapOnly
class Complex : public Uncopyable {
  protected:
    Complex() = default;
    virtual ~Complex() = default;
    static void *operator new[]   (size_t) = delete;
    static void  operator delete[](void*)  = delete;
};

//======================================================
// Basic Types & Resource Handles

class CString {
  public:
    CString(const char* str, int len) : strdata(str), strlength(len) {}
    CString(const char* str) : CString(str, str? std::strlen(str): 0) {}

    template<size_t LEN>
    constexpr CString(const char (&literal)[LEN]) : 
      CString(literal, LEN-1)
    {}

    CString() = default;
    CString(const CString&) = default;
    CString& operator=(const CString&) = default;

    const char* cstr() const { return strdata; }

    int  length()  const { return strlength;     }
    bool empty()   const { return length() == 0; }

    explicit operator const char*() const { return strdata; }
    bool operator!() const { return empty(); }

    int compare(const CString& that) const;

    // range
    // sequence

  private:

    const char* strdata {};
    int   strlength {};
};

static_assert(std::is_trivially_copyable<CString>::value, "CString not trivial");

bool operator==(const CString& lhs, const CString& rhs);

template <typename T>
class Object : public Uncopyable, public StackOnly {
  public:
    explicit Object(T* ptr = nullptr) : pointer(ptr) {}
    ~Object() { delete pointer; }

    bool empty() const { return pointer == nullptr; }

    T*       operator->()       { return  pointer; }
    const T* operator->() const { return  pointer; }
    T&       operator*()        { return  pointer; }
    const T& operator*()  const { return  pointer; }
    bool     operator!()  const { return  empty(); }
    operator bool()       const { return !empty(); }

    void swap(Object<T>& h)  { std::swap(pointer, h.pointer); }
    void move(Object<T>& h)  { swap(Object<T>().swap(h)); }
    void reset(T* p)         { Object<T>(p).swap(*this); }
    void dispose()           { Object<T>().swap(*this); }
    T*   release()           { return std::exchange(pointer, nullptr); }

  private:
    T* pointer {};
};

template <typename T>
struct Span {
    int size = 0;
    T*  data = nullptr;

    void swap(Span<T>& other)
    {
        std::swap(size, other.size);
        std::swap(data, other.data);
    }

    void move(Span<T>& from)
    {
        size = std::exchange(from.size, 0);
        data = std::exchange(from.data, nullptr);
    }
};

class Failure {
  public:
    Failure(CString msg) : message(msg) {}
    CString message;
};

class ParamFailure : public Failure {
  public:
    ParamFailure(CString msg) : Failure(msg) {}
};

template <typename T>
struct Positive {
    Positive(T set) : value(set) 
    { 
        if (value <= 0) throw ParamFailure("parameter must be greater than zero");
    }

    T value;
};

template <typename T>
class Array {
  public:
    Array() = default;
    Array(Span<T> span) : array(span) {}
    Array(Positive<int> newSize) : Array({newSize.value, new T[newSize.value]}) {}
    ~Array() { delete[] array.data; array.data = nullptr; }

    T& operator[](int i) { return array.data[i]; }
    const T& operator[](int i) const { return array.data[i]; }

    int  size()  const { return array.size; }
    bool empty() const { return array.data == nullptr; }
    bool operator!() const { return empty(); }
    operator bool() const  { return !empty(); }

    void swap(Array<T>& other)
    {
        array.swap(other.array);
    }

    void move(Array<T>& other)
    {
        array.move(other.array);
    }

    void dispose() { Array<T>().swap(*this); }

    Span<T> release() 
    {
        Span<T> result;
        result.swap(this->array);
        return result;
    }

    void reset(Span<T>& span)
    {
        Array<T>(span).swap(*this);
        span = Span<T>();
    }

  private:
    Span<T> array;
};


class OutStream {
  public:
    int print(CString s);
    int print(int i);
    int print(double d);
    int print(bool b);
    int print(char c);
    int print(const char* format...);

    static OutStream& console();
    static OutStream& error();
    static OutStream& null();

  protected:
    OutStream() = default;
    explicit OutStream(FILE* out_file) : file(out_file) {}

  private:
    FILE* file {};
};

class Sequence {
  public:
    virtual void next() =0;
    virtual bool pending() const =0;
    virtual void print(OutStream&) const =0;
    virtual ~Sequence() = default;
};

int print(Sequence& sequence, OutStream& out);

class Value {
  public:
    virtual void print(OutStream& out) const =0;
};

template <typename T>
class Wrapper : public Value {
  public:
    T data;

    explicit Wrapper(const T& t) : data(t) {}
    virtual void print(OutStream& out) const { out.print(data); }
};

template <typename T>
Wrapper<T>* wrap(const T& v) { return new Wrapper<T>(v); }

// Global stack of objects.
//    Add next member:        Type* next = gstack<T>(this);
//    Remove in destructor:   ~Type() { gstack(next); }
//    Access first element:   auto top = gstack<T>();
template <typename Type>
Type* gstack(Type* me = nullptr)
{
    static Type* top = nullptr;
    Type* result = top;
    if (me) top = me;
    return result;
}

template <typename T>
class GStackSequence : public Sequence {
  public:
    T*   get()  { return element; }
    void next() { element = element->next; }
    bool pending() const { return element; }
    void print(OutStream& out) const { element->print(out); }
    T* operator->() { return get(); }

  private:
    T* element = gstack<T>(); 
};

// ============================================================ 
// Source Tracing

struct SourcePoint {
    SourcePoint(CString file, int l) : filename(file), line(l) {}
    CString filename;
    int line;

    void print(OutStream& out) const;
};

#define kwr_FileLine  kwr::SourcePoint(__FILE__, __LINE__)

class Trace {

  public:
    explicit Trace(SourcePoint point, CString cat, CString message);
    explicit Trace(SourcePoint point, CString message);

    Trace* const next = gstack<Trace>(this);

    void report() const;
    virtual void print(OutStream& out) const;

    virtual ~Trace();

    static void print(OutStream& out, SourcePoint where, std::time_t when, CString category, CString what, CString end="\n");
    static GStackSequence<Trace> backtrace();

    static void turn(bool mode);
    static void set(OutStream& new_out);

  protected:
    SourcePoint where;
    std::time_t when;
    CString     category;
    CString     what;

    static bool trace_on;
    static OutStream ostream;
};

#define kwr_Trace(message)   \
  kwr::Trace kwr_UniqueName(kwr_Trace_, __LINE__) ( kwr_FileLine, (message) )

class ScopeTrace : public Trace {

  public:
    explicit ScopeTrace(SourcePoint point, CString message);
    virtual void print(OutStream& out) const;
    virtual ~ScopeTrace();
};

#define kwr_Scope(message)   \
  kwr::ScopeTrace kwr_UniqueName(kwr_Scope_, __LINE__) ( kwr_FileLine, (message) )

class WatchTrace : public Trace {

  public:
    template <typename T>
    WatchTrace(SourcePoint point, CString name, T value) :
      Trace(point, "Watch", name),
      watched(wrap(value))
    {
        report();
    }

    virtual void print(OutStream& out) const;

  private:
    Object<Value> watched;

};

#define kwr_Watch(variable)   \
  kwr::WatchTrace kwr_UniqueName(kwr_Watch_, __LINE__) ( kwr_FileLine, #variable, (variable) )

template <typename Handler>
void Assertion(SourcePoint source, bool condition, CString message)
{
    if (!condition) Handler::fail(source, message);
}

class AssertHandler {
  public:
    static void fail(SourcePoint source, CString message);
};

#define kwr_Assert(condition)  \
  kwr::Assertion<kwr::AssertHandler>( kwr_FileLine, (condition), kwr_Str(condition) )

// ============================================================ 
// Unit Testing

class TestFailure {
  public:
    SourcePoint source;
    CString message;

    void print(OutStream& out) const;
};

class TestCase {
  public:
    virtual void run() = 0;
    virtual const CString name() const;
    void print(OutStream& out) const;

    TestCase* const next = gstack<TestCase>(this);
    virtual ~TestCase();

    static void fail(SourcePoint point, CString message);
    static GStackSequence<TestCase> sequence();
};

#define kwr_TestCase(TESTNAME) \
struct TESTNAME: public kwr::TestCase { \
    virtual const kwr::CString name() const { return kwr_Str(TESTNAME); } \
    virtual void run(); \
} kwr_UniqueName(kwr_TestCase_,TESTNAME); \
void TESTNAME::run() 

#define kwr_Test(condition)  \
  kwr::Assertion<kwr::TestCase>( kwr_FileLine, (condition), kwr_Str(condition) )

} // kwr namespace

#endif
