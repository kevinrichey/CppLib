
#include <cstdio>
#include <exception>

#include "kwrlib.h"

namespace kwr {
 
}

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
        kwr_Test(str.compare("world").equal()); 
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
        kwr_Test(assigned.compare("xyzzy").equal());
        kwr_Test(assigned.cstr() == literal.cstr());

        // Return value
        kwr::CString hello( getString() );
        kwr_Test(hello.compare("hello").equal());

        // Implicit constructor
        passString("world");

        // Implicit assignment
        kwr::CString implicit;
        implicit = "implied";
        kwr_Test(implicit.compare("implied").equal());

        // String comparison
        kwr::CString aa("aa");
        kwr::CString bb("bb");
        kwr::CString cc("cc");
        kwr::CString a2("aa");
        kwr_Test(aa.compare(nullstr).greater());
        kwr_Test(aa.compare(aa).equal());
        kwr_Test(aa.compare(bb).less());
        kwr_Test(cc.compare(bb).greater());
    }
} kwr_TestCase_CStringTest;

int main(int argc, char* argv[])
{ 
    auto console = kwr::OutStream::console();

    printf("Run Tests\n");

    int count = 0;
    int failures = 0;
    auto test = kwr::TestCase::sequence();

    while (test) {
        ++count;
        try {
            test->run();
        }
        catch (kwr::TestFailure& failure) {
            ++failures;
            test->print(console);
            failure.print(console);
        }
        //catch (kwr::Failure& failure) {
        //++failures;
        //test->print();
        //printf("Assertion failure!  %s\n", failure.what());
        //}
        catch (std::exception& except) {
            ++failures;
            test->print(console);
            printf("exception: %s\n", except.what());
        }
        ++test;
    }

    printf("%s!  Tests: %d, Failures: %d\n", failures? "Failed": "Success", count, failures);

	  return failures;
}

