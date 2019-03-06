#ifndef KWR_HEADER_KWRLIB_H
#define KWR_HEADER_KWRLIB_H

#include <type_traits>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cstdio>

namespace kwr {

#define kwr_Str(x)       #x
#define kwr_NumStr(num)  kwr_Str(num)
#define kwr_Concat(a,b)  a##b
#define kwr_UniqueName(name, num)  kwr_Concat(name, num)

extern bool on;
extern bool off;

class CString;
class OutStream;

class Object {
  public:
    virtual void print() const;
    virtual void print(OutStream&) const;
    virtual CString name() const;
    virtual ~Object() {}
};
    
template <typename T>
class Handle {
  public:
    explicit Handle(T* ptr = nullptr) : pointer(ptr) {}
    T* operator->() { return pointer; }
    const T* operator->() const { return pointer; }
    ~Handle() { delete pointer; }

    Handle(const Handle<T>&) = delete;
    Handle(const Handle<T>&&) = delete;
    Handle& operator=(const Handle<T>&) = delete;
    Handle& operator=(const Handle<T>&&) = delete;

  private:
    T* pointer {};
};

class Compared {

  public:
    Compared(int r) : result(r) {}
    bool equal()   const { return result == 0; }
    bool greater() const { return result > 0; }
    bool less()    const { return result < 0; }
    int  value()   const { return result; }
    operator int() const { return result; }

  private:
    int result;
};

class IxSequence : public Object {
  public:
    virtual void next() =0;
    virtual bool pending() const =0;
    virtual CString name() const;
    virtual ~IxSequence() {}
};

int print(IxSequence& sequence, OutStream& out);

template <typename T>
class Sequence : public IxSequence {
  public:
    virtual T*   get()  =0;

    T* operator->()   { return get(); }
    void operator++() { next(); }
    operator bool() const { return pending(); }
};


class CString {
  public:
    CString(const char* str, int len) : strdata(str), strlength(len) {}
    CString(char* str) : CString(str, str? std::strlen(str): 0) {}

    template<size_t LEN>
    constexpr CString(const char (&literal)[LEN]) : 
      CString(literal, LEN-1)
    {}

    CString() = default;
    CString(const CString&) = default;
    CString& operator=(const CString&) = default;

    const char* cstr() const { return strdata; }
    explicit operator const char*() const { return strdata; }

    int length()  const { return strlength; }
    bool empty()  const { return length() == 0; }
    bool operator!() const { return empty(); }

    Compared compare(const CString& that) const;

    // range
    // sequence

  private:

    const char* strdata {};
    int   strlength {};
};

bool operator==(const CString& lhs, const CString& rhs);

class OutStream {
  public:
    explicit OutStream(FILE* out_file) : file(out_file) {}

    virtual void set(FILE* out_file) { file = out_file; }
    virtual explicit operator FILE*() { return file; }

    virtual int print(const char* s);
    virtual int print(CString s);
    virtual int print(int i);
    virtual int print(double d);
    virtual int print(bool b);
    virtual int print(char c);

    static OutStream& console();
    static OutStream& error();
    static OutStream& null();

  protected:
    OutStream() = default;

  private:
    FILE* file {};

};



class Value {
  public:
    virtual void print(OutStream& out) const;
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
class GStackSequence : public Sequence<T> {
  public:
    T*   get()  { return element; }
    void next() { element = element->next; }
    bool pending() const { return element; }
    void print(OutStream& out) const { element->print(out); }

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
    Handle<Value> watched;

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
