.. Copyright Yanzheng Li. All rights reserved.

Design and Architecture
=======================

coreVM's architecture can be generalized into 6 components:

  * :ref:`memory-management`
  * :ref:`native-types-system`
  * :ref:`object-management`
  * :ref:`garbage-collection`
  * :ref:`core-runtime`
  * :ref:`front-end`

Here's an illustration of the high level architecture:

.. figure:: ../../resources/corevm_architecture_overview.png


.. _memory-management:

Memory Management
-----------------

The most bottom layer in coreVM is the memory management component. The core of this layer comprises an allocator interface that can be be customized to use a variety of allocation schemes, such as first-fit, last-fit, buddy-system, block allocation, etc. The allocator interface provides the functionality to allocate and deallocate memories for other components in the system, including the object heap and the native types pool.


.. _native-types-system:

Native Types System
-------------------

The native types system is an independent component that defines all the native types in the system, as well the operators and interfaces that are needed to interact with them. There are currently 14 native types defined:

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

The purpose of the native types system is to provide a set of interfaces for languages to deal with arithmetic, logical, bitwise, complex operations, as well as conversions on the most commonly used types without having to implement their own.


.. _object-management:

Object Management
-----------------

Another component that directly relates to the memory management layer is the object management layer. At its core, it is consisted of an object heap which allocates memory from it to store objects. Objects in coreVM are referred as "dynamic objects", meaning their properties can be dynamically changed during execution. This provides the flexibility for language developers to incorporate their custom object abstractions and meta-programming features.

Dynamic object can also reference native type handle. For example, an object of custom integer type in a language may hold a native type handle of type 32-bit signed integer, in order to perform integer arithmetic operations.


.. _garbage-collection:

Garbage Collection
------------------

The garbage collection layer is responsible for cleaning up unreachable objects on the heap. The garbage collector is designed to be configured to use one of several types of garbage collection schemes, with one example being the "reference counting" scheme.

The garbage collector is tightly coupled with the object heap in terms of their interactions, but not so much in the design. Each dynamic object is associated with an instance of "dynamic object manager" that manages the lifespan of its owner in regards to garbage collection, and it is agnostic of the particular implementation of the GC scheme used. For example, the object manager type for the mark-and-sweep GC scheme tracks whether the object is being "marked", while the one for the reference-count GC scheme primarily keeps tracks of a reference count for that object.

The current version of the garbage collector has a non-copying, non-generational, and stop-the-world implementation. Future works to improve and optimize GC performance are in the roadmap.


.. _core-runtime:

Core Runtime
------------

The core runtime layer is the central component that interacts with the rest of the components in order to execute bytecode.

The core of the runtime is a process abstraction that manages the executions of bytecode. The process consists of a stack-based machine that manages a call stack while executing instructions that are in coreVM's bytecode instruction set. The call stack is a stack of call frames, with each frame containing all the data associated with a function call, and each frame also contains an "execution stack" for native types operations. In addition, the process contains a dynamic object heap for managing all the dynamic objects in the system, as well as a native types pool for managing all the native type instances. Finally, the runtime is responsible for thread management, signal handling, garbage collection, and many other types of operations during execution.


.. _front-end:

Front-end
---------

The front-end component is an intermediate layer that interacts between the core runtime and the outside world. Its responsibilities include loading instructions and definitions into the runtime, as well as configurations for various components of the system.