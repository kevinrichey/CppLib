#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>

#define kwr_Str(x)   #x
#define kwr_NumStr(num)  kwr_Str(num)

struct SourceLine
{
    const char* filename;
    unsigned line;
    const char* caption;
};

class Checkpoint
{
  public:
    Checkpoint(SourceLine here) : sourceline(here) { top = this; }
    ~Checkpoint() { top = next; }

  private:
    const SourceLine sourceline;
    Checkpoint *next = top;
    static Checkpoint* top;
};

Checkpoint* Checkpoint::top = nullptr;

class Failure : std::exception
{
  public:
    Failure(const SourceLine& location) : sourceline(location) {}

    virtual const char* what() const noexcept
    {
        return whatstr;
    }

    void print();

    SourceLine sourceline;
    
  private:

    static const char* whatstr;
};

const char* Failure::whatstr = "Assert Failure";

void Failure::print()
{
    printf("%s:%d: %s {%s}\n", sourceline.filename, sourceline.line, what(), sourceline.caption);
}

class Assertion
{
  public:

    Assertion(bool check, SourceLine location) :
      point(location) 
    {
        if (!check) throw Failure { location };
    }

  private:
    Checkpoint point;
};

#define kwr_Assert(condition) Assertion _assertion_##__LINE__( (condition), {__FILE__, __LINE__, (#condition)});

template <typename Type>
class Array 
{
  public:
    Array(Type* h, size_t len) : head(h), tail(head+len-1) {}
    virtual ~Array() =default;
    Type& operator[](int i) { return head[i]; }

  protected:
    Type* head;
    Type* tail;
};

template <typename Type>
class DynArray : public Array<Type>
{
  public:
    DynArray(int s) : Array<Type>(new Type[s], s) {}
    virtual ~DynArray() { delete[] this->head; }

    const char* layout = "Dynamic";

};

template <typename Type, size_t Size>
class FixArray : public Array<Type>
{
  public:
    FixArray() : Array<Type>(data, Size) {}

    const char* layout = "Static";

  private:
    Type data[Size];
};


int main( int argc, char* args[] ) 
{
    printf("Hello World\n");

    try
    {
        void *p = nullptr;
        kwr_Assert(p);
    }
    catch (Failure &failure)
    {
        failure.print();
    }


    DynArray<int> dynints(100);
    printf("dynints layout: %s\n", dynints.layout);

    FixArray<int, 10> fixints;
    printf("fixints layout: %s\n", fixints.layout);

    return 0;
}

