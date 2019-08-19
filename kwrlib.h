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

template <typename T>
T min(T a, T b, T c)
{
    return std::min( std::min(a,b), c);
}

//======================================================
// Basics

// Disable array-new
// static void *operator new[]   (size_t) = delete;
// static void  operator delete[](void*)  = delete;

class Object {
  protected:
    constexpr Object() = default;
    virtual ~Object() = default;
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
};

class String {
  public:
    String() = default;
    virtual int   length() const =0;
    virtual bool  empty() const =0;
    virtual const char* cstr() const =0;
};

int compare(const String& leftstr, const String& rightstr);

bool operator==(const String& leftstr, const String& rightstr);

struct Result {
    bool found = false;
    int  index = 0;

    operator bool() { return found; }
    operator int()  { return index; }
};

class BString : public String {
  public:
    BString() = default;
    BString(const char* s) : beginstr(s), endstr(s + std::strlen(s)) {}
    BString(const char* b, const char* e) : beginstr(b), endstr(e) {}
    BString(const BString& from) : beginstr(from.beginstr), endstr(from.endstr) {}
    BString& operator=(const BString&) = default;

    virtual int  length() const { return endstr-beginstr;   }
    virtual bool empty()  const { return !beginstr  ||  beginstr == endstr; }
    virtual const char* cstr() const { return beginstr; }

    virtual int  compare(const BString& that) const;

    Result find(char c)
    {
        const char* p = beginstr;
        while (p < endstr  &&  *p != c) ++p;
        return { *p == c, (int)(p - beginstr) };
    }

    BString take(int num)
    {
        return BString(beginstr, beginstr+num);
    }

    BString drop(int num)
    {
        return BString(beginstr+num, endstr);
    }

  private:
    static const char* emptystr;
    const char* beginstr = emptystr;
    const char* endstr   = emptystr;
};

class CString : public String {
  public:
    CString() = default;
    CString(const char* s) : str(s) {}
    CString(const CString&) = default;
    CString& operator=(const CString&) = default;

    virtual const char* cstr() const { return str; }
    virtual int  length() const { return str? strlen(str): 0;   }
    virtual bool empty()  const { return !str || !*str; }
    virtual int  compare(const CString& that) const;

  private:
    const char* str = nullptr;
};
bool operator==(const CString& lhs, const CString& rhs);


template <typename T>
struct Span {
    int size = 0;
    T*  data = nullptr;
};

template<typename T>
T scan(const String& s); 

template<> int scan<int>(const String& s);
template<> BString scan<BString>(const String& s);

class OutStream {
  public:
    int print(const String &s);
    int print(CString s);
    int print(int i);
    int print(unsigned int i);
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
    virtual bool more() const =0;
    virtual void print(OutStream&) const =0;  // Don't like this, break this
    virtual ~Sequence() = default;
};
int print(Sequence& sequence, OutStream& out);

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
    bool more() const { return element; }
    void print(OutStream& out) const { element->print(out); }
    T* operator->() { return get(); }

  private:
    T* element = gstack<T>(); 
};

///------------------------------------------------------
///# Quality

///## Error Handling
struct SourcePoint {
    CString filename;
    int line;

    void print(OutStream& out) const;
};

#define kwr_FileLine  kwr::SourcePoint{__FILE__, __LINE__}

void require(bool condition, CString message, SourcePoint source);
#define kwr_require(condition)   kwr::require((condition), #condition, kwr_FileLine)

void check(bool condition, CString message, SourcePoint source);
#define kwr_check(condition)   kwr::check((condition), #condition, kwr_FileLine)

void test(bool condition, CString message, SourcePoint source);
#define kwr_test(condition)   kwr::test((condition), #condition, kwr_FileLine)

///## Unit Testing

class TestCase {
  public:
    virtual void run() = 0;
    virtual const CString name() const;
    void print(OutStream& out) const;

    TestCase* const next = gstack<TestCase>(this);
    virtual ~TestCase();

    static GStackSequence<TestCase> sequence();
};

#define kwr_TestCase(TESTNAME) \
struct TESTNAME: public kwr::TestCase { \
    virtual const kwr::CString name() const { return kwr_Str(TESTNAME); } \
    virtual void run(); \
} kwr_UniqueName(kwr_TestCase_,TESTNAME); \
void TESTNAME::run() 


//======================================================
// Resources

class Resource {
  public:
    constexpr Resource() = default;
    ~Resource() = default;
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;

    static void *operator new     (size_t) = delete;
    static void *operator new[]   (size_t) = delete;
    static void  operator delete  (void*)  = delete;
    static void  operator delete[](void*)  = delete;

    virtual bool empty() const =0;
    virtual void dispose() =0;
};

template <typename T>
class Handle : public Resource {
  public:
    explicit Handle(T* ptr = nullptr) : pointer(ptr) {}
    ~Handle() { delete pointer; }

    bool empty() const { return pointer == nullptr; }

    T*       operator->()       { return  pointer; }
    const T* operator->() const { return  pointer; }
    T&       operator*()        { return  pointer; }
    const T& operator*()  const { return  pointer; }

    T*   release()           { return std::exchange(pointer, nullptr); }
    void swap(Handle<T>& h)  { std::swap(pointer, h.pointer); }
    void reset(T* p)         { Handle<T>(p).swap(*this); }
    void move(Handle<T>& h)  { reset(h.release()); }
    void dispose()           { reset(nullptr); }

  private:
    T* pointer {};
};

template <typename T>
class Array : public Resource {
  public:
    Array() = default;
    Array(Span<T> span) : array(span) {}
    Array(int newSize) : Array({newSize, new T[newSize]}) {}
    ~Array() { delete[] array.data; array.data = nullptr; }

    T& operator[](int i) { return array.data[i]; }
    const T& operator[](int i) const { return array.data[i]; }

    int  size()      const { return array.size; }
    bool empty()     const { return array.data == nullptr; }

    Span<T> release()             { return std::exchange(array, Span<T>()); }
    void    swap(Array<T>& other) { std::swap(array, other.array); }
    void    reset(Span<T> span)   { Array<T>(span).swap(*this); }
    void    move(Array<T>& other) { reset(other.release()); }
    void    dispose()             { reset(Span<T>()); }

  private:
    Span<T> array;
};

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

// ============================================================ 
// Source Tracing

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


// ============================================================ 
// Configuration

template<typename T>
class Attrib {
  public:
    Attrib(BString n, T v): name(n), value(v) {}
    void set(const String& s) { value = scan<T>(s); }
    operator T() const { return value; }

    BString name;
    T value;
};

#define kwr_Attrib(name, type, value)   kwr::Attrib<type> name { #name, value }

struct Argument {
    BString param;
    BString name;
    BString value;
};

struct Options {
    virtual void set(const Argument& arg) =0;
    void getargs(int argc, char* args[], char separator = '=');
};


} // kwr namespace

#endif
