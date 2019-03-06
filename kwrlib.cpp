
#include "kwrlib.h"
#include <cstdio>

namespace kwr {

bool on = true;
bool off = false;


void Object::print() const
{
    auto console = OutStream::console();
    print(console);
}

void Object::print(OutStream& out) const
{
    out.print(name());
}

CString Object::name() const
{
    return "Object";
}

CString IxSequence::name() const
{
    return "IxSequence";
}

void Value::print(OutStream& out) const { out.print("Value"); }

Compared CString::compare(const CString& that) const
{
    const char* left  = strdata? strdata : "";
    const char* right = that.strdata? that.strdata : "";
    int   maxlen = std::min(strlength, that.strlength) + 1; 
    return std::strncmp(left, right, maxlen);
}

bool operator==(const CString& lhs, const CString& rhs)
{
    return lhs.compare(rhs).equal();
}

int OutStream::print(const char* s)  { return file? fprintf(file, "%s", s): 0; }
int OutStream::print(CString s)      { return file? fprintf(file, "%s", (const char*)s): 0; }
int OutStream::print(int i)          { return file? fprintf(file, "%d", i): 0; }
int OutStream::print(double d)       { return file? fprintf(file, "%f", d): 0; }
int OutStream::print(bool b)         { return file? fprintf(file, "%s", b? "true" : "false"): 0; }
int OutStream::print(char c)         { return file? fprintf(file, "%c", c): 0; }

OutStream& OutStream::console()
{
    static OutStream console_out(stdout);
    return console_out;
}

OutStream& OutStream::error()
{ 
    static OutStream error_out(stderr);
    return error_out;
}

OutStream& OutStream::null()
{
    static OutStream null_out(nullptr);
    return null_out;
}


int print(IxSequence& sequence, OutStream& out)
{
    while (sequence.pending()) {
        sequence.print(out);
        sequence.next();
    }
}

void SourcePoint::print(OutStream& out) const
{
    fprintf((FILE*)out, "%s:%d: ", (const char*)filename, line);
}


void TestFailure::print(OutStream& out) const
{
    source.print(out);
    fprintf((FILE*)out, "failure: %s\n", message.cstr());
}

const CString TestCase::name() const 
{
    return "TestCase";
}

void TestCase::print(OutStream& out) const 
{
    printf("Test case: %s\n", name().cstr()); 
}

void TestCase::fail(SourcePoint source, CString message)
{
    throw TestFailure { source, message };
}

GStackSequence<TestCase> TestCase::sequence()
{
    return GStackSequence<TestCase>();
}

TestCase::~TestCase() 
{ 
    gstack<TestCase>(next); 
}


Trace::Trace(SourcePoint point, CString cat, CString message) :
  where(point), 
  when(std::time(nullptr)),
  category(cat),
  what(message)
{
}

Trace::Trace(SourcePoint point, CString message) :
  Trace(point, "Trace", message)
{
    report();
}

void Trace::report() const
{
    if (trace_on) print(ostream);
}

void Trace::print(OutStream& out) const
{
    Trace::print(out, where, when, category, what);
}

void Trace::print(OutStream& out, SourcePoint where, std::time_t when, CString category, CString what, CString end)
{
    where.print(out);
    std::tm* tm = std::localtime(&when);
    FILE* file = (FILE*)out;
    fprintf(file, "%04d-%02d-%02d: ", 1900+tm->tm_year, 1+tm->tm_mon, tm->tm_mday);
    fprintf(file, "%02d.%02d.%02d%s: ", tm->tm_hour, tm->tm_min, tm->tm_sec, tm->tm_isdst? "-DST": "");
    fprintf(file, "%s: %s%s", category.cstr(), what.cstr(), end.cstr());
}

Trace::~Trace() 
{ 
    gstack<Trace>(next); 
}

void Trace::turn(bool mode)
{
    trace_on = mode;
}

void Trace::set(OutStream& new_out)
{
    ostream = new_out;
}

bool Trace::trace_on = true;
OutStream Trace::ostream(OutStream::console());

ScopeTrace::ScopeTrace(SourcePoint point, CString message) :
  Trace(point, "Scope", message)
{
    report();
}

void ScopeTrace::print(OutStream& out) const
{
    Trace::print(out, where, when, "Begin", what);
}

ScopeTrace::~ScopeTrace()
{
    if (trace_on) Trace::print(Trace::ostream, where, when, "End", what);
}

void WatchTrace::print(OutStream& out) const
{
    Trace::print(out, where, when, category, what, " = ");
    watched->print(out);
    out.print('\n');
}

void AssertHandler::fail(SourcePoint source, CString message)
{
    auto errout = OutStream::error();
    Trace(source, "Assertion", message).print(errout);
    errout.print("Backtrace begin.\n");
    auto trace = Trace::backtrace();
    kwr::print(trace, errout);
    errout.print("Backtrace end.\n");
    std::abort();
}

GStackSequence<Trace> Trace::backtrace()
{
    return GStackSequence<Trace>();
}

} // kwr namespace

