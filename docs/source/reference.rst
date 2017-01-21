.. Copyright Yanzheng Li. All rights reserved.

================
Reference Manual
================

This reference manual contains useful information for developers who wish to
make use of coreVM's functionalities.

************************
Build and Install coreVM
************************

Steps for building coreVM
=========================

The project strives the make the build and installation process as
straightforward as possible. Currently, the build system requires
`GNU Make <https://www.gnu.org/software/make/>`_ and
`CMake <https://cmake.org/>`_.

1. Build and install all dependencies

  `git submodule update --init && make -C libs full=1`

  coreVM currently has the following dependencies:

  * `Boost <http://www.boost.org/>`_ version 1.55
  * `Google Test <https://code.google.com/p/googletest/>`_ version 1.7.0
  * `Sneaker <http://www.libsneaker.org/>`_ version 0.3.2 beta (0820a28)
  * `Apache Avro <https://avro.apache.org/docs/current/api/cpp/html/>`_ version 1.7.7
  * `LLVM <http://www.llvm.org/>`_ version 3.4
  * `Flex <https://github.com/westes/flex>`_ version 2.6.2
  * `Bison <https://www.gnu.org/software/bison/>`_ version 3.0.4
  * `benchmark <https://github.com/google/benchmark>`_ version 1.0.0

2. Build main binaries and run tests

  `make`

  This builds the following binaries:

  ===================  ==========================================
        Binary                        Description
  ===================  ==========================================
    libcorevm.a         coreVM static library (release version)
    libcorevm_dbg.a     coreVM static library (debug version)
    coreVM              coreVM client (release version)
  ===================  ==========================================

3. Optionally, build all targets, including benchmarks, sanity tests, and
   dynamic analysis tests

  `make full=2`

4. Install coreVM library binaries and headers

  `make install`



Build Environments
==================

This sections describes the build environments used for development and testing.
Please refer to this as a reference if you are unsure whether your system meets
the requirements for building coreVM.

Development 1
-------------

**Operating System:**
`Linux 3.8.0-29-generic #42~precise1-Ubuntu SMP x86_64 GNU/Linux (Ubuntu 14.04.4 LTS)`

**Compiler:**
`Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4) Target: x86_64-pc-linux-gnu Thread model: posix`

**Python:**
`Python 2.7.6 (default, Jun 22 2015, 17:58:13)`

**CMake:**
`cmake version 2.8.12.2`

**GNU Make:**
`GNU Make 3.81 This program built for x86_64-pc-linux-gnu`

**Valgrind:**
`valgrind-3.10.1`

**Sphinx:**
`Sphinx v1.2.2`

----

Development 2
-------------

**Operating System:**
`OS X 10.11.6 (15G1108) Darwin 15.6.0 root:xnu-3248.60.11~2/RELEASE_X86_64 x86_64`

**Compiler:**
`Apple LLVM version 7.3.0 (clang-703.0.31) Target: x86_64-apple-darwin15.6.0 Thread model: posix`

**Xcode:**
`Version 7.3.1 (7D1014)`

**Python:**
`Python 2.7.10`

**CMake:**
`cmake version 3.4.3`

**GNU Make:**
`GNU Make 3.81 This program built for i386-apple-darwin11.3.0`

**Flex:**
`flex 2.6.2`

**Bison:**
`bison (GNU Bison) 3.0.4` `Written by Robert Corbett and Richard Stallman`

**Sphinx:**
`Sphinx v1.3.5`

----

Continuous Integration 1
------------------------

**Operating System:**
`Linux testing-gce-6d2a2bc6-283a-470a-90b9-9443d55ea351 3.13.0-92-generic #139~precise1-Ubuntu SMP Wed Jun 29 21:30:37 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux`

**Compiler (Clang):**
`clang version 3.4 (tags/RELEASE_34/final) Target: x86_64-unknown-linux-gnu Thread model: posix`

**Compiler (gcc/g++):**
`g++ (Ubuntu 4.8.1-2ubuntu1~12.04) 4.8.1`

**Python:**
`Python 2.7.3`

**CMake:**
`cmake version 2.8.7`

**GNU Make:**
`GNU Make 3.81 This program built for x86_64-pc-linux-gnu`

**Sphinx:**
`Sphinx (sphinx-build) 1.4.8`

----

Continuous Integration 2
------------------------

**Operating System:**
`Darwin Traviss-Mac-1126.local 15.6.0 Darwin Kernel Version 15.6.0: Mon Aug 29 20:21:34 PDT 2016; root:xnu-3248.60.11~1/RELEASE_X86_64 x86_64`

**Compiler (Clang):**
`Apple LLVM version 7.3.0 (clang-703.0.31) Target: x86_64-apple-darwin15.6.0 Thread model: posix`

**Python:**
`Python 2.7.12`

**CMake:**
`cmake version 3.6.2`

**GNU Make:**
`GNU Make 3.81 This program built for i386-apple-darwin11.3.0`

