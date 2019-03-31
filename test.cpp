
#include <cstdio>
#include <exception>

#include "kwrlib.h"

using namespace kwr;

kwr_TestCase(PassingTrue)
{
    kwr_Test(true);
}

kwr_TestCase(FailingFalse)
{
    //kwr_Test(false);
}

class CStringTest : public kwr::TestCase {

  public:

    kwr::CString getString() 
    {
        return "hello"; 
    }

    void passString(kwr::CString str) 
    {
        kwr_Test(str == kwr::CString("world")); 
    }

    virtual void run() 
    {
        // Default constructor
        kwr::CString nullstr;
        kwr_Test(nullstr.length() == 0);
        kwr_Test(nullstr.empty());

        // Literal CString constructor
        kwr::CString literal("xyzzy");
        kwr_Test(literal.length() == 5);
        kwr_Test(literal == "xyzzy");
        kwr_Test(!literal.empty());

        // Copy constructor
        kwr::CString copy(literal);
        kwr_Test(copy.cstr() == literal.cstr());
        kwr_Test(copy == "xyzzy");

        // Copy assignment operator
        kwr::CString assigned;
        assigned = literal;
        kwr_Test(assigned == "xyzzy");
        kwr_Test(assigned.cstr() == literal.cstr());

        // Return value
        kwr::CString hello( getString() );
        kwr_Test(hello == "hello");

        // Implicit constructor
        passString("world");

        // Implicit assignment
        kwr::CString implicit;
        implicit = "implied";
        kwr_Test(implicit == "implied");
    }
} kwr_TestCase_CStringTest;

kwr_TestCase(ArrayTest)
{
    // Default constructor
    Array<int> nullArray;
    kwr_Test(nullArray.size() == 0);
    kwr_Test(nullArray.empty());
    kwr_Test(!nullArray);
    kwr_Test(false == nullArray);
    
    // Allocate constructor
    Array<int> tenints(10);
    kwr_Test(tenints.size() == 10);
    kwr_Test(!tenints.empty());
    kwr_Test(true == tenints);

    // Accessor
    for (int i = 0; i < tenints.size(); ++i) {
        tenints[i] = (i+1) * 10;
    }
    kwr_Test(50 == tenints[4]);


    // Swap
    Array<char> a(5);
    Array<char> b(5);
    for (int i = 0; i < a.size(); ++i) a[i] = 'a' + i;
    for (int i = 0; i < b.size(); ++i) b[i] = 'f' + i;
    a.swap(b);

    kwr_Test(b[0] == 'a');
    kwr_Test(b[1] == 'b');
    kwr_Test(b[2] == 'c');
    kwr_Test(b[3] == 'd');
    kwr_Test(b[4] == 'e');

    kwr_Test(a[0] == 'f');
    kwr_Test(a[1] == 'g');
    kwr_Test(a[2] == 'h');
    kwr_Test(a[3] == 'i');
    kwr_Test(a[4] == 'j');

    // Move
    Array<char> m(5);
    for (int i = 0; i < m.size(); ++i) m[i] = 'k' + i;
    kwr_Test(m[0] == 'k');
    kwr_Test(m[1] == 'l');
    kwr_Test(m[2] == 'm');
    kwr_Test(m[3] == 'n');
    kwr_Test(m[4] == 'o');

    m.move(a);
    kwr_Test(!a);
    kwr_Test(m[0] == 'f');
    kwr_Test(m[1] == 'g');
    kwr_Test(m[2] == 'h');
    kwr_Test(m[3] == 'i');
    kwr_Test(m[4] == 'j');

    // Dispose
    Array<int> d(10);
    kwr_Test(10 == d.size());
    d.dispose();
    kwr_Test(0 == d.size());
    kwr_Test(!d);

    // Release
    // I know this would leak if the test fails.
    // It's just a test case.
    Array<char> r(3);
    r[0] = 'p';
    r[1] = 'q';
    r[2] = 'r';
    kwr_Test(3 == r.size());
    auto rspan = r.release();
    kwr_Test(!r);
    kwr_Test(3 == rspan.size);
    kwr_Test('r' == rspan.data[2]);
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
    kwr_Test(s.size() == 5);
    kwr_Test('v' == s[0]);
    kwr_Test('w' == s[1]);
    kwr_Test('x' == s[2]);
    kwr_Test('y' == s[3]);
    kwr_Test('z' == s[4]);

    kwr_Test(0 == sspan.size);
    kwr_Test(nullptr == sspan.data);
    
    // Negative size
    //Array<char> f(-10);
}



int main(int argc, char* argv[])
{ 
    auto console = kwr::OutStream::console();

    console.print("Run Tests\n");

    int count = 0;
    int failures = 0;
    auto test = kwr::TestCase::sequence();

    while (test.pending()) {
        ++count;
        try {
            test->run();
        }
        catch (kwr::TestFailure& failure) {
            ++failures;
            test->print(console);
            failure.print(console);
        }
        catch (kwr::Failure& failure) {
            ++failures;
            test->print(console);
            console.print("Assertion failure!  %s\n", failure.message);
        }
        catch (std::exception& except) {
            ++failures;
            test->print(console);
            console.print("exception: %s\n", except.what());
        }
        test.next();
    }

    console.print("%s!  Tests: %d, Failures: %d\n", failures? "Failed": "Success", count, failures);

	  return failures;
}

