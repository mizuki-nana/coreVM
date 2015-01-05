coreVM
======

![logo](/resources/corevm_logo_256x256.png)

*Turn On, Boot Up, Jack In...*

A virtual machine designed to revolutionize the development of modern dynamic
programming languages.


## Overview
With the proliferation of dynamic programming languages being a dominating trend
in the software industry, many developers have taken the endeavor to invent
their own, in attempts to modernize the designs and implementations of many
legacy systems, and how modern software systems can be built. However, such a
task would usually become a massive undertaking as the steps involved in
developing a custom runtime for a dynamic programming language tend to be
extremely complex, error prone and resource consuming, thus ending up being a
very intimidating part in the process. Although there are candidates of generic
runtimes for dynamic languages that are available, many of them lack the
robustness, efficiency and versatility that can meet the language requirements.

coreVM is a language-agnostic virtual machine designed to provide a runtime for
dynamic programming languages. The design strives to achieve extremely versatile
interfaces and modern runtime functionalities, which unfetters language
developers from the burden of developing custom runtimes and the tedious
compiler integrations involved. Instead of re-inventing the wheel, developers
can focus on the design and development of language features.


## Design Overview
coreVM's architecture can be generalized into 6 components:

  * [memory management](#memory-management)
  * [native types](#native-types)
  * [object management](#object-management)
  * [garbage collection](#garbage-collection)
  * [core runtime](#core-runtime)
  * [frontend](#frontend)

Here's an illustration of the high level architecture:
![architecture](/resources/corevm_architecture_overview.png "architecture overview")

### Memory Management
The most bottom layer in coreVM is the memory management component, which is
responsible for allocating and deallocating memories used by the object heap
and the native types pool. The memory management component is equipped with
several allocation schemes, such as first-fit, last-fit, buddy-system, etc,
and can be configured to choose one over the others in different scenarios for
the most efficient usages. For example, the object heap uses the buddy-system
scheme by default, while the native types pool uses the first-fit scheme.

### Native Types
The native types system is an independent system that defines all the native
types in the VM and manages all permitted operations on them. There are
currently 14 native types defined:

  * 8-bit signed integer
  * 8-bit unsigned integer
  * 16-bit signed integer
  * 16-bit unsigned integer
  * 32-bit signed integer
  * 32-bit unsigned integer
  * 64-bit signed integer
  * 64-bit unsigned integer
  * single-precision floating point decimal
  * double-precision floating point decimal
  * boolean
  * string
  * array
  * map

The purpose of the native types system is to provide a set of interfaces for
languages to deal with arithmetic, logical, bitwise, and complex operations
on the most commonly used types without having to implement their own.

All these types are abstractly defined as a "native type handle" internally,
which is essentially a handle that points to an entity that is of one of these
14 types. Types can be converted from one to another, where appropriate.

### Object Management
Another component that directly relates to the memory management layer is the
object heap, which allocates memory from it to store objects. Objects in coreVM
are referred as "dynamic objects", meaning that their properties can be
dynamically changed during execution. This provides the flexibility for language
developers to incorporate their custom object abstractions and meta-programming
features.

Each dynamic object can hold one native type handle. For example, an object
of custom integer type in a language may hold a native type handle of type
32-bit signed integer, in order to perform any arithmetic operations on it.

### Garbage Collection
The garbage collection layer is responsible for cleaning up unreachable objects
on the heap. The garbage collector is designed to be configured to use one of
several types of garbage collection schemes. Currently only the
"reference counting" scheme is supported.

The garbage collector is tightly coupled with the object heap in terms of their
interactions, but not so much in the design. Each dynamic object is associated
with an instance of "dynamic object manager" that manages the lifespan of its
owner in regards to garbage collection, and it is agnostic of the particular
implementation of the GC scheme used. For example, the object manager type for
the mark-and-sweep GC scheme tracks whether the object is being "marked", while
the one for the reference-count GC scheme keeps tracks of a reference count for
that object.

The current version of the garbage collector has a non-copying,
non-generational, and stop-the-world implementation. Future works are needed to
improve the GC with more optimizations.

### Core Runtime
The core runtime layer is the central component that interacts with the rest of
the components in order to perform bytecode executions.

The runtime is a stack-based machine that manages a call stack while executing
instructions that are in coreVM's bytecode instruction set. The call stack is a
stack of call frames, with each frame containing all the data associated with
one function call. Each frame also contains an "execution stack" for native
types operations. Finally, the runtime is responsible for thread management,
signal handling, and garbage collection during execution.

Currently, the runtime is a single-threaded stack-based machine, a
registered-based version and multi-threading are in the future roadmap.

### Frontend
The frontend component is an intermediate layer that interacts between the core
runtime and the outside world. It's responsible for loading instructions and
definitions into the runtime.

In addition, coreVM defines a custom file format for storing bytecode
instructions and object definitions. The actual data encapsulated in the format
is in JSON, which is not only easy to interpret semantically and convenient for
validation (currently using JSON schema validation), but also has very minimal
learning curve for many developers.


## Current State & Roadmap
Since the inception of this project, which is the beginning of March 2014, a lot
of milestones have been achieved and many components are near completion at this
point. However, there are still many new and exciting features to be designed
and implemented ahead to greatly enhance the prowess and versatility of coreVM.
There are three major features that are envisioned after the initial release,
and they are the Debugging and Profiling APIs, Embedder APIs, and the
Extension APIs.

### Debugging and Profiling APIs
These are a set of APIs and facilities that provide developers powerful
capabilities to debug executable code running on coreVM. This includes the
typical debugging capabilities such as execution pausing, variables examination,
call stack unwinding and rewinding, etc. In addition, there will be a set of
facilities for instrumenting a diversity of aspects regarding executions,
such as process introspections, memory footprint statistics, heap allocation
heat map, to name a few.

### Embedder APIs
The Embedder APIs provide developers the capability to create virtual execution
runtimes in their native applications by interacting with a set of APIs and
abstracted models that define the entities and behaviors of executions of
coreVM's internal runtime. For example, a developer who wants to build a little
scripting engine in his or her application can employ the Embedder APIs to
accomplish the heavy liftings of executing user provided scripts. All the work
for that developer is to parse user provided scripts, and the rest can be
left for coreVM to handle.

### Extension APIs
The Extension APIs are at the frontier of the next wave of heavy development
with the goal of greatly enhance the capability and versatility of coreVM.
The foundation of this is a scripting engine that allows developers to modify
the functionalities of existing instructions or to create their own ones to
performs custom functions. Developers can use a set of micro-instructions
to program their desired functionalities for existing and new instructions.

The next feature is called the *Reactive Event Model*, which is a model that
provides the runtime the capability to react to a set of defined events that are
occurring to the execution environment. For example, a developer may want to
implement a language feature which passively reacts to certain events happening
in the host operating system.

The last feature is to support native plugins, which allows developers to
incorporate the execution of native code in conjunction with coreVM's execution,
in order to allow more interactions with the operating system.


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
  Exception handling mechanism                |            Feb, 2015
  Re-entrant execution                        |            Feb, 2015
  Mark-and-sweep garbage collection           |            Mar, 2015
  Generational and concurrent GC              |            Mar, 2015
  Benchmark infrastructure                    |            Apr, 2015
  Docker integration                          |            Apr, 2015
  Continuous integration                      |            Apr, 2015
  Initial working build *                     |            Apr, 2015
  Multi-threaded runtime                      |            May, 2015
  JIT optimizations                           |            Jun, 2015
  Debugging and Profiling APIs                |            Jul, 2015
  Embedder APIs                               |            Sep, 2015
  Extension APIs                              |            Nov, 2015
  Parallelism support                         |            Jan, 2016
  Address space layout randomization          |            Mar, 2016

<br />
Here's an illustration of the envisioned future architecture of coreVM:
![envisioned future architecture](/resources/corevm_envisioned_future_architecture_overview.png "envisioned future architecture overview")


## Focues Areas
This project is consisted of three focus areas that are in-sync with its
development:

### Project "Lithum-Ion"
Focuses on achieving the highest performance possible while striving to maintain
great architecture designs. Performance and efficiency should be factors taken
consideration at every stage of design and development. Some metrics for
measuring performance and efficiency include per-instruction and overall
execution time, memory usage, byte-code loading time, etc.

### Project "Swiss Army Knife"
Focuses on achieving versatile capabilities and flexibilities to meet the needs
of language developers, without compromising on performances. This initiative
involves two parts. One is to deliver developers the capabilities through a set
of APIs. The other one involves supporting the former, by striving for great
architectures internally to keep components flexible and extensible.

### Project "Air Force One"
Focuses on improving the security and robustness of the system, without
sacrificing the previous two points. The system should be designed and
implemented to be able to handle a wide range of error conditions gracefully.
In addition, the system needs to be able to protect and sandbox the runtime from
external malicious exploits. Last, establish a robust infrastructure for users
to report vulnerabilities, as well as being informed of these vulnerabilities
and the corresponding patches. Finally, adjust the development cycle of the
project to accomodate iterating on patching security vulnerabilities.


## Moonshots
While the ultimate goal of this project is to facilitate the design and
development of modern dynamic programming languages for language developers,
I personally have two ideas for the use of this project that I have envisioned
for the foreseeable future.

One is to use coreVM as the underlying runtime for the
[HEX programming language](http://www.github.com/yanzhengli/hex) that I
worked on a while ago. This was actually one of the initial goals of the project
before its inception.

The second idea is to have a port of Python that runs on coreVM, as I've always
wanted to join the Python family that's comprised of many flavors of the
language. It will be interesting to see the differences in stability,
performance, and many other aspects across the major flavors of the language,
such as [CPython](https://www.python.org/), [PyPy](http://pypy.org/),
[Jython](http://www.jython.org/), [IronPython](http://ironpython.net/),
and [Pyston](https://github.com/dropbox/pyston). Hopefully I will come up with
a good name of my variant by then.


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


## Inquiries
For technical issues and bugs, please file new issues on GitHub.
For all other inquiries, please e-mail myself at
<a href="mailto:yli@corevm.org">yli@corevm.org</a>.

In addition, a mailing list from @corevm.org will be made available in the near
future.