**Sphinx:**
`Sphinx (sphinx-build) 1.4.8`


****************************
Bytecode Format and Encoding
****************************

coreVM employs a binary encoding format that is versioned and structured.
Internally referred as "structured bytecode", the encoding scheme makes both
encoding and decoding straightforward while aiming to encode bytecode data in an
efficient manner.

coreVM relies on the `Apache Avro data serialization system <https://avro.apache.org/docs/current/>`_
for bytecode serialization and deserialization, which offers a diverse range of
language supports, including C++, Java, and Python. This offers tremendous
flexibilities to language developers. For example, the bytecode decoder in
coreVM is written in C++, and the Python compiler in
`Project Pyta <roadmap.html#project-pyta>`_ is implemented in Python.

Below is the coreVM bytecode schema:

.. code-block:: json

  {
    "namespace": "corevm.avro",
    "type": "record",
    "name": "StructuredBytecode",
    "fields": [
      {
        "name": "format",
        "type": "string"
      },
      {
        "name": "format_version",
        "type": "string"
      },
      {
        "name": "target_version",
        "type": "string"
      },
      {
        "name": "path",
        "type": "string"
      },
      {
        "name": "timestamp",
        "type": "long"
      },
      {
        "name": "encoding",
        "type": "string"
      },
      {
        "name": "author",
        "type": "string"
      },
      {
        "name": "string_literal_table",
        "type": {
          "type": "array",
          "items":  {
            "type": "string"
          }
        }
      },
      {
        "name": "fpt_literal_table",
        "type": {
          "type": "array",
          "items":  {
            "type": "double"
          }
        }
      },
      {
        "name": "__MAIN__",
        "type": {
          "type": "array",
          "items": {
            "type": "record",
            "name": "closure",
            "fields": [
              {
                "name": "name",
                "type": "string"
              },
              {
                "name": "id",
                "type": "long"
              },
              {
                "name": "parent_id",
                "type": "long",
                "default": -1
              },
              {
                "name": "vector",
                "type": {
                  "type": "array",
                  "items": {
                    "type": "record",
                    "name": "instr",
                    "fields": [
                      {
                        "name": "code",
                        "type": "long"
                      },
                      {
                        "name": "oprd1",
                        "type": "long"
                      },
                      {
                        "name": "oprd2",
                        "type": "long"
                      }
                    ]
                  }
                }
              },
              {
                "name": "locs",
                "type": {
                  "type": "array",
                  "items": {
                    "type": "record",
                    "name": "loc",
                    "fields": [
                      {
                        "name": "index",
                        "type": "long"
                      },
                      {
                        "name": "lineno",
                        "type": "long"
                      },
                      {
                        "name": "col_offset",
                        "type": "long"
                      }
                    ]
                  }
                }
              },
              {
                "name": "catch_sites",
                "type": {
                  "type": "array",
                  "items": {
                    "type": "record",
                    "name": "catch_site",
                    "fields": [
                      {
                        "name": "from",
                        "type": "long"
                      },
                      {
                        "name": "to",
                        "type": "long"
                      },
                      {
                        "name": "dst",
                        "type": "long"
                      }
                    ]
                  }
                }
              }
            ]
          }
        }
      }
    ]
  }

Below are descriptions on the fields in the schema.

**Field "format"**

The format of the bytecode encoding format. Accepted values are "bytecode".


**Field "format_version"**

The version of the bytecode encoding format. Current version is `v0.0.1`.


**Field "target_version"**

The highest version of coreVM that this encoding format targets to. In other
words, the highest version of coreVM that can accept this format. Current
version is `v0.1.0`.


**Field "path"**

The absolute file path of this bytecode stored on disk.


**Field "timestamp"**

The UNIX timestamp of which this bytecode was created or updated.


**Field "encoding"**

String encoding used for the string literals in the bytecode
(e.g. "utf-8", "ascii", etc).


**Field "author"**

The name of the person whom authored this bytecode.


**Field "string_literal_table"**

An array of string literals.


**Field "fpt_literal_table"**

An array of floating-point literals.


**Field "__MAIN__"**

Highest level of bytecode execution related data. An array of "closures".


**Field "__MAIN__.name"**

Name of a closure.


**Field "__MAIN__.id"**

Integer identifier of a closure that uniquely identifies itself in the bytecode.


**Field "__MAIN__.parent_id"**

Optional integer identifier of a closure's parent.


**Field "__MAIN__.vector"**

An array of instructions of a code block.


**Field "__MAIN__.vector.code"**

Integer code of an instruction. Please see the "Instruction Set" section below
for more details.


**Field "__MAIN__.vector.oprd1"**

First operand of an instruction.


**Field "__MAIN__.vector.oprd2"**

Second operand of an instruction.


**Field "__MAIN__.locs"**

An array of source code location records.


**Field "__MAIN__.locs.lineno"**

Source code line number of a location record.


**Field "__MAIN__.locs.col_offset"**

