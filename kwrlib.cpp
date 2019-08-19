
#include "kwrlib.h"
#include <cstdio>

namespace kwr {

bool on = true;
bool off = false;

int compare(const String& leftstr, const String& rightstr)
{
    const char* left  = leftstr.empty()?  "": leftstr.cstr();
    const char* right = rightstr.empty()? "": rightstr.cstr();
    int   maxlen = std::min(leftstr.length(), rightstr.length());
    return std::strncmp(left, right, maxlen);
}

bool operator==(const String& leftstr, const String& rightstr)
{
    return !compare(leftstr, rightstr);
}

const char* BString::emptystr = "";

int BString::compare(const BString& that) const
{
    const char* left  = beginstr? beginstr: "";
    const char* right = that.beginstr? that.beginstr: "";
    int   maxlen = std::min(length(), that.length()); 
    return std::strncmp(left, right, maxlen);
}

int CString::compare(const CString& that) const
{
    const char* left  = str? str : "";
    const char* right = that.str? that.str: "";
    int   maxlen = std::min(length(), that.length()); 
    return std::strncmp(left, right, maxlen);
}

bool operator==(const CString& lhs, const CString& rhs)
{
    return !lhs.compare(rhs);
}

template<> int scan<int>(const String& s)
{
    return atoi(s.cstr());
}

template<> BString scan<BString>(const String& s)
{
    const char* str = s.cstr();
    return BString(str, str+s.length());
}

int OutStream::print(const String& s) { return file? fprintf(file, "%s.*", s.length(), s.cstr()): 0; }
int OutStream::print(CString s)      { return file? fprintf(file, "%s.*", s.length(), s.cstr()): 0; }
int OutStream::print(int i)          { return file? fprintf(file, "%d", i): 0; }
int OutStream::print(unsigned int i) { return file? fprintf(file, "%u", i): 0; }
int OutStream::print(double d)       { return file? fprintf(file, "%f", d): 0; }
int OutStream::print(bool b)         { return file? fprintf(file, "%s", b? "true" : "false"): 0; }
int OutStream::print(char c)         { return file? fprintf(file, "%c", c): 0; }

int OutStream::print(const char* format...)
{
    int num = 0;

    if (file)
    {
        va_list args;
        va_start(args, format);
        num = vfprintf(file, format, args);
        va_end(args);
    }

    return num;
}

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


int print(Sequence& sequence, OutStream& out)
{
    while (sequence.more()) {
        sequence.print(out);
        sequence.next();
    }
}

void SourcePoint::print(OutStream& out) const
{
    out.print("%s:%d: ", filename.cstr(), line);
}


const CString TestCase::name() const 
{
    return "TestCase";
}

void TestCase::print(OutStream& out) const 
{
    out.print("Test case: %s\n", name().cstr()); 
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
    out.print("%04d-%02d-%02d: ", 1900+tm->tm_year, 1+tm->tm_mon, tm->tm_mday);
    out.print("%02d.%02d.%02d%s: ", tm->tm_hour, tm->tm_min, tm->tm_sec, tm->tm_isdst? "-DST": "");
    out.print("%s: %s%s", category.cstr(), what.cstr(), end.cstr());
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

GStackSequence<Trace> Trace::backtrace()
{
    return GStackSequence<Trace>();
}

void Options::getargs(int argc, char* args[], char separator)
{
    int argnum = 1;
    while (argnum < argc) {
        Argument arg { args[argnum], "", "" };
        if (Result r = arg.param.find(separator)) {
            arg.name  = arg.param.take(r);
            arg.value = arg.param.drop(1+r);
        }
        set(arg);
        ++argnum;
    }
}

} // kwr namespace

