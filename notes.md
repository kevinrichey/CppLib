
# Todo

- Assert handler to throw exception
- Change assert handler at runtime
- Should Assert be a Trace? 
- Switch class for on/off modes.
- OutStream::println to print entire line from CString or sequence
- Automatic sequence of printable elements for printing and string building
- Dynamic string
- Fixed string
- Span for pointer/length pair
- Array

# Code Standard

## Naming

- Types: UpperCamelCase
- Functions: lowerCamelCase
- Variables: lowercase, lower_snake_case
- Namespaces: abbreviated lowercase, 3-5 chars.
- #define symbols: ns_UPPER_SNAKE_CASE
- #define macros: ns_UpperCamelCase
- Header Include Guards: KWR_HEADER_NS_FILE_H

ns = namespace

## Abbrevation Conventions

- o: object
- i: index 
- j: secondary index
- k: tertiary index
- f: first 
- l: last
- b: begin
- e: end
- r: range
- s: string
- n: number
- x: horizonal coordinate
- y: vertical coordinate
- z: height coordinate
- r: row number
- c: column number, character
- T: type template parameter
- p: pointer

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

- Use exceptions judiciously. 
- Write exception-safe code.
- Copy-swap assignment ops.

## Standard Library

Prefer C++11.

- swap, move, exchange
- pair, tuple, tie
- type traits
- initializer_list
- function
- avoid containers & iterators

## Resource Management

- Allocate objects individually
- Primitive/trivial types only for arrays
- Every resource has explicit owner
- Prefer immutable objects

# Object Semantics

## Object Attributes

- Immutable
- non-copyable
- non-moveable 
- trivial 
- polymorphic
- value type
- array allocation (yes/no)
- reference handling
- composite: contains other object members
- primitive: built-in types
- consumable: using, accessing, or processing the object will consume it value
- Temporary: lifetime limited within code execution scope.
- Eternal: lifetime persists during entire program execution.

## Polymorphic Types

- class declaration
- Non-copyable (delete copy-ctor, copy-assign).
- Non-moveable (delete move-ctor, move-assign)
- No array allocation (delete new[] and delete[])
- Reference/pointer handling only.
- virtual destructor

## Simple Types

- struct declaration
- primitive and simple members only
- no virtual methods
- array allocation allowed
- Prefer is_trivial 

## Common Object Operations

- compare
- clone
- print 
- save
- load

## Mem

Pointer/size pair to memory.

- data      raw pointer to memory
- size      size of memory

## Resource

Buffers, files, objects.

Copy ctr & op deleted to avoid implicit copy by value

- dispose     Delete & nullify
- release     Nullify & return raw resource
- reset       Delete & assign new raw resource
- swap        Exchange resource with another
- move        Dispose & take resource
- Destructor  Dispose

### Handle Operations

- raw         Access raw pointer
- op->        Access object member, check null

### Memory Operations

- data        Access Mem pointer.
- size        Total space available.
- length      Number of elements, <= size.
- op[i]       Access element, check null & bounds


# Function Semantics

## Function Attributes

- has/not side-effects
- atomic, no dependencies 

## Parameters

- owner         Give ownership to function.
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


# Strings

AString
: Compile time fixed length string, like `char str[X]` literal char array.
: Mutable, fixed length.

CString
: Constant/Static string for parameters and compile-time literals like `"hello world"`.
: Immutable, fixed length.

DString
: Dynamically sized string like `new char[X]`.
: Mutable, variable length, resource.

## Generic String Accessors

- length()
- size()
- cstr()
- empty()
- operator!()

## String Operations

- split(s)
- hash

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

## Sequences

Examples: counters, input streams, randomizers.

- get(), op *       Return current element value.
- op->              Access member of element.
- next(), op++      Advance element.
- empty()           True if sequence is exhausted.
- op bool           True if sequence continues.
- infinite          True if sequence never ends. 

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


# Quality

## Unit Testing

- Test case class with run method.
- Register test case with test suite.
- Run all test cases.
- Report each test failure.
- Test Assertions.

## Code Tracing

- Source location in Vim quickfix format: "filename:line:"
- Date-time
- Category
- Message 
- Backtrace recent trace points

## Trace Categories

- Trace:   general trace message
- Scope:   scope entry & exit
- Watch:   variable name & value 
- Error:   runtime error
- Warn:    runtime warning
- Debug:   debugging info (disabled in release build)
- Assert:  logic error
- Test:    assertion for unit testing

## Assertions

- Fail on false condition
    - True/false
    - Comparison
    - Is/not null
    - Expected exception
- Report details 
    - [SourceLine]
    - Category
    - Level
    - Message 
    - Failed condition
    - Backtrace
- Interrupt execution
    - Throw
    - Abort
    - Nothing/Ignore
- Categories
    - Unit Test
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

## Trace File

- Filename
- Path/location
- Roll-over
- Lifetime
- Header (events repeated at start of every new file)
- Footer (events repeated at end of every closing file)


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



