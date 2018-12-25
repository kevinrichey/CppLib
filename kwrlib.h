#pragma once

#include <functional>
#include <cstring>

namespace kwr {

//======================================================================
// Primitive Utilities

#define kwr_Str(x)       #x
#define kwr_NumStr(num)  kwr_Str(num)
#define kwr_Concat(a,b)  a##b

/// Pair a variable value and name.
template <typename T>
struct ValName { T value; const char* name; };
#define kwr_ValName(t)  ValName<decltype((t))> { (t), kwr_Str(t) }

/// Source code location: filename and line #
struct SourceLine
{
    const char* filename;
    unsigned    line;
};

#define kwr_SourceLine  kwr::SourceLine { __FILE__, __LINE__ }

class Checkpoint
{
  public:
    Checkpoint(const SourceLine& here) : sourceline(here) { top = this; }
    ~Checkpoint() { top = next; }

  private:
    const SourceLine sourceline;
    Checkpoint *next = top;
    static Checkpoint* top;
};

#define KWR_CHECKPOINT_NAME(name)   kwr_Concat(_checkpoint_,name)
#define kwr_Checkpoint(msg)         kwr::Checkpoint KWR_CHECKPOINT_NAME(__LINE__)( kwr_SourceLine )

class Failure : public std::exception
{
  public:
    explicit Failure(SourceLine source, const char* errormsg = nullptr) :
      location(source)
    {
        if (errormsg) {
            strncpy(message, errormsg, sizeof(message));
        }
    }

    virtual const char* what() const noexcept { return message; }

    SourceLine source() const { return location; }

  private:
    SourceLine location;
    char message[64] {};
};

bool fail(SourceLine source, const char* label);

class Assertion
{
  public:
    inline Assertion(SourceLine source, bool condition, const char* label) 
    {
        if (!condition) fail(source, label);
    }
};

typedef Assertion Require;

#define kwr_Assert(condition)   kwr::Assertion  _kwr_assert_##__LINE__( kwr_SourceLine, (condition), #condition )



template <typename T> 
class Range
{
  public:
    Range(T first, T last) : head(first), tail(last) {}
    size_t size() const { return tail - head + 1; }
    void pop() { ++head; }
    T get() const { return head; }

  private:
    T head, tail;
};

template <typename T> 
class Range<T*>
{
  public:
    Range(T* first, T* last) : head(first), tail(last) {}
    Range(T* first, int length) : Range(first, first+length-1) {}

    size_t size() const { return tail - head + 1; }
    bool more() const { return size() != 0; }
    void pop() { ++head; }

    void pop(int n)
    { 
        if (n > 0) head += std::min(n, (int)size());
        else       tail += std::max(n, -(int)size());
    }

    const T &get() const { return *head; }
    void put(const T& t) { *head = t; }
    T &operator[](size_t i) { return head[i]; }

  private:
    T* head;
    T* tail;
};

template <typename FromType, typename ToType>
Range<ToType> copy (Range<FromType> from, Range<ToType> to)
{
    auto copied = to;

    while (from.more() && to.more())
    {
        to.put( from.get() );
        from.pop();
        to.pop();
    }

    copied.pop( -to.size() );
    return copied;
}


// Alias any function name.
#define kwr_FnAlias(oldfunc, newfunc) \
  template <typename... Args> \
inline auto newfunc(Args&&... args) -> decltype(oldfunc(std::forward<Args>(args)...)) \
{ return oldfunc(std::forward<Args>(args)...); }


//======================================================================
// Containers & Ranges

template <typename T=int> 
class Counter
{
  public:
    typedef T Type;
    Counter(T start) : current(start) {}
    bool finite() const { return false; }
    void pop() { ++current; }
    T get() const { return current; }
    Range<T> to(T end) { return Range<T>(current, end); }

  private:
    T current;
};

template <typename T>
Counter<T> from(T f) { return Counter<T>(f); }

template <typename T> 
class Array 
{
  public:
    Array(Array<T>& a) = delete;
    Array<T>& operator=(Array<T>& a) = delete;

    explicit Array(size_t n) : first(new T[n]), end(first+n) {}

    Array(Array<T>&& a) : first(std::move(a.first)), end(std::move(a.end))
    {
        a.first = a.end = nullptr;
    }

    template <typename RT>
    Array(Range<RT> range) : Array(range.size())
    {
        for (auto *i = first; i != end; ++i, range.pop()) {
            *i = range.get();
        }
    }

    template <typename RT, typename F>
    Array(Range<RT> range, F function) : Array(range.size())
    {
        for (auto *i = first; range.size(); range.pop()) {
            *i++ = function(range.get());
        }
    }

    size_t size() const { return end-first; }

    T& operator[](size_t i)
    {
        //Require { 0 <= i && i < end-first, "Array index out of bounds" };
        return first[i];
    }

    Range<T> range() { return Range<T>(first, end); }

    ~Array()
    {
        delete[] first;
        first = end = nullptr;
    }

  private:
    T *first, *end;
};

template <typename Ctype>
class StringT
{
  public:
    typedef Ctype Type;

    explicit StringT(size_t strSize) : 
      length(strSize), 
      str(new Type[length+1]) 
    {}

    explicit StringT(const Type* s) :
      StringT(strlen(s))
    {
        strcpy(str, s);
    }

    StringT(const StringT<Type>& other) : 
      StringT(other.length) 
    {
        strcpy(str, other.str);
    }

    void swap(StringT& other)
    {
        std::swap(length, other.length);
        std::swap(str, other.str);
    }

    void copy(const StringT<Type>& other)
    {
        StringT<Type>(other).Swap(*this);
    }

    StringT& operator= (const StringT& other)
    {
        Copy(other);
        return *this;
    }

    size_t size() const
    {
        return length;
    }

    bool equals(const StringT<Type>& other) const
    {
        return !strcmp(str, other.str);
    }

    const Type* const getStr() const
    {
        return str;
    }

    Type* release() 
    {
        Type* s = str;
        str = nullptr;
        return s;
    }

    ~StringT()
    {
        delete[] str;
    }

  private:

    size_t length;
    Type* str;
};

typedef StringT<char> String;


//======================================================================
// Math Operations

extern const double pi;

double lerp(double a, double b, double f);
double coserp(double a, double b, double x);
double sCurve(double x);
double step(double a, double x);
double pulse(double a, double b, double x);
double clamp(double a, double b, double x);
double smoothstep(double a, double b, double x);
double spline(double x, double nknots, double *knots);

} // kwr