Source code column offset of a location record.


**Field "__MAIN__.locs.index"**

Zero-based index of this location record in the bytecode.


**Field "__MAIN__.catch_sites"**

An array of exception handling related data, referred as "catch site".


**Field "__MAIN__.catch_sites.from"**

Index of instruction of current code block's instruction vector at which
exception handling should be enabled.


**Field "__MAIN__.catch_sites.to"**

Index of instruction of current code block's instruction vector at which
exception handling should be disabled.


**Field "__MAIN__.catch_sites.dst"**

Index of instruction of current code block's instruction vector to jump to
should an exception occurs between the "from" and "to" portion of the vector.


----


Instruction Set
===============

The coreVM instruction set contains a set of instructions in three-address
form that cover a huge range of capabilities. Each instruction is
compressed of a numeric code that denotes its identity, as well as two
optional operands. Instructions are categorized into groups by their
functionalities:

* :ref:`object-instructions`
* :ref:`control-instructions`
* :ref:`function-instructions`
* :ref:`runtime-instructions`
* :ref:`arithmetic-and-logic-instructions`
* :ref:`native-type-creation-instructions`
* :ref:`native-type-conversion-instructions`
* :ref:`native-type-manipulation-instructions`
* :ref:`native-string-type-instructions`
* :ref:`native-array-type-instructions`
* :ref:`native-map-type-instructions`


.. _object-instructions:

Object Instructions
-------------------

Instructions that interact with dynamic objects.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  new           0         0             Creates a new object and place it on top of the stack.
  ldobj         1         1             Load an object by its key and push it onto stack.
  stobj         2         1             Pops the object on top of the stack and stores it with a key into the frame.
  stobjn        3         2             Pops the object on top of the stack and stores it with a key into the `n`-th frame on the call stack from the top. A value of 0 means the top frame.
  getattr       4         1             Pop the object at the top of the stack, get its attribute and push it onto the stack.
  setattr       5         1             Pop the object at the top of the stack as the attribute, pop the next object as the target, and sets the attribute on the target.
  delattr       6         1             Pop the object at the top of the stack, and deletes its attribute and push it back onto the stack.
  hasattr2      7         0             Determines if the object on top of the stack has an attribute, with the attribute name being the string value of the element on top of the eval stack. Places the result on top of the eval stack.
  getattr2      8         0             Gets an attribute from the object on top of the stack, with the attribute ame being the string value of the element on top of the eval stack. Pops the object off the stack and places the result on top of the stack.
  setattr2      9         0             Pop the object at the top of the stack as the attribtue value, and set it as an attribute value on the next object on top of the stack, using the attribute name that is the string value of the element on top of the eval stack.
  delattr2      10        0             Deletes an attribute from the object on top of the stack, with the attribute name being the string value of the element on top of the eval stack.
  pop           11        0             Pops the object on top of the stack.
  ldbobj2       12        1             Load an invisible object by a key and push it onto the stack.
  stobj2        13        1             Pops the object on top of the stack and stores it with a key into the frame as an invisible object.
  delobj        14        1             Deletes an object from the current scope.
  delobj2       15        1             Deletes an invisible object from the current scope.
  getval        16        0             Copies the native type value of the top object of the stack and push it on top of the eval-stack.
  setval        17        0             Pops off the native type value off the eval-stack and assigns it to the top object of the stack.
  getval2       18        1             Copies of the native type value of the named object in the current frame, and pushes it on top of the eval stack.
  clrval        19        0             Clears the native type value from the top object of the stack.
  cpyval        20        1             Copies the native type value associated from the object on top of the stack onto the next object on the stack. The first operand is a value specifying the type of conversion to perform on the native type value copied.
  cpyrepr       21        0             Copies the string representation of the native type value from the object on top of the stack onto the next object onto the stack.
  istruthy      22        0             Computes the truthy value of the native type value associated with the object on top of the stack, and push the result on top of the eval stack.
  objeq         23        0             Pops off the top two objects on the stack and tests if they are the same object.
  objneq        24        0             Pops off the top two objects on the stack and tests if they are different objects.
  setctx        25        1             Sets the closure context of the object. The first operand is the closure ID.
  cldobj        26        2             Conditionally loads an object associated with the variable key value represented by either `oprd1` or `oprd2`, by evaluating the boolean equivalent of the object on top of the evaluation stack. Loads `oprd1` if the value evaluates to true, `oprd2` otherwise.
  rsetattrs     27        1             Reverse set attributes. Set the object on top of stack as the attribute values onto the objects pointed to as values in the native map equivalent on top of the eval stack.
  setattrs      28        1             Pops off the object on top of the stack, and set copies of all of its attributes onto the next on the stack. For each of the copied objects, set the second object on the stack as an attribute using the first operand as the attribute key.
  putobj        29        0             Pops the object on top of the stack, and pushes its value onto the top of the current evaluation stack.
  getobj        30        0             Pops the top of the eval stack, and put its value on the object stack.
  swap          31        0             Swaps the top two objects on top of the stack.
  setflgc       32        1             Sets the `IS_NOT_GARBAGE_COLLECTIBLE` flag on the object on top of the stack. The first operand is a boolean value used to set the value of the flag. A value of `1` sets the flag, `0` otherwise.
  setfldel      33        1             Sets the `IS_INDELIBLE` flag on the object on top of the stack. The first operand is a boolean vlaue used to set the value of the flag. A value of `1` sets the flag, `0` otherwise.
  setflcall     34        1             Sets the `IS_NON_CALLABLE` flag on the object on top of the stack. The first operand is a boolean value used to set the value of the flag. A value of `1` sets the flag, `0` otherwise.
  setflmute     35        1             Sets the `IS_IMMUTABLE` flag on the object on top of the stack. The first operand is a boolean value used to set the value of the flag. A value of `1` sets the flag, `0` otherwise.
  ============  ========  ============  ===============


