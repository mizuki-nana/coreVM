coreVM
======

![logo](/resources/corevm_logo_256x256.png)

A modern virtual machine for dynamic programming languages built with efficiency
and versatility in mind.

## Overview
coreVM is a language-agnostic virtual machine designed to provide a runtime for
dynamic programming languages of various kinds. The design strives to achieve
extremely versatile interfaces and modern runtime functionalities, which
unfetters language developers from the burden of developing custom runtimes and
the tedious compiler integrations involved, therefore ultimately allow them to
focus more on the design and developement of language features.


## Design Overview
coreVM's architecture can be generalized into 6 components:

  * [memory management](#memory-management)
  * [native types](#native-types)
  * [object management](#object-management)
  * [garbage collection](#garbage-collection)
  * [core runtime](#core-runtime)
  * [frontend](#frontend)

Here's an illustration of the high level architecture:
![architecture](https://raw.github.com/airbnb/chronos/master/docs/emr_use_case.png "architecture illustration")

### Memory Management
The most bottom layer in coreVM is the memory management component, which is
responsible for allocating and deallocating dynamic memories used by the
native types pool and the object heap. The memory management component is
equipped with several allocation schemes, such as first-fit, last-fit,
buddy-system, etc, and can be configured to choose one over the others in
different scenarios for the most efficient use. For example, the object heap
uses the buddy-system scheme by default, while the native types pool uses the
first-fit scheme.

### Native Types
One component that interacts directly with the memory management layer is the
native types system, which is an independent system that defines all the native
types in the VM and manages all the permitted operations on them. There are
currently 14 native types defined:

  * 8-bit signed integer
  * 8-bit unsigned integer
  * 16-bit signed integer
  * 16-bit unsigned integer
  * 32-bit signed integer
  * 32-bit unsigned integer
  * 64-bit signed integer
  * 64-bit unsigned integer
  * boolean
  * single-precision floating point decimal
  * double-precision floating point decimal
  * string
  * array
  * map

All these types are abstractly defined as a "native type handle" internally,
which is essentially a handle that points to an entity that is of one of these
14 types. All the arithmetic, logical and bitwise operations that are found in
other programming languages can be performed on native type handles, plus even
complex operations on the string, array and map types.

### Object Management
Another component that directly relates to the memory management layer is the
object heap, which allocates memory from it to store objects. Objects in coreVM
are referred as "dynamic objects", which means that their properties can be
dynamically changed during execution.

Each object has a unique identifier upon creation, and it's used by the object
heap to manage all the objects, which is essentially a map of object IDs to
their corresponding instances.

Each dynamic object can hold one native type handle. For example, an object
of custom integer type in a language may hold a native type handle of type
32-bit signed integer, in order to perform any arithmetic operations on it.

### Garbage Collection
The garbage collection layer is responsible for cleaning up dead objects from
the object heap. The garbage collector is designed to be configured to use one
of several types of garbage collection schemes. Currently only the
"reference counting" scheme is supported.

The garbage collector tightly couples with the object heap in terms of their
interactions, but not so much in the design. Each dynamic object is associated
with an instance of "dynamic object manager" that manages the lifespan of its
owner in regards to garbage collection, and it's agnostic of the particular
implementation of the GC scheme used. For example, the object manager type for
the mark-and-sweep GC scheme tracks the "marked" flag on the object, while the
one for the reference-count GC scheme tracks the reference count of that
particular object.

The current version of the garbage collector has a non-copying, non-generational,
and stop-the-world implementation. Future works are needed to improve the GC
for more optimizations.

### Core Runtime
The core runtime layer is the central component that interacts with the rest of
the layers in order to perform bytecode executions.

The runtime is a stack-based machine that manages a call stack while executing 
instructions that are in coreVM's bytecode instruction set. The call stack is a
stack of call frames, with each frame containing all the data associated with
one function call. Each frame also contains an "execution stack" for native
types operations. Finally, the runtime is responsible for managing threads,
singal handling, and garbage collection during execution.

Currently, the runtime is a single-threaded stack-based machine, a
registered-based version and multi-threading are in the future roadmap.

### Frontend
The frontend component is an intermediate layer that interacts between the core
runtime and the outside world. It's responsible for loading instructions and
definitions into the runtime.

In addition, coreVM defines a custom file format for storing bytecode
instructions and object definitions. The actual data encapsulated in the format
is in JSON, which is easy to interpret semantically and convenient for
validation (currently using JSON schema validation).


## Current State & Roadmap
Since the inception of this project, which is the beginning of March 2014, a lot
of milestones have been achieved and many components are near completion at this
point. However, there are still lots of work ahead to be done before the initial
working build. In addition, many advanced features and optimizations are not to
be implemented until a later time.

Below is a table of all the past milestones and some of the goals defined in the
roadmap in the near future, with their respective completion dates and ETAs:

                Goal                          |       Completion date/ETA
--------------------------------------------- | --------------------------------
  Memory allocation schemes (done)            |            Mar, 2014
  Dynamic object management (done)            |            Mar, 2014
  Native types system (done)                  |            Jun, 2014
  Instruction execution (done)                |            Aug, 2014
  Signal handling (done)                      |            Aug, 2014
  GC implementation and integration (done)    |            Aug, 2014
  Bytecode loading and validation (done)      |            Oct, 2014
  Frontend and core runtime integration       |            Nov, 2014
  Exception handling mechanism                |            Dec, 2014
  Re-entrant execution                        |            Dec, 2014
  Mark-and-sweep garbage collection           |            Jan, 2015
  Generational and concurrent GC              |            Jan, 2015
  JIT optimization                            |            Feb, 2015
  Multi-threaded runtime                      |            Feb, 2015
  Native sandbox                              |            Feb, 2015
  Continuous Integration                      |            Mar, 2015
  Benchmark Infrastructure                    |            Mar, 2015
  Docker Integration                          |            Mar, 2015


## Compilation Environments

### OS X
Operating System:<br/>
`OS X 10.9.5 (13F34)`

Compiler:<br/>
`Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)
Target: x86_64-apple-darwin13.4.0
Thread model: posix`

Make:<br/>
`GNU Make 3.81`
`built for i386-apple-darwin11.3.0`

Sphinx:<br/>
`Sphinx 1.2.2`

### Debian
Operating System:<br/>
`Linux 3.8.0-29-generic #42~precise1-Ubuntu SMP x86_64 GNU/Linux (Ubuntu 14.04.1 LTS)`

Compiler:<br/>
`Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)
Target: x86_64-pc-linux-gnu
Thread model: posix`

Make:<br/>
`GNU Make 3.81`
`This program built for x86_64-pc-linux-gnu`

Sphinx:<br/>
`Sphinx v1.1.3`


## Dependencies
The latest build of coreVM depends on the following libraries:

  * [Boost] (http://www.boost.org/) `version 1.55`
  * [Google Test] (https://code.google.com/p/googletest/) `version 1.6.0`
  * [Sneaker] (http://www.libsneaker.org/) `version 0.2.7`


## License
coreVM is licensed under The MIT License: http://opensource.org/licenses/MIT
