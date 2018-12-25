
# Tools

## Source Control (git)

## Environment (MSYS2)

## Compiler

## Build

## Docs


# Code Standard

## Naming

- Types (class, struct, etc): UpperCamelCase
- Methods & functions: lowerCamelCase
- Variables & objects: lowercase, lower_snake_case
- Namespaces: abbreviated lowercase, 3-5 chars.
- #define symbols: ns_UPPER_SNAKE_CASE (ns = namespace)
- #define macros: ns_UpperCamelCase (ns = namespace)

### Specific Naming Rules

Header Include Guards
: INCLUDE_NS_FILE_H
: - NS = namespace (eg "KWR").
: - FILE = Filename (eg KWRLIB).

## Indentation & Formatting

- Indentation:               4 spaces (shiftwidth), no tabs
- Namespace blocks:          no indent
- Base class & ctor inits:   2 spaces after ctor definition
- Scope declarations:        2 spaces after class {
- Class members:             4 spaces from margin
- Return types & templates:  no indent
- Unclosed parens:           align with first character
- Line continuations:        2 spaces
- No line breaks or wrapping.
- Single space after control keywords: eg. "for (...)"
- Open brace end of control statements.
- Single spaces around operators (x = 1 + 2).

## Error Handling & Exceptions Safety

- Ignore bad_alloc errors, let program crash.
- Avoid exceptions.
- Follow exception safe code anyway.
- Use Copy & swap for assignment ops.

## Standard Library

- swap, move, exchange
- pair, tuple, tie
- unique_ptr?
- type traits
- initializer_list
- function



# Object Semantics

## Content Operations

- swap
- move
- copy
- compare
- print 
- string

## Resource Operations

Examples: Arrays, strings, buffers.

- dispose     Free resource & become null.
- release     Nullify and return raw resource.
- data        Access raw memory resource.
- get         Access raw object resource.
- size        Total space available.
- take        Take ownership from another object.

## Collection Operations

- count       number of elements
- at(n)       Reference element #n.
- begin()     Start iteration at head.
- end()       Where to stop iteration.
- range(a,b)  Range over elements a through b.

## Sequence Operations

Examples: counters, input streams

- get()       Return current element value.
- next(n)     Move to next n'th element/value.
+ empty()     True if sequence is exhausted.
- infinite    Unlimited size?

## Range Operations

- get(n)      Return element #n value.
- put(e, n)   Assign element #n value e.
- length()    Number of elements remaining
- empty()     True if range is exhausted.
- first()     Index of first element (usually 0).
- last()      Index of last element (length - 1).
- slice(n,m)  Return range from elements n to m.
- tail()      Remaining elements after head, slice(1,last)
+ take(n)     slice(first, n-1)
+ drop(n)     slice(n, last)

### Classes

Count
: Range of numbers between *min* and *max*.

MemRange
: Range of objects in contiguous memory array.
: Used for traversing arrays.

## Consumers/Modifiers

- insert(e, n)   Insert element e at position n.
- append(e)      Add element e to end.
- remove(n,m)    Remove elements from positions n through m.
- crop(n,m)      Remove elements outside positions n through m.

## Pipelines

![Collection Pipeline Operations](https://martinfowler.com/articles/collection-pipeline/#op-catalog)

- clone:        new copy of collection
- concat/op+:        combine 2 collections
- diff(s)/op-:        remove instances of sub s, replace(s,empty)
- filter(f):           return elements that pass function f
- remove(f):        remove elements that pass function f
- map(f):          apply function f to each element
- reduce(f):       accumulate elements with function f(a,c)
- slice/op(n,m):    sub-list elements in range n-m
- substr(n,l):    sub-list of l elements starting at n
- replace(n,m,s):    replace elements in range n-m with sub-list s
- replace(s,t):       replace instances of sub-list s with sub-list t

## Type Members

- Element     type of contained element
- Returns     function return type
- Base        base class type
- Range       range type

# Function Semantics

## Parameters

- unique_ptr    Give ownership to function.
- optional      May be empty/null and unused.
- span          Pointer and size of memory array.

- output     Value returned through parameter.
- non_null   Painter may not be null (use a reference?)
- non_empty  Container size must be > 0.
- ensures    Value must meet given condition, eg:
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
- Error code

- moveable objects
- auto-delete ignored resources



# Type info

# Quality

## Testing

- Test case: class with test run method.
- Register test case with test suite.
- Run all test cases.
- Report each test failure.
- Test Assertions.

## Tracing

Details

- Source location in Vim quickfix format: "filename:line:"
- Category
- Message & Details

Categories

- TracePoint: source location & trace info.
- Trace: general trace message.
- ScopeTrace:  scope entry/exit tracing.
- Watch: variable name & value at point in time
- Backtrace: report of recent trace points
- Category: type of trace/assertion (class, name, config)
- Failure:  assertion failure exception.
- Assert:   assertion tracing.

## Assertions

- Fail on false condition
    - True/false
    - Comparison
    - Is/not null
    - Expected exception
- Report details to stream
    - SourceLine
    - Category
    - Level (always 1)
    - Messaage (failed condition)
    - Backtrace
- Interrupt execution
    - Throw
    - Abort
    - Nothing/Ignore

- Categories
    - Test
    - Precondition (Require)
    - Postcondition (Ensure)
    - Invariant
    - Check
    - Fail
- Reporting Stream 
    - stdout
    - stderr
    - file
    - UI
- Config (per Category)
    - Interruption mode
    - Reporting stream

## Memory

## Logging

## Errors

## Return values

## Type Info

# Strings

- LitString  (literal string, immutable & fixed size)
- FixString  (fixed size, mutable)
- DynString  (dynamically sized, mutable)

## Operations

- op=:           replace with new string value
- length:          # characters
- op[i]:           char at position i
- empty/op!:           true if length == 0
- equals, op==:    compare for equality
- compare(s):         compare strings
- find(c):             position of char c
- find(s):             position of substr s

- clone():
- slice/op(n,m):    substr range
- concat/op+.
- diff(s)/op-.
- format(values):   format string with given values.
- split(s)

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


# Range Operations

# Configuration & Arguments

# Source Documenting

# File I/O

# Object Serialization

# Data Table

# String Table


# Logging

## Category

- Error
- Warning
- Debug
- Monitor
- Watch
- Checkpoint
- Info
- Custom?
- Sub-categories?
  - Error.NotFound
  - Info.Startup

## Frequency

- Level of detail & volume
- Higher = larger number of events
- Inc/decrement
- Value = approx # of events / time

For example:

0. Not used
1. App Start/init & Stop/de-init
+ Main Loop
+ New transaction/input/operation
..
+ Detailed loop

## ID Label

Arbitrary identifier for log events.

## Event

Individual log message.

- Date-Time
- Category
- Volume
- Channel
- Label
- Message
- Source line

## Config

- Max Level (Volume)
- Category on/off
- Event on/off
- ID Label on/off
- Channel Output, on/off
- Conditional: only report events that pass criteria

## Channel

Maps to a Log Output

- Name
- Output

## Output

- File
- Console
- Debugger
- Window
- Null

## File

- Filename
- Path/location
- Roll-over
- Lifetime
- Header (events repeated at start of every new file)
- Footer (events repeated at end of every closing file)

<!-- vim:set ft=markdown: -->
