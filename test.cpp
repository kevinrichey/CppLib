
#include <cstdio>
#include <exception>

#include "kwrlib.h"
#include "kwrerr.h"

using namespace kwr;

kwr_TestCase(PassingTrue)
{
    kwr_test(true);
}

kwr_TestCase(FailingFalse)
{
    //kwr_test(false);
}

class CStringTest : public kwr::TestCase {

  public:

    kwr::CString getString() 
    {
        return "hello"; 
    }

    void passString(kwr::CString str) 
    {
        kwr_test(str == kwr::CString("world")); 
    }

    virtual void run() 
    {
        // Default constructor
        kwr::CString nullstr;
        kwr_test(nullstr.length() == 0);
        kwr_test(nullstr.empty());

        // Literal CString constructor
        kwr::CString literal("xyzzy");
        kwr_test(literal.length() == 5);
        kwr_test(literal == "xyzzy");
        kwr_test(!literal.empty());

        // Copy constructor
        kwr::CString copy(literal);
        kwr_test(copy.cstr() == literal.cstr());
        kwr_test(copy == "xyzzy");

        // Copy assignment operator
        kwr::CString assigned;
        assigned = literal;
        kwr_test(assigned == "xyzzy");
        kwr_test(assigned.cstr() == literal.cstr());

        // Return value
        kwr::CString hello( getString() );
        kwr_test(hello == "hello");

        // Implicit constructor
        passString("world");

        // Implicit assignment
        kwr::CString implicit;
        implicit = "implied";
        kwr_test(implicit == "implied");
    }
} kwr_TestCase_CStringTest;

kwr_TestCase(ArrayTest)
{
    // Default constructor
    Array<int> nullArray;
    kwr_test(nullArray.size() == 0);
    kwr_test(nullArray.empty());
    
    // Allocate constructor
    Array<int> tenints(10);
    kwr_test(tenints.size() == 10);
    kwr_test(!tenints.empty());

    // Accessor
    for (int i = 0; i < tenints.size(); ++i) {
        tenints[i] = (i+1) * 10;
    }
    kwr_test(50 == tenints[4]);


    // Swap
    Array<char> a(5);
    Array<char> b(5);
    for (int i = 0; i < a.size(); ++i) a[i] = 'a' + i;
    for (int i = 0; i < b.size(); ++i) b[i] = 'f' + i;
    a.swap(b);

    kwr_test(b[0] == 'a');
    kwr_test(b[1] == 'b');
    kwr_test(b[2] == 'c');
    kwr_test(b[3] == 'd');
    kwr_test(b[4] == 'e');

    kwr_test(a[0] == 'f');
    kwr_test(a[1] == 'g');
    kwr_test(a[2] == 'h');
    kwr_test(a[3] == 'i');
    kwr_test(a[4] == 'j');

    // Move
    Array<char> m(5);
    for (int i = 0; i < m.size(); ++i) m[i] = 'k' + i;
    kwr_test(m[0] == 'k');
    kwr_test(m[1] == 'l');
    kwr_test(m[2] == 'm');
    kwr_test(m[3] == 'n');
    kwr_test(m[4] == 'o');

    m.move(a);
    kwr_test(a.empty());
    kwr_test(m[0] == 'f');
    kwr_test(m[1] == 'g');
    kwr_test(m[2] == 'h');
    kwr_test(m[3] == 'i');
    kwr_test(m[4] == 'j');

    // Dispose
    Array<int> d(10);
    kwr_test(10 == d.size());
    d.dispose();
    kwr_test(0 == d.size());
    kwr_test(d.empty());

    // Release
    // I know this would leak if the test fails.
    // It's just a test case.
    Array<char> r(3);
    r[0] = 'p';
    r[1] = 'q';
    r[2] = 'r';
    kwr_test(3 == r.size());
    auto rspan = r.release();
    kwr_test(r.empty());
    kwr_test(3 == rspan.size);
    kwr_test('r' == rspan.data[2]);
    delete[] rspan.data;

    // Reset
    Array<char> s(3);
    s[0] = 's';
    s[1] = 't';
    s[2] = 'u';

    Span<char> sspan { 5, new char[5] };
    sspan.data[0] = 'v';
    sspan.data[1] = 'w';
    sspan.data[2] = 'x';
    sspan.data[3] = 'y';
    sspan.data[4] = 'z';
    
    s.reset(sspan);
    kwr_test(s.size() == 5);
    kwr_test('v' == s[0]);
    kwr_test('w' == s[1]);
    kwr_test('x' == s[2]);
    kwr_test('y' == s[3]);
    kwr_test('z' == s[4]);

    //kwr_test(0 == sspan.size);
    //kwr_test(nullptr == sspan.data);
    
    // Negative size
    //Array<char> f(-10);
}

