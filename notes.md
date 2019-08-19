
# Todo

- Assert handler to throw exception
- Change assert handler at runtime
- Switch class for on/off modes.
- OutStream::println to print entire line from CString or sequence
- Automatic sequence of printable elements for printing and string building
- Dynamic string
- Fixed string

# Code Standard

## Naming

- Types: UpperCamelCase
- Functions: lowerCamelCase
- Variables: lowercase, lower_snake_case
- Namespaces: abbreviated lowercase, 3-5 chars.
- #define symbols: kwr_UPPER_SNAKE_CASE
- #define macros: kwr_UpperCamelCase
- Header Guards: KWR_INCLUDED_FILE_H

Container, collection, range, etc variables
: Plural nouns: `items`, `names`, ets.

## Varible Name Abbrevations

- o: object
- i: index 
- j: secondary index
- k: tertiary index
- f: first 
- l: last
- b: begin
- e: end
- s: string
- n: number
- x: horizonal coordinate
- y: vertical coordinate
- z: height coordinate
- r: row number
- c: column number, character, count
- T: type template parameter
- p: pointer

## Indentation & Formatting

1. Spaces, no tabs.
2. No line breaks or wrapping.
3. Use your best judgement for readability.

Element                   Rule
---------------------     ---------------------------------
Indentation               4 spaces (no tabs)
Namespace blocks          no indentation
Member initializer list   2 spaces from ctor definition
Class scope declarations  2 spaces from class declaration
Class members             4 spaces from class declaration
Return types & templates  no indent
Unclosed parens           aligned with open parens
Line continuations        2 spaces

Expressions
: Single spaces around operators, no spacing inside parens.
: Example: `x = (y * (i + 2) )`

Class Declarations
: Inheritance on same line as class name.
: Template declaration on previous line to class name.
: Open curly brace at end of class name declaration.

Function Declarations
: Open curly brace on next line, aligned with return type.
: *Optional*: Extra spacing to align function declarations.

Control Statements
: Single space between keyword and open parens.
: Open braces at end of control statement.
: Example: `for (int i = 0; i < max; ++i) { `


## Types

- Standard Primitives:  int, double, char, bool
- Use *class* for polymorphic, complex, and non-trivial types.
- Use *struct* for trivial and standard-layout types.

# Quality

## Errors

An error is a failure in software quality. 
Software errors are frequently broken down into different categories: logic errors, syntax errors, runtime errors.
Organized method of handling runtime errors. 
Fundamental to our programming discipline.

### Error Handling Workflow

Detection
: Check for error condition. 
: If true, no error detected, continue program flow.
: If false, error is detected, interrupt program flow to handle the error.

Gathering
: Collect error information.

Handling
: Pass info to a handler or return value.

Reporting
: Output error info in user-readable format.

### Error Detection 

Assertions
: Detect programming logic errors. Boolean condition which must *always* be true. A false assertion is a *Defect*.
: - Precondition, require()
: - Postcondition, ensure()
: - Invariant, verify()

Checks
: Detect runtime errors. A false check is a *Fault*.

Tests
: Detect errors from unit testing. A false test is a *Failure*.

### Common Error Conditions

- Null pointer
- Variable out of range
- Invalid object state
- Exception thrown
- File not found
- Bad or corrupt input data
- Out of memory
- Failure to acquire a resource
- Library or API error code

### Error Information

Primary error debugging information.
Collected at the point of [Error Detection].

- when:     date/time stamp of error detection.
- where:    source filename and line number of error.
- what:     description or code snippet.
- category: Defect, Fault, Failure (class type).

Other error information:

- Current time & date.
- Product Version - global structure with release number, major number, minor number, and repository revision number.
- Backtrace at point of error detection. May include scope trace, watched variables, input data, user text.

### Error Handling

Errors interrupt the current execution flow. Default handler for global behavior, can be set at runtime with config. Handlers can be chained into multiple layers. Individual error can override or extend handler chain.

- Abort program
- Throw exception
- Ignore & continue
- Return error code
- Set error state
- Print error info to console
- Write error info to log file
- Pop-up error window
- Send error info to email
- Print/log the current backtrace
- DelegateHandler (set/change handler behavior at runtime)
- DefectHandler (default handler for assertions: print, backtrace, popup, abort)
- FaultHandler  (default handler for checks: print, log, throw)
- FailHandler   (default handler for tests: throw)

