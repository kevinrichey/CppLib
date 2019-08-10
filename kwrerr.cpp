#include "kwrerr.h"

namespace kwr {

const char EventCategory::Error[]    { "Error" };
const char EventCategory::Defect[]   { "Defect" };
const char EventCategory::Fault[]    { "Fault" };
const char EventCategory::Failure[]  { "Failure" };


void Event::print(kwr::OutStream& out) const
{
    out.print("%s:%d: %s \"%s\"\n", where.filename.cstr(), where.line, category.cstr(), what.cstr());
}


void ThrowHandler::handle(Event& error)
{
    error.raise(); 
}


void PrintHandler::handle(Event& error) 
{
    error.print(kwr::OutStream::console()); 
}


void DefectHandler::handle(Event& error) 
{
    error.print(kwr::OutStream::console()); 
    // backtrace
    std::abort();
}


void require(bool condition, CString message, SourcePoint source)
{
    if (!condition) {
        Defect defect(source, message, "Defect");
        defect.handle();
    }
}

void check(bool condition, CString message, SourcePoint source)
{
    if (!condition) {
        Fault fault(source, message, "Fault");
        fault.handle();
    }
}

void test(bool condition, CString message, SourcePoint source)
{
    if (!condition) {
        Failure failure(source, message, "Failure");
        failure.handle();
    }
}

}; // kwr