.. _control-instructions:

Control Instructions
--------------------

Instructions that directly control flow of executions.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  pinvk         36        0             Prepares the invocation of a function. Creates a new frame on top of the call stack, and sets its closure context using the context of the object on top of the stack.
  invk          37        0             Invokes the vector of the object on top of the stack.
  rtrn          38        0             Unwinds from the current call frame and jumps to the previous one.
  jmp           39        1             Unconditionally jumps to a particular instruction address.
  jmpif         40        1             Conditionally jumps to a particular instruction address only if the top element on the eval stacks evaluates to True.
  jmpr          41        1             Unconditionally jumps to an instruction with an offset starting from the beginning of the current frame.
  exc           42        1             Pop the object at the top and raise it as an exception. The first operand is a boolean value indicating whether the runtime should search for a catch site in the current closure. A value of `false` will make the runtime pop the current frame.
  excobj        43        0             Gets the exception object associated with the current frame, and pushes it on top of the stack.
  clrexc        44        0             Clears the exception object associated with the frame on top of the call stack.
  jmpexc        45        2             Jumps to the specified address, based on the state of the exception object associated with the frame on top of the call stack. The first operand is the number of addresses to jump over starting from the current program counter. The second operand specifies whether or not to jump based on if the top of stack frame has an exception object. A value of `1` specifies the jump if the frame has an exception object, `0` otherwise.
  exit          46        1             Halts the execution of instructions and exits the program (with an optional exit code).
  ============  ========  ============  ===============


.. _function-instructions:

Function Instructions
---------------------

Instructions related to functions and call invocations.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  putarg        47        0             Pops the top object off the stack and assign it as the next argument for the next call.
  putkwarg      48        1             Pops the top object off the stack and assign it as the next keyword-argument for the next call.
  putargs       49        0             Pops the top object off the stack, retrieves its native type value as a native type array, and then iterate through each array element, use it as an object ID to retrieve an object from the heap, and assigns it as the next argument for the next call.
  putkwargs     50        0             Pops the top object off the stack, retrieves its native type value as a native type map, and then iterate through each key-value pair, use the value as an object ID to retrieve an object from the heap, and use the key as an encoding ID to assign the object as the next keyword-argument for the next call.
  getarg        51        1             Pops off the first argument for the current call and put it on the current frame using the encoding key specified in the first operand.
  getkwarg      52        2             If the top frame has the keyword-argument pair with the key specified as the first operand, pops off the pair and stores the value into the frame using the key. And, advance the program counter by the value specified in the second operand.
  getargs       53        0             Pops off all the arguments for the current call, insert them into a native-list and push it on top of eval-stack.
  getkwargs     54        0             Pops off all the keyword-arguments for the current call, insert them into a native-map and push it on top of eval-stack.
  hasargs       55        0             Determines if there are any arguments remaining on the current frame, and pushes the result onto the top of the eval stack.
  ============  ========  ============  ===============


.. _runtime-instructions:

Runtime Instructions
--------------------

Instructions related to a wide range of runtime functionalities.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  gc            56        0             Manually performs garbage collection.
  debug         57        1             Show debug information. The first operand is the set of debug options: 1. Show instructions in canonical form.
  dbgfrm        58        1             Show debug information on the current frame. The first operand is the set of debug options: 1. Show instructions in canonical form.
  dbgmem        59        1             Show information of current process memory usages. The first operand is the set of options: 1. Show peak virtual memory size and resident set size.
  dbgvar        60        1             Show information of a variable.
  print         61        2             Converts the native type value associated with the object on top of the stack into a native string, and prints it to std output. The second operand is a boolean value specifying whether a trailing new line character should be printed. Defaults to `false`.
  swap2         62        0             Swaps the top two elements on the evaluation stack.
  ============  ========  ============  ===============


.. _arithmetic-and-logic-instructions:

Arithmetic and Logic Instructions
---------------------------------