## Unit Testing

- TestCase class with method that runs automatically.
- Register each test case to a suite.
- Run all test suites.
- Test assertions to detect failures.
- Report test resuts to stdout in Vim quickfix format.

## Tracing

Tracing tracks and reports program flow events.
Useful for debugging, profiling, instrumentation, auditing. 

### Tracing Workflow

- Event
- Gather Info
- Stack
- Handling
- Reporting
- Backtrace

### Trace Events

- Trace: general trace message
- Scope: scope entry & exit
- Watch: variable name & value 
- Debug: debugging info 
- Error: Assertions & Checks

### Trace Information

- Date-time
- Source location, Vim quickfix format: "filename:line:"
- Event Type
- Message 

### Trace Stack

All trace events pushed onto a stack.
Stack can be dumped to report.

### Trace Handling

- Print to console
- Write to log file
- Do nothing/ignore

### Backtrace

Report the full stack of trace events.
Useful in error handlers to dump context info around the error condition. 

## Logging

Report program runtime status. 

- Config
- Errors
- Tracing
- Status
- Debugging
- Timing

### Log File

- Filename
- Path/location
- Roll-over
- Lifetime

### Log Data Format

- Header (info repeated at top of each file)
- Footer (info repeated at bottom of each file)
- Date-time stamp
- Source location
- Category
- Message

# Resource Management

Manage buffers, files, objects, etc that must be disposed.

## Resource Management Guidelines

- Allocate objects individually
- Only dispose resources from the destructor.
- Every resource has explicit owner
- RAII for all resources.
- Uncopyable, stack-only objects.

## Resource Handle Operations

- size:        Return total capacity of resource.
- empty:       Return true if resource is null/empty.
- release:     Nullify & return raw resource
- swap
- reset(d):    Resource(d).swap( * this)
- move(r):     reset(r.release())
- dispose:     reset(null)
- destructor:  delete resource

### Object Handle Operations

- raw         Raw pointer, unchecked.
- op->        Member access, checked.
- op *        Deref to object reference, checked.

### Memory Handle Operations

- data        Access Mem pointer.
- size        Total space available.
- length      Number of elements, <= size.
- op[i]       Element access, checked.

# Classes

## Class Properties

- immutable:     all methods are const.
- uncopyable:    removed copy & move operations.
- heap-only:     private constructor & factory method.
- stack-only:    removed operators new & delete.
- no-array:      removed operators new[] & delete[].

## Class Types

- object:     Class derived from Object.
- handle
- interface

# Object

## Object Properties

Derived from root base class **Object**.
Uncopyable: copy semantics are deleted.
Polymorphic.
Some may be heap-only, no-array.

## Common Object Operations

Each operation is optional - derived classes only override what they need.

Operation    Purpose                             Default 
-----------  ----------------------------------  ----------------
compare      Compare for searching & sorting     memory addresses
print        Write to text output.               "object"
clone        Create duplicate of object.         null
save         Serialize to a file.                noop
load         Deserialize from file.              noop

validate     Check object invariant.             noop
type         Return type ID information.         ObjectType
class_name   ns-qualified class name string.     "kwr::Object"


# Function Semantics

## Function Attributes

- has/not side-effects
- atomic, no dependencies 

## Parameters

- owner         Give ownership to function.
- optional      May be empty/null and unused.
- span          Pointer and size of memory array.
- output        Value returned through parameter.
- non_null      Painter may not be null (use a reference?)
- non_empty     Container size must be > 0.
- ensures       Value must meet given condition, eg:
  - positive     (value > 0)
  - negative     (value < 0)
  - non-zero     (value != 0)
  - ranged       (n <= value <= x)
  - not_equals   (value != n)
- interface    Any object that implements an interface

## Return values

- optional        Return may contain value or be empty/null.
- unique_ptr      Return ownership to caller.
- required        Caller must receive and assign value.
- status          Return a value or status code.

- moveable objects
- auto-delete ignored resources


# Strings

AString
: "Array String", compile time fixed length string, like `char str[X]` literal char array.
: Null-terminated, mutable, fixed length.

