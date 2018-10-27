
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

- discard     Free resource & become null.
- disown      Nullify and return raw resource.
- data        Access raw resource.
- size        Total space available

## Collection Operations

- count       number of elements
- at(n)       Reference element #n.
- begin()     Start iteration at head.
- end()       Where to stop iteration.
- range(a,b)  Range over elements a through b.

## Sequence Operations

- get()       Return current element value.
- next(n)     Move to next n'th element/value.
- done()      No more elements?
- infinite    Unlimited size?

## Range Operations

- get(n)      Return element #n value.
- put(e, n)   Assign element #n value e.
- length()    Number of elements remaining
- last()      Index of last element (length - 1).
- slice(n,m)  Return range from elements n to m.
- tail        Remaining elements after head, slice(1,last)

## Editor Operations

- add(e)      Add element to end.

## Type Members

- Element     type of contained element
- Returns     function return type
- Base        base class type
- Range       range type


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

## Basic Operations

- cstr()
- length()
- copy(), op=
- equals ==
- compare() 

# Arrays

## DataSize

Template struct of pointer & size of data.

## Basic Operations

- data()
- size()

# Sequences

# Ranges

## Range Spec

Typedefs

+ Type    element type

Constants

+ front = 0   position of first element
+ back = -1   position of last element

Traversal

+ pop(n=front)    Remove n+1 elements from front
+ pop(-n)         Remove n elements from end (n is negative)
+ crop(f,e)       Shrink range to elements f through e
+ take(count)     crop(front, count-1)
+ drop(count)     crop(count, back)

Length

+ more()        true if there is a next element
+ empty()       true if range is exhausted
+ finite()      true if size is finite
+ size()        number of remaining elements
 
Access

+ get(n=front)      return n'th element
+ put(e, n=front)   set current element to e
+ append(e)         adds new element e to end 

## Range Classes

NumRange
: Range of numbers between *min* and *max*.

MemRange
: Range of objects in contiguous memory array.
: Used for traversing arrays and strings.

## Range Operations

![Collection Pipeline Operations](https://martinfowler.com/articles/collection-pipeline/#op-catalog)

# Pipeline

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