namespace kwr {

class File
{
  public:
    enum Mode { In, Out };

    File(CString name, CString mode) 
    {
        handle = fopen(name.cstr(), mode.cstr());
        if (!handle)
        {
            throw Fault(kwr_FileLine, strerror(errno));
        }
    }

    template<typename T>
    int read(T* data, int count=1)
    {
        return fread(data, sizeof(T), count, handle);
    }

    ~File() { if (!handle) fclose(handle); }

  private:
    FILE*   handle;
};

}

kwr_TestCase(FileTest)
{
    int size = 25;
    File file("fortytwo.txt", "r");
    char text[50];
    int count = file.read(text, size);
    text[count] = '\0';

    kwr_test(size == count);
    kwr_test(CString("Life Universe Everything\n") == CString(text));
}

kwr_TestCase(KwrErr)
{
    EventHandler* handler = Defect::handler;

    try {
        int x = 1;
        int line = 0;

        ThrowHandler thrower;
        Defect::handler = &thrower;

        try {
            line = __LINE__+1;
            kwr_require(x == 2);
            kwr_test(!"Defect not thrown");
        }
        catch (Defect& defect) {
            kwr_test(defect.what == "x == 2");
            kwr_test(defect.where.line == line);
            kwr_test(defect.where.filename == __FILE__);
        }
    }
    catch(...) {
        Defect::handler = handler;
        throw;
    }
}

void bstringParam(BString bstr)
{
    kwr_test(!bstr.empty());
    kwr_test(bstr.length() == 5);
    kwr_test(!strcmp(bstr.cstr(), "xyzzy"));
}

kwr_TestCase(BStringTest)
{
    BString defctr;
    kwr_test(defctr.empty());
    kwr_test(defctr.length() == 0);
    kwr_test(!strcmp(defctr.cstr(), ""));

    BString strctr("xyzzy");
    kwr_test(!strctr.empty());
    kwr_test(strctr.length() == 5);
    kwr_test(!strcmp(strctr.cstr(), "xyzzy"));

    const char* text = "one two three 456";
    BString one(text, text+3);
    kwr_test(!one.empty());
    kwr_test(one.length() == 3);
    kwr_test(!strncmp(one.cstr(), "one", one.length()));

    BString onecmp("one");
    kwr_test(!one.compare(one));

    const char* s = text+14;
    const char* e = s;
    while (*e++);
    BString numbers(s, e);
    kwr_test(atoi(numbers.cstr()) == 456);
}

int main(int argc, char* argv[])
{ 
    auto console = kwr::OutStream::console();

    ThrowHandler thrower;
    Failure::handler = &thrower;
    Defect::handler = &thrower;
    Fault::handler = &thrower;

    console.print("Run Tests\n");

    int count = 0;
    int failures = 0;
    auto test = kwr::TestCase::sequence();

    while (test.more()) {
        ++count;
        try {
            test->run();
        }
        catch (kwr::Error& error) {
            ++failures;
            test->print(console);
            error.print(console);
        }
        catch (std::exception& except) {
            ++failures;
            test->print(console);
            console.print("std::exception: %s\n", except.what());
        }
        catch (...) {
            ++failures;
            test->print(console);
            console.print("Unknown exception.\n");
        }
        test.next();
    }

    console.print("%s!  Tests: %d, Failures: %d\n", failures? "Failed": "Success", count, failures);

	  return failures;
}

