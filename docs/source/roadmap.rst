.. Copyright Yanzheng Li. All rights reserved.

Goals and Roadmap
=================

Goals
-----

Since the inception of this project, which is the beginning of March 2014, a number of milestones have been achieved and many components are feature complete at this point. However, there are still many new and exciting features to be designed and implemented ahead to greatly enhance the prowess and versatility of coreVM. There are several major features that are envisioned after the initial version, and they are:

  * :ref:`debugging-and-profiling-api`
  * :ref:`embedder-api`
  * :ref:`extension-api`
  * :ref:`threading-api`
  * :ref:`just-in-time-compilation`
  * :ref:`project-pyta`


.. _debugging-and-profiling-api:

Debugging and Profiling API
^^^^^^^^^^^^^^^^^^^^^^^^^^^

These are a set of APIs and facilities that provide developers powerful capabilities to debug executable code running on coreVM. This includes the typical debugging capabilities such as pause/resume executions, variables examination, call stack unwinding and rewinding, etc. In addition, there will be a set of facilities for instrumenting a diversity of aspects regarding executions, such as process introspections, memory footprint statistics, heap allocation heat map, to name a few.


.. _embedder-api:

Embedder API
^^^^^^^^^^^^

The Embedder API provide developers the capabilities to create virtual execution runtimes in their applications by interacting with a set of APIs and abstract models that define the entities and behaviors of executions of coreVM's internal runtime. For example, a developer who wants to build a little scripting engine in his or her application can employ the Embedder API to accomplish the heavy liftings of executing user provided scripts. All the work for that developer is to parse user provided scripts, and the rest can be left for coreVM to handle.


.. _extension-api:

Extension API
^^^^^^^^^^^^^

The Extension API is at the frontier of the next wave of heavy development with the goal of greatly enhance the capability and versatility of coreVM. The foundation of this is a scripting engine that allows developers to modify the functionalities of existing instructions and to create their own ones to performs custom functions. Developers can use a set of micro-instructions to program their desired functionalities for existing and new instructions.

The next feature is called the Reactive Event Model, which is a model that provides the runtime the capability to react to a set of defined events that are occurring to the execution environment. For example, a developer may want to implement a language feature which passively reacts to certain events happening in the host operating system.

The last feature is to support native plugins, which allows developers to incorporate the execution of native code in conjunction with coreVM's execution, in order to allow more interactions with the operating system.


.. _threading-api:

Threading API
^^^^^^^^^^^^^

Thread management and concurrent executions are essential to most modern languages. Developers want to work with languages that have robust concurrency support and thread management capabilities, in order to take advantage of modern hardware and to achieve higher program throughput.

The Threading API aims to provide language developers a set of easy-to-use interfaces for managing threaded executions.


.. _just-in-time-compilation:

Just-In-Time Compilation
^^^^^^^^^^^^^^^^^^^^^^^^

Just-in-time compilation, or simply JIT, once a novel idea and powerful technique for statically typed languages, are now becoming prevalent for dynamic languages as well. Many modern dynamic languages use a combination of techniques to compile portions of bytecode into native machine code, in order to speed up executions of critical code paths.

Future versions of coreVM will be equipped with a JIT engine that performs the same task by employing a variety of techniques, such as OSR, type inference, live code analysis, to name a few.


.. _project-pyta:

Project Pyta
^^^^^^^^^^^^

Besides the set of powerful features described above, another monumental endeavor in the project is to engineer a different implementation of the Python programming language, codenamed Project Pyta, using an approach that is completely different than that of CPython's by utilizing the capabilities of coreVM. Having the ability to support a real reference language complements the development of the VM as it helps to test the system and provides meaningful feedbacks to improve its stability and performance, as well as to enhance its capabilities and versatilities. Moreover, it also serves as an great example of developing compilers as well as the necessary tools for language developers to utilize and leverage the capabilities of coreVM.


----


Milestones and Roadmap
----------------------

Below is a table of all the past milestones and some of the goals defined in the roadmap in the near future, with their respective completion dates and ETAs:

.. table::

   =============================================  ============================
                      Milestone                       Completion date/ETA
   =============================================  ============================
   Memory allocation schemes (done)                        Mar, 2014
   Dynamic object management (done)                        Mar, 2014
   Native types system (done)                              Jun, 2014
   Instruction execution (done)                            Aug, 2014
   Signal handling (done)                                  Aug, 2014
   GC implementation and integration (done)                Aug, 2014
   Bytecode loading and validation (done)                  Feb, 2015
   Frontend and runtime integration (done)                 Feb, 2015
   Bug fixes and regression tests (done)                   Mar, 2015
   Support basic features in Python (done)                 Mar, 2015
   Python features convergence (done)                      Apr, 2015
   Optimizations (done)                                    May, 2015
   Benchmark infrastructure                                Jun, 2015
   Python features convergence (cont'd)                    Oct, 2015
   Optimizations (cont'd)                                  Feb, 2016
   JIT optimizations                                       Jul, 2016
   Multi-threaded runtime                                  Nov, 2016
   Debugging and Profiling API                             TBD
   Embedder API                                            TBD
   Extension API                                           TBD
   Threading API                                           TBD
   Parallelism support                                     TBD
   Address space layout randomization                      TBD
   =============================================  ============================

Here's an illustration of the envisioned future architecture of coreVM:

  .. figure:: ../../resources/corevm_envisioned_future_architecture_overview.png