Instructions that deal with arithmetic and logical operations.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  pos           63        0             Apply the positive operation on the top element on the evaluation stack.
  neg           64        0             Apply the negation operation on the top element on the evaluation stack.
  inc           65        0             Apply the increment operation on the top element on the evaluation stack.
  dec           66        0             Apply the decrement operation on the top element on the evaluation stack.
  abs           67        0             Apply the `abs` operation on the top element on the evaluation stack.
  sqrt          68        0             Apply the `sqrt` operation on the top element on the evaluation stack.
  add           69        0             Pops the top two elements on the eval stack, applies the addition operation and push result onto eval stack.
  sub           70        0             Pops the top two elements on the eval stack, applies the subtraction operation and push result onto eval stack.
  mul           71        0             Pops the top two elements on the eval stack, applies the multiplication operation and push result onto eval stack.
  div           72        0             Pops the top two elements on the eval stack, applies the division operation and push result onto eval stack.
  mod           73        0             Pops the top two elements on the eval stack, applies the modulus operation and push result onto eval stack.
  pow           74        0             Pops the top two elements on the eval stack, applies the power operation and push result onto eval stack.
  bnot          75        0             Applies the bitwise NOT operation on the top element on the evaluation stack.
  band          76        0             Pops the top two elements on the eval stack, applies the bitwise AND operation and push result onto eval stack.
  bor           77        0             Pops the top two elements on the eval stack, applies the bitwise OR operation and push result onto eval stack.
  bxor          78        0             Pops the top two elements on the eval stack, applies the bitwise XOR operation and push result onto eval stack.
  bls           79        0             Pops the top two elements on the eval stack, applies the bitwise left shift operation and push result onto eval stack.
  brs           80        0             Pops the top two elements on the eval stack, applies the bitwise right shift operation and push result onto eval stack.
  eq            81        0             Pops the top two elements on the eval stack, applies the equality operation and push result onto eval stack.
  neq           82        0             Pops the top two elements on the eval stack, applies the inequality operation and push result onto eval stack.
  gt            83        0             Pops the top two elements on the eval stack, applies the greater than operation and push result onto eval stack.
  lt            84        0             Pops the top two elements on the eval stack, applies the less than operation and push result onto eval stack.
  gte           85        0             Pops the top two elements on the eval stack, applies the greater or equality operation and push result onto eval stack.
  lte           86        0             Pops the top two elements on the eval stack, applies the less or equality operation and push result onto eval stack.
  lnot          87        0             Apply the logic NOT operation on the top element on the evaluation stack.
  land          88        0             Pops the top two elements on the eval stack, applies the logical AND operation and push result onto eval stack.
  lor           89        0             Pops the top two elements on the eval stack, applies the logical OR operation and push result onto eval stack.
  cmp           90        0             Pops the top two elements on the eval stack, applies the "cmp" operation and push result onto eval stack.
  ============  ========  ============  ===============


.. _native-type-creation-instructions:

Native Type Creation Instructions
---------------------------------

Instructions for creating native type values.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  int8          91        1             Creates an instance of type `int8` and place it on top of eval stack.
  uint8         92        1             Creates an instance of type `uint8` and place it on top of eval stack.
  int16         93        1             Creates an instance of type `int16` and place it on top of eval stack.
  uint16        94        1             Creates an instance of type `uint16` and place it on top of eval stack.
  int32         95        1             Creates an instance of type `int32` and place it on top of eval stack.
  uint32        96        1             Creates an instance of type `uint32` and place it on top of eval stack.
  int64         97        1             Creates an instance of type `int64` and place it on top of eval stack.
  uint64        98        1             Creates an instance of type `uint64` and place it on top of eval stack.
  bool          99        1             Creates an instance of type `bool` and place it on top of eval stack.
  dec1          100       1             Creates an instance of type `dec` and place it on top of eval stack. The first operand represents the index of the floating-point literal stored in the corresponding compartment.
  dec2          101       1             Creates an instance of type `dec2` and place it on top of eval stack. The first operand represents the index of the floating-point literal stored in the corresponding compartment.
  str           102       1             Creates an instance of type `str` and place it on top of eval stack.
  ary           103       0             Creates an instance of type `array` and place it on top of eval stack.
  map           104       0             Creates an instance of type `map` and place it on top of eval stack.
  ============  ========  ============  ===============


.. _native-type-conversion-instructions:

Native Type Conversion Instructions
-----------------------------------

Instructions for native type conversions.

Note that conversions are only possible when makes sense, for example,
converting a 64-bit integer to 32-bit integer. When conversion is not possible,
an error will occur.