BString
: "Bounded string", string bounded by begin & end pointers.
: Not null-terminated, immutable, fixed length;

CString
: Constant null-terminated string for parameters and literals.
: Null-terminated, immutable, fixed length.

DString
: Dynamically sized string like `new char[X]`.
: Mutable, variable length, resource.

## Generic String Accessors

- cstr()
- length()
- empty()
- operator!()

## String Operations

- split(s)
- hash(s)

## String Modifiers

- op= 
- trim(c)
- concat
- format(values)


# Aggregate Semantics

## Collections

Examples: arrays, containers, lists.

- length           number of elements
- at(i), op[i]     Reference element #i.
- begin()          Start iteration at head.
- end()            Where to stop iteration.
- range(b,e)       Range over elements b through e.

## Sequence

Examples: counters, input streams, randomizers.

- next()            Advance element.
- more()            True if another value is next in sequence.
- empty()           True if sequence is exhausted, !more()
- infinite()        True if sequence never ends. 
- get()             Return current element value.
- max()             Largest possible value. 
- min()             Smallest possible value.
- op->              Access member of element.

## Ranges

### Range Generic Accessors

- length()    Number of elements remaining
- empty()     length() == 0
- first()     Index of first element (usually 0)
- last()      Index of last element (usually length() - 1).

### Range Typed Accessors

- get(i)      Element at index i.
- slice(b,e)  Range from elements b through e.
- find(e)     Index of element e or null.
- find(r)     Index of first element of sub-range r, or null.
- backfind(e)
- backfind(r)
- endsWith(r)
- sequence    Sequence over the range.

- take(n)     slice(first, n-1)
- drop(n)     slice(n, last)
- head()      get(first())
- tail()      drop(1,last)

### Range Modifiers

- put(e, i)      Assign value e to element index i.
- add(e)         Append element e to end.
- add(e, i)      Insert element e at index i.
- cut(i)         Remove element from index i.
- cut(b,e)       Remove elements from index b through e.
- remove(e)      Remove all instances of element e.
- crop(b,e)      Remove elements except index n through m.
- replace(n,m,s)      replace elements in range n-m with sub-list s
- replace(s,t)        replace instances of sub-list s with sub-list t
- replace(e,n)     Replace all instances of element e with value v.

## Aggregate Operations

Functions that operate on ranges and return new ranges or elements.

![Collection Pipeline Operations](https://martinfowler.com/articles/collection-pipeline/#op-catalog)

- clone               New copy of collection
- concat, op+         Combine 2 collections
- diff(s), op-        remove instances of sub s, replace(s,empty)
- filter(f)           return elements that pass function f
- remove(f)           remove elements that pass function f
- map(f)              apply function f to each element
- reduce(f)           accumulate elements with function f(a,c)
- slice, op(n,m)      sub-list elements in range n-m
- sublist(n,l)        sub-list of l elements starting at n


# Functions

- noop(e) {}
- constant(e) -> X
- identity(e) -> e
- copy(e) -> e'
- factory<C>() -> new C
- deleteObject(o)
- deleteArray(a)    
- freeMemory(m)
- throwException<E>(message)
- abort(message)
- println(message)



# Configuration & Arguments

# Source Documenting

# File I/O

# Object Serialization

# Data Table

# String Table

# Fonts

- [Inconsolata](https://fonts.google.com/specimen/Inconsolata?selection.family=Inconsolata), 
  free open-source font from Google under Open Font License.
- [Free UCS Outline Fonts](http://savannah.gnu.org/projects/freefont/), 
  free open-source font from GNU under GPL3.
- [Roboto](https://fonts.google.com/specimen/Roboto?selection.family=Roboto),
  free from Google under Apache 2.
- [Roboto Condensed](https://fonts.google.com/specimen/Roboto+Condensed?selection.family=Roboto+Condensed),
  free from Google under Apache 2.
- [Roboto Slab](https://fonts.google.com/specimen/Roboto+Slab?selection.family=Roboto+Slab),
  free from Google under Apache 2.
- [Liberation Fonts](https://github.com/liberationfonts), 
  free from Fedora under SIL Open Font License.

// vim: wrap linebreak
