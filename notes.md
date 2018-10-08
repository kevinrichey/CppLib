
Outline
====================

- Code Style
- Strings
- Quality 
    - Testing
    - Debugging
    - Tracing
    - Logging
- Collections 
    - Arrays
    - Lists
- Ranges
- Collection Pipelines
- Configuration & Arguments
- Source Documenting

Code Style
====================

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
- Unclosed parans:           align with first character
- Line continuations:        2 spaces
- No line breaks.
- No line wrapping.
- Single space after control keywords: eg. "for (...)"
- Open brace on next line.
- Single spaces around operators (x = 1 + 2).

## Resources

- RAII: All resources (memory, file, sockets, etc) managed & disposed by a destructor.
- Owner: object responsible for releasing/deleting a resource.
- Copy: deep copy object contents
- Move: transfer resource to another object
- Swap: exchange resources between two objects
- Release: return resource. 

## Light Objects

- Small objects easy/cheap to copy, up to 32 bytes.
- Use default copy semantics.
- Move semantics unnecessary (deleted).
- Pass & return by value.

## Heavy Objects

- Large objects too expensive to copy.
- Heap allocation.
- Not copyable (Copy semantics disabled).
- Move semantics optional.
- Pass by reference/pointer to functions.
- Return by pointer (created from new) or move semantics.
- Eg. containers.

## Exceptions & Safety

- Use Copy & swap for assignment ops.

Strings
==============================

- Types
    - Literal
    - Automatic
    - Dynamic

Quality
==============================

## Primitives

- TracePoint: source location & trace info.
- Watch: variable name & value at point in time
- Level: volume of reporting (0. none/off, 1. low, 99. high)
- Backtrace: report of recent trace points
- Category: type of trace/assertion (class, name, config)

## Tracing

- Details
    - Source location in quickfix format: "filename:line:"
    - Category
    - Volume Level
    - Message & Details
- Categories
    - App Start & Stop
    - Scope entry & exit (Level +/-)
    - Checkpoint
    - Watch variables

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

## Testing

- Test case: class with test run method.
- Register test case with test suite.
- Run all test cases.
- Report each test failure.
- Test Assertions.

## Config

- Volume level
- Categories on/off
- Log file name, path, rotation


Ranges
=====================

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


Logging
==============================

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