.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  toint8        105       0             Converts the element on top of the eval stack to type `int8`.
  touint8       106       0             Converts the element on top of the eval stack to type `uint8`.
  toint16       107       0             Converts the element on top of the eval stack to type `int16`.
  touint16      108       0             Converts the element on top of the eval stack to type `uint16`.
  toint32       109       0             Converts the element on top of the eval stack to type `int32`.
  touint32      110       0             Converts the element on top of the eval stack to type `uint32`.
  toint64       111       0             Converts the element on top of the eval stack to type `int64`.
  touint64      112       0             Converts the element on top of the eval stack to type `uint64`.
  tobool        113       0             Converts the element on top of the eval stack to type `bool`.
  todec1        114       0             Converts the element on top of the eval stack to type `dec`.
  todec2        115       0             Converts the element on top of the eval stack to type `dec2`
  tostr         116       0             Converts the element on top of the eval stack to type `string`.
  toary         117       0             Converts the element on top of the eval stack to type `array`.
  tomap         118       0             Converts the element on top of the eval stack to type `map`.
  ============  ========  ============  ===============


.. _native-type-manipulation-instructions:

Native Type Manipulation Instructions
-------------------------------------

Instructions for manipulating native type values.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  truthy        119       0             Computes a boolean truthy value based on the top element on the eval stack, and puts it on top of the stack.
  repr          120       0             Computes the string equivalent representation of the element on top of the eval stack, and push it on top of the stack.
  hash          121       0             Computes the non-crytographic hash value of the element on top of the eval stack, and push the result on top of the eval stack.
  slice         122       0             Computes the portion of the element on the top 3rd element of the eval stack as a sequence, using the 2nd and 1st top elements as the `start` and `stop` values as the indices range [start, stop).
  stride        123       0             Computes a new sequence of the element on the 2nd top eval stack as a sequence, using the top element as the `stride` interval.
  reverse       124       0             Computes the reverse of the element on top of the eval stack as a sequence.
  round         125       0             Rounds the second element on top of the eval stack using the number converted from the element on top of the eval stack.
  ============  ========  ============  ===============


.. _native-string-type-instructions:

Native String Type Instructions
-------------------------------

Instructions for manipulating native type values of the native string type.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  strlen        126       0             Pops the top element on the eval stack, and performs the "string size" operation.
  strat         127       0             Pops the top two elements on the eval stack, and performs the "string at" operation.
  strclr        128       0             Pops the top element on the eval stack, and performs the "string clear" operation.
  strapd        129       0             Pops the top two elements on the eval stack, and performs the "string append" operation.
  strpsh        130       0             Pops the top two elements on the eval stack, and performs the "string pushback" operation.
  strist        131       0             Pops the top three elements on the eval stack, and performs the "string insertion" operation.
  strist2       132       0             Pops the top three elements on the eval stack, and performs the "string insertion" operation.
  strers        133       0             Pops the top two elements on the eval stack, and performs the "string erase" operation.
  strers2       134       0             Pops the top two elements on the eval stack, and performs the "string erase" operation.
  strrplc       135       0             Pops the top four elements on the eval stack, and performs the "string replace" operation.
  strswp        136       0             Pops the top two elements on the eval stack, and performs the "string swap" operation.
  strsub        137       0             Pops the top two elements on the eval stack, and performs the "string substring" operation.
  strsub2       138       0             Pops the top three elements on the eval stack, and performs the "string substring" operation.
  strfnd        139       0             Pops the top two elements on the eval stack, and performs the "string find" operation.
  strfnd2       140       0             Pops the top three elements on the eval stack, and performs the "string find" operation.
  strrfnd       141       0             Pops the top two elements on the eval stack, and performs the "string rfind" operation.
  strrfnd2      142       0             Pops the top three elements on the eval stack, and performs the "string rfind2" operation.
  ============  ========  ============  ===============


.. _native-array-type-instructions:

Native Array Type Instructions
------------------------------

Instructions for manipulating native type values of the native array type.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  arylen        143       0             Pops the top element on the eval stack, and performs the "array size" operation.
  aryemp        144       0             Pops the top element on the eval stack, and performs the "array empty" operation.
  aryat         145       0             Pops the top two elements on the eval stack, and performs the "array at" operation.
  aryfrt        146       0             Pops the top element on the eval stack, and performs the "array front" operation.
  arybak        147       0             Pops the top element on the eval stack, and performs the "array back" operation.
  aryput        148       0             Pops the top three elements on the eval stack, and performs the "array put" operation.
  aryapnd       149       0             Pops the top two elements on the eval stack, and performs the "array append" operation.
  aryers        150       0             Pop the top two elements on the eval stack, and performs the "array erase" operation.
  arypop        151       0             Pops the top element on the eval stack, and performs the "array pop" operation.
  aryswp        152       0             Pops the top two elements on the eval stack, and performs the "array swap" operation.
  aryclr        153       0             Pops the top element on the eval stack, and performs the "array clear" operation.
  arymrg        154       0             Pops the top two elements on the eval stack, converts them to arrays, merge them into one single array, and put it back to the eval stack.
  ============  ========  ============  ===============


.. _native-map-type-instructions:

Native Map Type Instructions
----------------------------

