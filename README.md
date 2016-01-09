coreVM
======

![logo](/resources/corevm_logo_256x256.png)

*Turn On, Boot Up, Jack In...*

A virtual machine designed to revolutionize the development of modern dynamic
programming languages.

* [Overview] (#overview)
* [Design and Architecture] (#design-and-architecture)
* [Current State and Roadmap] (#current-state-and-roadmap)
* [Focus Areas] (#focues-areas)
* [Moonshots] (#moonshots)
* [Compilation Environments] (#compilation-environments)
* [Dependencies] (#dependencies)
* [License] (#license)
* [Blog Posts] (#blog-posts)
* [Inquiries] (#inquiries)

## Overview
With the proliferation of dynamic programming languages being a dominating trend
in the software industry, many developers have taken the endeavor to invent
their own, in attempts to modernize the designs and implementations of many
legacy systems, and how modern software systems can be developed. However, such
a task would usually become a massive undertaking as the steps involved in
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


## Design and Architecture
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
the most efficient usages.

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
are referred as "dynamic objects", meaning their properties can be dynamically
changed during execution. This provides the flexibility for language
developers to incorporate their custom object abstractions and meta-programming
features.

Each dynamic object can hold one native type handle. For example, an object
of custom integer type in a language may hold a native type handle of type
32-bit signed integer, in order to perform integer arithmetic operations.

### Garbage Collection
The garbage collection layer is responsible for cleaning up unreachable objects
on the heap. The garbage collector is designed to be configured to use one of
several types of garbage collection schemes, with one example being the
"reference counting" scheme.

The garbage collector is tightly coupled with the object heap in terms of their
interactions, but not so much in the design. Each dynamic object is associated
with an instance of "dynamic object manager" that manages the lifespan of its
owner in regards to garbage collection, and it is agnostic of the particular
implementation of the GC scheme used. For example, the object manager type for
the mark-and-sweep GC scheme tracks whether the object is being "marked", while
the one for the reference-count GC scheme keeps tracks of a reference count for
that object.

The current version of the garbage collector has a non-copying,
non-generational, and stop-the-world implementation. Future works to improve and
optimize GC performance are in the roadmap.

### Core Runtime
The core runtime layer is the central component that interacts with the rest of
the components in order to perform bytecode executions.

The runtime is a stack-based machine that manages a call stack while executing
instructions that are in coreVM's bytecode instruction set. The call stack is a
stack of call frames, with each frame containing all the data associated with
one function call. Each frame also contains an "execution stack" for native
types operations. Finally, the runtime is responsible for thread management,
signal handling, and garbage collection during execution.

### Frontend
The frontend component is an intermediate layer that interacts between the core
runtime and the outside world. Its responsibilities include loading
instructions and definitions into the runtime, as well as configurations for
various components of the system.


## Current State and Roadmap
Since the inception of this project, which is the beginning of March 2014, a
great number of milestones have been achieved and many components are near
completion at this point. However, there are still many new and exciting
features to be designed and implemented ahead to greatly enhance the prowess and
versatility of coreVM. There are three major features that are envisioned after
the initial release, and they are the *Debugging and Profiling APIs*,
*Embedder APIs*, and the *Extension APIs*.

### Debugging and Profiling APIs
These are a set of APIs and facilities that provide developers powerful
capabilities to debug executable code running on coreVM. This includes the
typical debugging capabilities such as pause/resume executions, variables
examination, call stack unwinding and rewinding, etc. In addition, there will
be a set of facilities for instrumenting a diversity of aspects regarding
executions, such as process introspections, memory footprint statistics, heap
allocation heat map, to name a few.

### Embedder APIs
The Embedder APIs provide developers the capabilities to create virtual
execution runtimes in their applications by interacting with a set of APIs and
abstract models that define the entities and behaviors of executions of
coreVM's internal runtime. For example, a developer who wants to build a little
scripting engine in his or her application can employ the Embedder APIs to
accomplish the heavy liftings of executing user provided scripts. All the work
for that developer is to parse user provided scripts, and the rest can be
left for coreVM to handle.

### Extension APIs
The Extension APIs are at the frontier of the next wave of heavy development
with the goal of greatly enhance the capability and versatility of coreVM.
The foundation of this is a scripting engine that allows developers to modify
the functionalities of existing instructions and to create their own ones to
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

### Project Pyta
Besides the set of powerful features described above, another monumental
endeavor in the project is to engineer a different implementation of the Python
programming language, codenamed *Project Pyta*, using an approach that is
completely different than that of CPython's by utilizing the capabilities of
coreVM. Having the ability to support a real reference language complements the
development of the VM as it helps to test the system and provides meaningful
feedbacks to improve its stability and performance, as well as to enhance its
capabilities and versatilities. Moreover, it also serves as an great example
of developing compilers as well as the necessary tools for language developers
to utilize and leverage the capabilities of coreVM.

### Milestones and Roadmap

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
  Bytecode loading and validation (done)      |            Feb, 2015
  Frontend and runtime integration (done)     |            Feb, 2015
  Bug fixes and regression tests (done)       |            Mar, 2015
  Support basic features in Python (done)     |            Mar, 2015
  Python features convergence (done)          |            Apr, 2015
  Optimizations (done)                        |            May, 2015
  Benchmark infrastructure                    |            Jun, 2015
  Python features convergence (cont'd)        |            Oct, 2015
  Optimizations (cont'd)                      |            Feb, 2016
  JIT optimizations                           |            Jul, 2016
  Multi-threaded runtime                      |            Nov, 2016
  Debugging and Profiling APIs                |            TBD
  Embedder APIs                               |            TBD
  Extension APIs                              |            TBD
  Parallelism support                         |            TBD
  Address space layout randomization          |            TBD

<br />
Here's an illustration of the envisioned future architecture of coreVM:
![envisioned future architecture](/resources/corevm_envisioned_future_architecture_overview.png "envisioned future architecture overview")


## Focues Areas
This project is consisted of three focus areas that are in-sync with its
development:

### Project "Lithium-Ion"
Focuses on achieving the highest performance possible while striving to maintain
great architecture designs. Performance and efficiency should be factors taken
into consideration at every stage of design and development. Some metrics for
measuring performance and efficiency include per-instruction and overall
execution time, memory usage, bytecode loading time, etc.

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
To be supported.

### Debian
Operating System:<br/>
`Linux 3.8.0-29-generic #42~precise1-Ubuntu SMP x86_64 GNU/Linux (Ubuntu 14.04.1 LTS)`

Compiler:<br/>
`Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)
Target: x86_64-pc-linux-gnu
Thread model: posix`

Python:<br/>
`Python 2.7.6 (default, Mar 22 2014, 22:59:56)`

Make:<br/>
`GNU Make 3.81`
`This program built for x86_64-pc-linux-gnu`

Sphinx:<br/>
`Sphinx v1.1.3`


## Dependencies
The latest build of coreVM depends on the following libraries:

  * [Boost] (http://www.boost.org/) `version 1.55`
  * [Google Test] (https://code.google.com/p/googletest/) `version 1.6.0`
  * [Sneaker] (http://www.libsneaker.org/) `version 0.2.8`
  * [Apache Avro™] (https://avro.apache.org/docs/current/api/cpp/html/) `version 1.7.7`
  * [benchmark] (https://github.com/google/benchmark) `version 0.1.0`


## License
coreVM is licensed under [The MIT License](http://opensource.org/licenses/MIT).


## Blog Posts
Follow the latest news and updates on the [coreVM Official Blog](https://corevm.ghost.io/).


## Inquiries
For technical issues and bugs, please file new issues on GitHub.
For all other inquiries, please e-mail myself at
<a href="mailto:yanzheng819@gmail.com">yanzheng819@gmail.com</a>.
