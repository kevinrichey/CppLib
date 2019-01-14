#include <cstdio>
#include <limits>
#include "kwrlib.h"

namespace kwr {

enum class ReturnCode { Success = 0, Error, Failure };


} // kwr namespace

void foo()
{
    kwr_Scope("foo()");

    kwr_Trace("doing nothing");
}


int main(int argc, char* args[]) 
{
    kwr::Trace::turn(kwr::off);

    kwr_Scope("main()");
    kwr_Trace("Hello, world!");

    kwr::CString programCommand( args[0] );
    printf("Starting %s\n", programCommand.cstr());

    kwr_Watch(argc);
    
    printf("Hello, %s\n", args[1]);

    foo();

    //kwr_Assert(1 == 2);

    return static_cast<int>(kwr::ReturnCode::Success);
}