Instructions for manipulating native type values of the native map type.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  maplen        155       0             Pops the top element on the eval stack, and performs the "map size" operation.
  mapemp        156       0             Pops the top element on the eval stack, and performs the "map empty" operation.
  mapfind       157       0             Pops the top two elements on the eval stack, and performs the "map find" operation.
  mapat         158       0             Pops the top two elements on the eval stack, and performs the "map at" operation.
  mapput        159       0             Pops the top three elements on the eval stack, and performs the "map put" operation.
  mapset        160       1             Converts the top element on the eval stack to a native map, and insert a key-value pair into it, with the key represented as the first operand, and the value as the object on top of the stack.
  mapers        161       0             Pops the top element on the eval stack, and performs the "map erase" operation.
  mapclr        162       0             Pops the top element on the eval stack, and performs the "map clear" operation.
  mapswp        163       0             Pops the top two elements on the eval stack, and performs the "map swap" operation.
  mapkeys       164       0             Inserts the keys of the map on top of the eval stack into an array, and place it on top of the eval stack.
  mapvals       165       0             Inserts the values of the map on top of the eval stack into an array, and place it on top of the eval stack.
  mapmrg        166       0             Pops the top two elements on the eval stack, converts them to maps, merge them into one single map, and put it back to the eval stack.
  ============  ========  ============  ===============


----

****
APIs
****

The coreVM library provides a set of powerful APIs that offer additional
capabilities beyond the functionalities from the instruction set. They provide
greater flexibilities and more granular controls to the execution of bytecodes
to developers.

The library is consisted of the following APIs:

  * :ref:`core-api`
  * :ref:`ir-api`
  * Debugging and Profiling API (coming soon)
  * Embedder API (coming soon)
  * Extension API (coming soon)
  * Threading API (coming soon)


.. _core-api:

Core API
========

The *Core API* provides interfaces that expose coreVM's fundamental functionalities.


**Bytecode Execution Configuration**

Header: `corevm/api/core/configuration.h`

.. cpp:class:: corevm::api::core::Configuration

  An encapsulation of a set of configuration parameters for bytecode execution.

  .. cpp:function:: Configuration()
    :noindex:

    Constructor.

  .. cpp:function:: static bool load_config(const char*, Configuration&)
    :noindex:

    Loads configuration values from a file that encapsulates the values in
    JSON format. Returns a boolean value indicating whether the operation has
    succeeded. Below is the schema for the JSON content:

    .. code-block:: json

      {
        "type": "object",
        "properties":
        {
          "heap-alloc-size": {
            "type": "integer"
          },
          "pool-alloc-size": {
            "type": "integer"
          },
          "gc-interval": {
            "type": "integer"
          },
          "gc-flag": {
            "type": "integer"
          },
          "logging": {
            "type": "string"
          }
        }
      }

  .. cpp:function:: void set_heap_alloc_size(uint64_t)
    :noindex:

    Sets the size (number of bytes) of the object heap. A default value is set
    if not specified.

  .. cpp:function:: void set_pool_alloc_size(uint64_t)
    :noindex:

    Sets the size (number of bytes) of the native types pool. A default value
    is set if not specified.

  .. cpp:function:: void set_gc_interval(uint32_t)
    :noindex:

    Sets the duration of time interval (in milliseconds) for triggering
    garbage collections. A default value is used if not specified.

  .. cpp:function:: void set_gc_flag(uint8_t)
    :noindex:

    Sets a flag for garbage collection. This is optional.

  .. cpp:function:: void set_log_mode(const char*)
    :noindex:

    Sets the logging mode. Acceptable values are "stdout", "stderr", and "file".
    A default value is used if not specified.

  .. cpp:function:: uint64_t heap_alloc_size() const
    :noindex:

    Gets the size (number of bytes) of the object heap.

  .. cpp:function:: uint64_t pool_alloc_size() const
    :noindex:

    Gets the size (number of bytes) of the native types pool.

  .. cpp:function:: uint32_t gc_interval() const
    :noindex:

    Gets the duration of time interval (in milliseconds) for triggering
    garbage collections.

  .. cpp:function:: bool has_gc_flag() const
    :noindex:

    Returns if the optional GC flag has been set.

  .. cpp:function:: uint8_t gc_flag() const
    :noindex:

    Gets the optional flag for garbage collection.

  .. cpp:function:: const std::string& log_mode() const
    :noindex:

    Gets the logging mode.


**Bytecode Execution Invocation**

Header: `corevm/api/core/entry.h`

.. cpp:function:: int corevm::api::core::invoke_from_file(const char* filepath, const corevm::api::core::Configuration& config)
  
  Executes the bytecode stored in `filepath`, along with the specified
  configuration object.

  Returns 0 on successful execution, non-zero values otherwise.


.. _ir-api:

IR API
======

The *IR API* provides interfaces for creating and manipulating bitcode in the
coreVM Intermediate Representation.

**IR Version**

Header: `corevm/api/ir/version.h`

.. cpp:enum:: corevm::api::ir::IR_VERSION

  An integer value that defines the current version of the IR format. The table
  below illustrates how to interpret the value:

  .. table::

    =================  =============================
       Description               Formulae
    =================  =============================
      Major version      `IR_VERSION / 100000`
      Minor version      `IR_VERSION / 100 % 1000`
      Patch version      `IR_VERSION % 100`
    =================  =============================


**********************************
coreVM Intermediate Representation
**********************************

Please refer to the `coreVM Intermediate Representation Reference Manual <ir_reference.html>`_.


****************
coreVM Toolchain
****************

The coreVM repo comes with a set of tools used for facilitating developments
and debugging when working with coreVM. These reside under the top level
`tools/ <https://github.com/yanzhengli/coreVM/tree/dev/tools>`_ directory.

Below are descriptions of each of the tools:

diff_benchmarks.py
==================

Computes and prints the diff between two benchmark runs, with each run's result
written in JSON format outputted by `benchmark <https://www.github.com/google/benchmark>`__.

Usage:

  .. code::

    python tools/diff_benchmarks.py --help

    Usage: diff_benchmarks.py LEFT_FILE RIGHT_FILE [options]

    Options:
      --version             show program's version number and exit
      -h, --help            show this help message and exit
      -c, --color           Display outputs in colors
      -m COMPARISON_METRIC, --metric=COMPARISON_METRIC
                            Comparion metric=<real_time|cpu_time>

extract_metadata
================

Prints out the coreVM instruction set data and the dynamic object flags data
in JSON format. This is useful for compilers or other programs that need to
generate coreVM bytecode.

Usage:

  .. code::

    ./build/tools/extract_metadata --help
  
    Extract coreVM info
    Usage: ./build/tools/extract_metadata [options]
    Options:
      --help                Print a help message and exit
      --output arg          Output file

Example:

  .. code::

    ./build/tools/extract_metadata --output out.txt

ir_gen
======

Generates dummy IR and write to a specified output file.

Usage:

  .. code::

    python tools/ir_gen.py --help

    Usage: ir_gen.py [options]

    Generates dummy coreVM IR

    Options:
      --version             show program's version number and exit
      -h, --help            show this help message and exit
      -o OUTPUT_FILE, --output=OUTPUT_FILE
                            Output file

Example:

  .. code::

    python tools/ir_gen.py --output sample.ir

ir_dis
======

coreVM IR disassembler. Prints out the IR textual representation when specified
an input file that contains the binary representation.

Usage:

  .. code::

    ./build/tools/ir_dis --help

    coreVM IR disassembler
    Usage: ./build/tools/ir_dis [options]
    Options:
      --help                Print a help message and exit
      --input arg           input file
      --output arg          output file

Example:

  .. code::

    ./build/tools/ir_dis --input sample.ir

Sample Output:

  .. code::

    "module name" : "Dummy_IR"
    "format version" : "1000"
    "target version" : "10"
    "path" : "./dummy_ir.ir"
    "author" : "Yanzheng Li"
    "timestamp" : "1472959465"

    type Person {
        string name;
        ui8 age;
        Person* sibling;
        Location* address;
        array [ 10 * Person ] friends;
    }

    type Location {
        string street_address;
        string* country;
        string zipcode;
    }

    type NullType {
    }

    def Person* createPerson(string* name, ui32 age) {
    entry:
        %person = alloca [ auto ] Person;
        setattr string "age" %age %person;
        br %age [ label #end, label #end ];
    end:
        ret Person %person;
    }

    def string compute(ui32 lhs_val, dpf rhs_val, array [ 4 * dpf ]* values) : createPerson {
    entry:
        %sum = add ui64 %lhs_val %rhs_val;
        putelement ui8 16 %values ui32 2;
    }

    def void doNothing() {
    }

 

ir_asm
======

coreVM IR assembler. Converts IR from textual representation to binary format.

Usage:

  .. code::

    ./build/tools/ir_asm --help

    coreVM IR assembler
    Usage: ./build/tools/ir_asm [options]
    Options:
      --help                Print a help message and exit
      --input arg           input file
      --output arg          output file
      --debug               debug mode

Example:

  .. code::

    ./build/tools/ir_asm --input sample.ir.txt --output sample_copy.ir

ir_stats
========

Prints out stats of a IR module.

Usage:

  .. code::

    ./build/tools/ir_stats --help

    Print stats of coreVM IR
    Usage: ./build/tools/ir_stats [options]
    Options:
      --help                Print a help message and exit
      --input arg           input file

Example:

  .. code::

    ./build/tools/ir_stats --input sample.ir

Sample Output:

  .. code::

    Module name: Dummy_IR
    Format version: v0.1.0
    Target version: v0.1.0
    Path: ./dummy_ir.ir
    Author: Yanzheng Li
    Timestamp: 1472959465

    Struct decls: 2
    Struct decl: Person
            Fields: 4
    Struct decl: Location
            Fields: 3

    Closures: 1
    Closure: find_friends
            Blocks: 2

    Total instruction count: 3
