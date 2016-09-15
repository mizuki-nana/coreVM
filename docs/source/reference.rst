.. Copyright Yanzheng Li. All rights reserved.

Reference Manual
================

This reference manual contains useful information for developers who wish to
make use of coreVM's functionalities.


Build and Install coreVM
------------------------

Steps for building coreVM
^^^^^^^^^^^^^^^^^^^^^^^^^

The project strives the make the build and installation process as
straightforward as possible. Currently, the build system requires
`GNU Make <https://www.gnu.org/software/make/>`_ and
`CMake <https://cmake.org/>`_.

1. Build and install all dependencies

  `git submodule update --init && make -C libs full=1`

  coreVM currently has the following dependencies:

  * `Boost <http://www.boost.org/>`_ version 1.55
  * `Google Test <https://code.google.com/p/googletest/>`_ version 1.7.0
  * `Sneaker <http://www.libsneaker.org/>`_ version 0.3.1
  * `Apache Avro <https://avro.apache.org/docs/current/api/cpp/html/>`_ version 1.7.7
  * `LLVM <http://www.llvm.org/>`_ version 3.4.0
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
^^^^^^^^^^^^^^^^^^

This sections describes the build environments used for development and testing.
Please refer to this as a reference if you are unsure whether your system meets
the requirements for building coreVM.

Development 1
#############

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
#############

**Operating System:**
`OS X 10.11.6 (15G1004) Darwin 15.6.0 root:xnu-3248.60.11~1/RELEASE_X86_64 x86_64`

**Compiler:**
`Apple LLVM version 7.3.0 (clang-703.0.31) Target: x86_64-apple-darwin15.6.0 Thread model: posix`

**Python:**
`Python 2.7.10`

**CMake:**
`cmake version 3.4.3`

**GNU Make:**
`GNU Make 3.81 This program built for i386-apple-darwin11.3.0`

**Sphinx:**
`Sphinx v1.3.5`

----

Continuous Integration 1
########################

**Operating System:**
`Linux 3.13.0-63-generic #104~precise1-Ubuntu SMP x86_64 GNU/Linux`

**Compiler:**
`clang version 3.4 (tags/RELEASE_34/final) Target: x86_64-unknown-linux-gnu Thread model: posix`

**Python:**
`Python 2.7.3`

**CMake:**
`cmake version 2.8.7`

**GNU Make:**
`GNU Make 3.81 This program built for x86_64-pc-linux-gnu`

**Sphinx:**
`Sphinx (sphinx-build) 1.4.6`

----

Continuous Integration 2
########################

**Operating System:**
`OS X 10.9.5 Darwin Kernel Version 13.4.0`

**Compiler:**
`Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn) Target: x86_64-apple-darwin13.4.0 Thread model: posix`

**Python:**
`Python 2.7.5`

**CMake:**
`cmake version 3.0.2`

**GNU Make:**
`GNU Make 3.81 This program built for i386-apple-darwin11.3.0`

**Sphinx:**
`Sphinx (sphinx-build) 1.4.6`


Bytecode Format and Encoding
----------------------------

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
---------------

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
^^^^^^^^^^^^^^^^^^^

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
  ldbobj2        12        1             Load an invisible object by a key and push it onto the stack.
  stobj2        13        1             Pops the object on top of the stack and stores it with a key into the frame as an invisible object.
  delobj        14        1             Deletes an object from the current scope.
  delobj2       15        1             Deletes an invisible object from the current scope.
  gethndl       16        0             Copies the native handle of the top object of the stack and push it on top of the eval-stack.
  sethndl       17        0             Pops off the native handle off the eval-stack and assigns it to the top object of the stack.
  gethndl2      18        1             Copies of the native type handle of the named object in the current frame, and pushes it on top of the eval stack.
  clrhndl       19        0             Clears the native handle from the top object of the stack.
  cpyhndl       20        1             Copies the native type handle associated from the object on top of the stack onto the next object on the stack. The first operand is a value specifying the type of conversion to perform on the native type handle copied.
  cpyrepr       21        0             Copies the string representation of the native type handle from the object on top of the stack onto the next object onto the stack.
  istruthy      22        0             Computes the truthy value of the native type handle associated with the object on top of the stack, and push the result on top of the eval stack.
  objeq         23        0             Pops off the top two objects on the stack and tests if they are the same object.
  objneq        24        0             Pops off the top two objects on the stack and tests if they are different objects.
  setctx        25        1             Sets the closure context of the object. The first operand is the closure ID.
  cldobj        26        2             Conditionally loads an object associated with the variable key value represented by either `oprd1` or `oprd2`, by evaluating the boolean equivalent of the object on top of the evaluation stack. Loads `oprd1` if the value evaluates to true, `oprd2` otherwise.
  setattrs      27        2             Pops off the object on top of the stack, and convert its native type handle to a native map. Then use its key-value pairs as attribute name-value pairs to set on the next object on the top of the stack. The first operand is a boolean value specifying whether each mapped object should be cloned before set on the target object. The second operand is a boolean value indicating if the native map values should be overriden with the cloned object IDs.
  rsetattrs     28        1             Reverse set attributes. Set the object on top of stack as the attribute values onto the objects pointed to as values in the native map equivalent on top of the eval stack.
  setattrs2     29        1             Pops off the object on top of the stack, and set copies of all of its attributes onto the next on the stack. For each of the copied objects, set the second object on the stack as an attribute using the first operand as the attribute key.
  putobj        30        0             Pops the object on top of the stack, and pushes its value onto the top of the current evaluation stack.
  getobj        31        0             Pops the top of the eval stack, and put its value on the object stack.
  swap          32        0             Swaps the top two objects on top of the stack.
  setflgc       33        1             Sets the `IS_NOT_GARBAGE_COLLECTIBLE` flag on the object on top of the stack. The first operand is a boolean value used to set the value of the flag. A value of `1` sets the flag, `0` otherwise.
  setfldel      34        1             Sets the `IS_INDELIBLE` flag on the object on top of the stack. The first operand is a boolean vlaue used to set the value of the flag. A value of `1` sets the flag, `0` otherwise.
  setflcall     35        1             Sets the `IS_NON_CALLABLE` flag on the object on top of the stack. The first operand is a boolean value used to set the value of the flag. A value of `1` sets the flag, `0` otherwise.
  setflmute     36        1             Sets the `IS_IMMUTABLE` flag on the object on top of the stack. The first operand is a boolean value used to set the value of the flag. A value of `1` sets the flag, `0` otherwise.
  ============  ========  ============  ===============


.. _control-instructions:

Control Instructions
^^^^^^^^^^^^^^^^^^^^

Instructions that directly control flow of executions.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  pinvk         37        0             Prepares the invocation of a function. Creates a new frame on top of the call stack, and sets its closure context using the context of the object on top of the stack.
  invk          38        0             Invokes the vector of the object on top of the stack.
  rtrn          39        0             Unwinds from the current call frame and jumps to the previous one.
  jmp           40        1             Unconditionally jumps to a particular instruction address.
  jmpif         41        1             Conditionally jumps to a particular instruction address only if the top element on the eval stacks evaluates to True.
  jmpr          42        1             Unconditionally jumps to an instruction with an offset starting from the beginning of the current frame.
  exc           43        1             Pop the object at the top and raise it as an exception. The first operand is a boolean value indicating whether the runtime should search for a catch site in the current closure. A value of `false` will make the runtime pop the current frame.
  excobj        44        0             Gets the exception object associated with the current frame, and pushes it on top of the stack.
  clrexc        45        0             Clears the exception object associated with the frame on top of the call stack.
  jmpexc        46        2             Jumps to the specified address, based on the state of the exception object associated with the frame on top of the call stack. The first operand is the number of addresses to jump over starting from the current program counter. The second operand specifies whether or not to jump based on if the top of stack frame has an exception object. A value of `1` specifies the jump if the frame has an exception object, `0` otherwise.
  exit          47        1             Halts the execution of instructions and exits the program (with an optional exit code).
  ============  ========  ============  ===============


.. _function-instructions:

Function Instructions
^^^^^^^^^^^^^^^^^^^^^

Instructions related to functions and call invocations.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  putarg        48        0             Pops the top object off the stack and assign it as the next argument for the next call.
  putkwarg      49        1             Pops the top object off the stack and assign it as the next keyword-argument for the next call.
  putargs       50        0             Pops the top object off the stack, retrieves its native type handle as a native type array, and then iterate through each array element, use it as an object ID to retrieve an object from the heap, and assigns it as the next argument for the next call.
  putkwargs     51        0             Pops the top object off the stack, retrieves its native type handle as a native type map, and then iterate through each key-value pair, use the value as an object ID to retrieve an object from the heap, and use the key as an encoding ID to assign the object as the next keyword-argument for the next call.
  getarg        52        1             Pops off the first argument for the current call and put it on the current frame using the encoding key specified in the first operand.
  getkwarg      53        2             If the top frame has the keyword-argument pair with the key specified as the first operand, pops off the pair and stores the value into the frame using the key. And, advance the program counter by the value specified in the second operand.
  getargs       54        0             Pops off all the arguments for the current call, insert them into a native-list and push it on top of eval-stack.
  getkwargs     55        0             Pops off all the keyword-arguments for the current call, insert them into a native-map and push it on top of eval-stack.
  hasargs       56        0             Determines if there are any arguments remaining on the current frame, and pushes the result onto the top of the eval stack.
  ============  ========  ============  ===============


.. _runtime-instructions:

Runtime Instructions
^^^^^^^^^^^^^^^^^^^^

Instructions related to a wide range of runtime functionalities.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  gc            57        0             Manually performs garbage collection.
  debug         58        1             Show debug information. The first operand is the set of debug options: 1. Show instructions in canonical form.
  dbgfrm        59        1             Show debug information on the current frame. The first operand is the set of debug options: 1. Show instructions in canonical form.
  dbgmem        60        1             Show information of current process memory usages. The first operand is the set of options: 1. Show peak virtual memory size and resident set size.
  dbgvar        61        1             Show information of a variable.
  print         62        2             Converts the native type handle associated with the object on top of the stack into a native string, and prints it to std output. The second operand is a boolean value specifying whether a trailing new line character should be printed. Defaults to `false`.
  swap2         63        0             Swaps the top two elements on the evaluation stack.
  ============  ========  ============  ===============


.. _arithmetic-and-logic-instructions:

Arithmetic and Logic Instructions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instructions that deal with arithmetic and logical operations.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  pos           64        0             Apply the positive operation on the top element on the evaluation stack.
  neg           65        0             Apply the negation operation on the top element on the evaluation stack.
  inc           66        0             Apply the increment operation on the top element on the evaluation stack.
  dec           67        0             Apply the decrement operation on the top element on the evaluation stack.
  abs           68        0             Apply the `abs` operation on the top element on the evaluation stack.
  sqrt          69        0             Apply the `sqrt` operation on the top element on the evaluation stack.
  add           70        0             Pops the top two elements on the eval stack, applies the addition operation and push result onto eval stack.
  sub           71        0             Pops the top two elements on the eval stack, applies the subtraction operation and push result onto eval stack.
  mul           72        0             Pops the top two elements on the eval stack, applies the multiplication operation and push result onto eval stack.
  div           73        0             Pops the top two elements on the eval stack, applies the division operation and push result onto eval stack.
  mod           74        0             Pops the top two elements on the eval stack, applies the modulus operation and push result onto eval stack.
  pow           75        0             Pops the top two elements on the eval stack, applies the power operation and push result onto eval stack.
  bnot          76        0             Applies the bitwise NOT operation on the top element on the evaluation stack.
  band          77        0             Pops the top two elements on the eval stack, applies the bitwise AND operation and push result onto eval stack.
  bor           78        0             Pops the top two elements on the eval stack, applies the bitwise OR operation and push result onto eval stack.
  bxor          79        0             Pops the top two elements on the eval stack, applies the bitwise XOR operation and push result onto eval stack.
  bls           80        0             Pops the top two elements on the eval stack, applies the bitwise left shift operation and push result onto eval stack.
  brs           81        0             Pops the top two elements on the eval stack, applies the bitwise right shift operation and push result onto eval stack.
  eq            82        0             Pops the top two elements on the eval stack, applies the equality operation and push result onto eval stack.
  neq           83        0             Pops the top two elements on the eval stack, applies the inequality operation and push result onto eval stack.
  gt            84        0             Pops the top two elements on the eval stack, applies the greater than operation and push result onto eval stack.
  lt            85        0             Pops the top two elements on the eval stack, applies the less than operation and push result onto eval stack.
  gte           86        0             Pops the top two elements on the eval stack, applies the greater or equality operation and push result onto eval stack.
  lte           87        0             Pops the top two elements on the eval stack, applies the less or equality operation and push result onto eval stack.
  lnot          88        0             Apply the logic NOT operation on the top element on the evaluation stack.
  land          89        0             Pops the top two elements on the eval stack, applies the logical AND operation and push result onto eval stack.
  lor           90        0             Pops the top two elements on the eval stack, applies the logical OR operation and push result onto eval stack.
  cmp           91        0             Pops the top two elements on the eval stack, applies the "cmp" operation and push result onto eval stack.
  ============  ========  ============  ===============


.. _native-type-creation-instructions:

Native Type Creation Instructions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instructions for creating native type handles.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  int8          92        1             Creates an instance of type `int8` and place it on top of eval stack.
  uint8         93        1             Creates an instance of type `uint8` and place it on top of eval stack.
  int16         94        1             Creates an instance of type `int16` and place it on top of eval stack.
  uint16        95        1             Creates an instance of type `uint16` and place it on top of eval stack.
  int32         96        1             Creates an instance of type `int32` and place it on top of eval stack.
  uint32        97        1             Creates an instance of type `uint32` and place it on top of eval stack.
  int64         98        1             Creates an instance of type `int64` and place it on top of eval stack.
  uint64        99        1             Creates an instance of type `uint64` and place it on top of eval stack.
  bool          100       1             Creates an instance of type `bool` and place it on top of eval stack.
  dec1          101       1             Creates an instance of type `dec` and place it on top of eval stack. The first operand represents the index of the floating-point literal stored in the corresponding compartment.
  dec2          102       1             Creates an instance of type `dec2` and place it on top of eval stack. The first operand represents the index of the floating-point literal stored in the corresponding compartment.
  str           103       1             Creates an instance of type `str` and place it on top of eval stack.
  ary           104       0             Creates an instance of type `array` and place it on top of eval stack.
  map           105       0             Creates an instance of type `map` and place it on top of eval stack.
  ============  ========  ============  ===============


.. _native-type-conversion-instructions:

Native Type Conversion Instructions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instructions for native type conversions.

Note that conversions are only possible when makes sense, for example,
converting a 64-bit integer to 32-bit integer. When conversion is not possible,
an error will occur.


.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  toint8        106       0             Converts the element on top of the eval stack to type `int8`.
  touint8       107       0             Converts the element on top of the eval stack to type `uint8`.
  toint16       108       0             Converts the element on top of the eval stack to type `int16`.
  touint16      109       0             Converts the element on top of the eval stack to type `uint16`.
  toint32       110       0             Converts the element on top of the eval stack to type `int32`.
  touint32      111       0             Converts the element on top of the eval stack to type `uint32`.
  toint64       112       0             Converts the element on top of the eval stack to type `int64`.
  touint64      113       0             Converts the element on top of the eval stack to type `uint64`.
  tobool        114       0             Converts the element on top of the eval stack to type `bool`.
  todec1        115       0             Converts the element on top of the eval stack to type `dec`.
  todec2        116       0             Converts the element on top of the eval stack to type `dec2`
  tostr         117       0             Converts the element on top of the eval stack to type `string`.
  toary         118       0             Converts the element on top of the eval stack to type `array`.
  tomap         119       0             Converts the element on top of the eval stack to type `map`.
  ============  ========  ============  ===============


.. _native-type-manipulation-instructions:

Native Type Manipulation Instructions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instructions for manipulating native type handles.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  truthy        120       0             Computes a boolean truthy value based on the top element on the eval stack, and puts it on top of the stack.
  repr          121       0             Computes the string equivalent representation of the element on top of the eval stack, and push it on top of the stack.
  hash          122       0             Computes the non-crytographic hash value of the element on top of the eval stack, and push the result on top of the eval stack.
  slice         123       0             Computes the portion of the element on the top 3rd element of the eval stack as a sequence, using the 2nd and 1st top elements as the `start` and `stop` values as the indices range [start, stop).
  stride        124       0             Computes a new sequence of the element on the 2nd top eval stack as a sequence, using the top element as the `stride` interval.
  reverse       125       0             Computes the reverse of the element on top of the eval stack as a sequence.
  round         126       0             Rounds the second element on top of the eval stack using the number converted from the element on top of the eval stack.
  ============  ========  ============  ===============


.. _native-string-type-instructions:

Native String Type Instructions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instructions for manipulating native type handles of the native string type.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  strlen        127       0             Pops the top element on the eval stack, and performs the "string size" operation.
  strat         128       0             Pops the top two elements on the eval stack, and performs the "string at" operation.
  strclr        129       0             Pops the top element on the eval stack, and performs the "string clear" operation.
  strapd        130       0             Pops the top two elements on the eval stack, and performs the "string append" operation.
  strpsh        131       0             Pops the top two elements on the eval stack, and performs the "string pushback" operation.
  strist        132       0             Pops the top three elements on the eval stack, and performs the "string insertion" operation.
  strist2       133       0             Pops the top three elements on the eval stack, and performs the "string insertion" operation.
  strers        134       0             Pops the top two elements on the eval stack, and performs the "string erase" operation.
  strers2       135       0             Pops the top two elements on the eval stack, and performs the "string erase" operation.
  strrplc       136       0             Pops the top four elements on the eval stack, and performs the "string replace" operation.
  strswp        137       0             Pops the top two elements on the eval stack, and performs the "string swap" operation.
  strsub        138       0             Pops the top two elements on the eval stack, and performs the "string substring" operation.
  strsub2       139       0             Pops the top three elements on the eval stack, and performs the "string substring" operation.
  strfnd        140       0             Pops the top two elements on the eval stack, and performs the "string find" operation.
  strfnd2       141       0             Pops the top three elements on the eval stack, and performs the "string find" operation.
  strrfnd       142       0             Pops the top two elements on the eval stack, and performs the "string rfind" operation.
  strrfnd2      143       0             Pops the top three elements on the eval stack, and performs the "string rfind2" operation.
  ============  ========  ============  ===============


.. _native-array-type-instructions:

Native Array Type Instructions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instructions for manipulating native type handles of the native array type.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  arylen        144       0             Pops the top element on the eval stack, and performs the "array size" operation.
  aryemp        145       0             Pops the top element on the eval stack, and performs the "array empty" operation.
  aryat         146       0             Pops the top two elements on the eval stack, and performs the "array at" operation.
  aryfrt        147       0             Pops the top element on the eval stack, and performs the "array front" operation.
  arybak        148       0             Pops the top element on the eval stack, and performs the "array back" operation.
  aryput        149       0             Pops the top three elements on the eval stack, and performs the "array put" operation.
  aryapnd       150       0             Pops the top two elements on the eval stack, and performs the "array append" operation.
  aryers        151       0             Pop the top two elements on the eval stack, and performs the "array erase" operation.
  arypop        152       0             Pops the top element on the eval stack, and performs the "array pop" operation.
  aryswp        153       0             Pops the top two elements on the eval stack, and performs the "array swap" operation.
  aryclr        154       0             Pops the top element on the eval stack, and performs the "array clear" operation.
  arymrg        155       0             Pops the top two elements on the eval stack, converts them to arrays, merge them into one single array, and put it back to the eval stack.
  ============  ========  ============  ===============


.. _native-map-type-instructions:

Native Map Type Instructions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instructions for manipulating native type handles of the native map type.

.. table::

  ============  ========  ============  ===============
    Mnemonic     Opcode     Operands      Description
  ============  ========  ============  ===============
  maplen        156       0             Pops the top element on the eval stack, and performs the "map size" operation.
  mapemp        157       0             Pops the top element on the eval stack, and performs the "map empty" operation.
  mapfind       158       0             Pops the top two elements on the eval stack, and performs the "map find" operation.
  mapat         159       0             Pops the top two elements on the eval stack, and performs the "map at" operation.
  mapput        160       0             Pops the top three elements on the eval stack, and performs the "map put" operation.
  mapset        161       1             Converts the top element on the eval stack to a native map, and insert a key-value pair into it, with the key represented as the first operand, and the value as the object on top of the stack.
  mapers        162       0             Pops the top element on the eval stack, and performs the "map erase" operation.
  mapclr        163       0             Pops the top element on the eval stack, and performs the "map clear" operation.
  mapswp        164       0             Pops the top two elements on the eval stack, and performs the "map swap" operation.
  mapkeys       165       0             Inserts the keys of the map on top of the eval stack into an array, and place it on top of the eval stack.
  mapvals       166       0             Inserts the values of the map on top of the eval stack into an array, and place it on top of the eval stack.
  mapmrg        167       0             Pops the top two elements on the eval stack, converts them to maps, merge them into one single map, and put it back to the eval stack.
  ============  ========  ============  ===============


----


APIs
----

The coreVM library provides a set of powerful APIs that offer additional
capabilities beyond the functionalities from the instruction set. They provide
greater flexibilities and more granular controls to the execution of bytecodes
to developers.

The library is consisted of the following APIs:

  * :ref:`core-api`
  * Debugging and Profiling API (coming soon)
  * Embedder API (coming soon)
  * Extension API (coming soon)
  * Threading API (coming soon)


.. _core-api:

Core API
^^^^^^^^

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


coreVM Intermediate Representation
----------------------------------

The coreVM Intermediate Representation, also referred as "coreVM IR", is an
abstract format that can represent the imperative and declarative semantics of
most programming languages. It is also the entry point of which coreVM's JIT
pipeline starts.

The coreVM IR captures the constructs and semantics of programming languages
in a high-level and generalized form. Compared to some other language
intermediate representations, such as LLVM IR, coreVM's syntax and semantics are
much simpler, and consequently it is not designed to capture all the low-level
details. The coreVM IR is designed this way so that it is easy to generate IR
from either raw source code or coreVM bytecode. In addition, it is subject to
additional lowering processes in order to be translated into lower-form
representations.


IR Format and Structure
^^^^^^^^^^^^^^^^^^^^^^^

Similar to the coreVM bytecode format, the IR is currently defined as a schema
based on the `Apache Avro data serialization system <https://avro.apache.org/docs/current/>`_.
Below is the IR schema:

.. code-block:: json

  {
    "namespace": "corevm.ir",
    "type": "record",
    "name": "IRModule",
    "fields": [
      {
        "name": "meta",
        "type": {
          "type": "record",
          "name": "IRModuleMeta",
          "fields": [
            {
              "name": "name",
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
              "name": "author",
              "type": "string"
            },
            {
              "name": "timestamp",
              "type": "long"
            }
          ]
        }
      },
      {
        "name": "types",
        "type": {
          "type": "array",
          "items": {
            "name": "IRStructDecl",
            "type": "record",
            "fields": [
              {
                "name": "name",
                "type": "string"
              },
              {
                "name": "fields",
                "type": {
                  "type": "array",
                  "items": {
                    "type": "record",
                    "name": "IRStructField",
                    "fields": [
                      {
                        "name": "identifier",
                        "type": "string"
                      },
                      {
                        "name": "ref_type",
                        "type": {
                          "type": "enum",
                          "name": "IRValueRefType",
                          "symbols": [
                            "value",
                            "pointer"
                          ]
                        }
                      },
                      {
                        "name": "type",
                        "type": [
                          "string",
                          {
                            "type": "enum",
                            "name": "IRValueType",
                            "symbols": [
                              "voidtype",
                              "boolean",
                              "i8",
                              "ui8",
                              "i16",
                              "ui16",
                              "i32",
                              "ui32",
                              "i64",
                              "ui64",
                              "spf",
                              "dpf",
                              "string",
                              "array",
                              "structtype",
                              "ptrtype"
                            ]
                          }
                        ]
                      }
                    ]
                  }
                }
              }
            ]
          }
        }
      },
      {
        "name": "closures",
        "type": {
          "type": "array",
          "items": {
            "type": "record",
            "name": "IRClosure",
            "fields": [
              {
                "name": "name",
                "type": "string"
              },
              {
                "name": "parent",
                "type": ["null", "string"]
              },
              {
                "name": "rettype",
                "type": "corevm.ir.IRValueType"
              },
              {
                "name": "ret_reftype",
                "type": "corevm.ir.IRValueRefType"
              },
              {
                "name": "parameters",
                "type": {
                  "type": "array",
                  "items": {
                    "type": "record",
                    "name": "IRParameter",
                    "fields": [
                      {
                        "name": "identifier",
                        "type": "string"
                      },
                      {
                        "name": "ref_type",
                        "type": "corevm.ir.IRValueRefType"
                      },
                      {
                        "name": "type",
                        "type": "corevm.ir.IRValueType"
                      }
                    ]
                  }
                }
              },
              {
                "name": "blocks",
                "type": {
                  "type": "array",
                  "items": {
                    "type": "record",
                    "name": "IRBasicBlock",
                    "fields": [
                      {
                        "name": "label",
                        "type": "string"
                      },
                      {
                        "name": "body",
                        "type": {
                          "type": "array",
                          "items": {
                            "type": "record",
                            "name": "IRInstruction",
                            "fields": [
                              {
                                "name": "target",
                                "type": [
                                  "null",
                                  "string"
                                ],
                                "default": null
                              },
                              {
                                "name": "type",
                                "type": "corevm.ir.IRValueType"
                              },
                              {
                                "name": "opcode",
                                "type": {
                                  "type": "enum",
                                  "name": "IROpcode",
                                  "symbols": [
                                    "alloca",
                                    "load",
                                    "store",
                                    "getattr",
                                    "setattr",
                                    "delattr",
                                    "getelement",
                                    "putelement",
                                    "len",
                                    "ret",
                                    "br",
                                    "switch2",
                                    "pos",
                                    "neg",
                                    "inc",
                                    "dec",
                                    "add",
                                    "sub",
                                    "mul",
                                    "div",
                                    "mod",
                                    "bnot",
                                    "band",
                                    "bor",
                                    "bxor",
                                    "bls",
                                    "brs",
                                    "eq",
                                    "neq",
                                    "gt",
                                    "lt",
                                    "gte",
                                    "lte",
                                    "lnot",
                                    "land",
                                    "lor",
                                    "cmp",
                                    "call"
                                  ]
                                }
                              },
                              {
                                "name": "opcodeval",
                                "type": {
                                  "type": "record",
                                  "name": "IRValue",
                                  "fields": [
                                    {
                                      "name": "type",
                                      "type": "corevm.ir.IRValueType"
                                    },
                                    {
                                      "name": "value",
                                      "type": [
                                        "null",
                                        "boolean",
                                        "int",
                                        "long",
                                        "float",
                                        "double",
                                        "string",
                                        {
                                          "type": "record",
                                          "name": "IRArrayValue",
                                          "fields": [
                                            {
                                              "type": "corevm.ir.IRValueType",
                                              "name": "type"
                                            },
                                            {
                                              "type": "int",
                                              "name": "len"
                                            }
                                          ]
                                        }
                                      ]
                                    }
                                  ]
                                }
                              },
                              {
                                "name": "oprds",
                                "type": {
                                  "type": "array",
                                  "items": {
                                    "type": "record",
                                    "name": "IROperand",
                                    "fields": [
                                      {
                                        "name": "type",
                                        "type": {
                                          "type": "enum",
                                          "name": "IROperandType",
                                          "symbols": [
                                            "constant",
                                            "ref"
                                          ]
                                        }
                                      },
                                      {
                                        "name": "value",
                                        "type": [
                                          "corevm.ir.IRValue",
                                          "string"
                                        ]
                                      }
                                    ]
                                  }
                                }
                              },
                              {
                                "name": "labels",
                                "type": [
                                  "null",
                                  {
                                    "type": "array",
                                    "items": {
                                      "type": "record",
                                      "name": "IRLabel",
                                      "fields": [
                                        {
                                          "name": "name",
                                          "type": "string"
                                        }
                                      ]
                                    }
                                  }
                                ],
                                "default": null
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
        }
      }
    ]
  }

Below are descriptions of the entities defined in the schema.

**Entity 'IRModule'**

Highest level of entity in IR, encapsulates all the data and metadata associated
with a module, which corresponds to a physical translation unit.

**Entity 'IRModuleMeta'**

Entity that captures all the metadata of a module.

**Entity 'IRStructDecl'**

Represents a custom structure declaration.

**Entity 'IRStructField'**

Represents a single field in a structure declaration.

**Enumeration 'IRValueRefType'**

Represents a set of types that a value can be referenced. Currently a value can
be referenced either via by-value or by-pointer.

.. table::

  =============  =================================
       Type             Description
  =============  =================================
    `value`         Reference-by-value.
    `pointer`       Reference-by-pointer.
  =============  =================================

**Enumeration 'IRValueType'**

Represents a set of primitive types. Possible values are:

.. table::

  ==============  ========================================
       Type         Description
  ==============  ========================================
    `voidtype`      Void type.
    `boolean`       Boolean type.
    `i8`            Signed 8-bit integer.
    `ui8`           Unsigned 8-bit integer.
    `i16`           Signed 16-bit integer.
    `ui16`          Unsigned 16-bit integer.
    `i32`           Signed 32-bit integer.
    `ui32`          Unsigned 32-bit integer.
    `i64`           Signed 64-bit integer.
    `ui64`          Unsigned 64-bit integer.
    `spf`           Single-precision floating point.
    `dpf`           Double-precision floating point.
    `string`        String type.
    `array`         Fixed-size array type.
    `structtype`    Statically-declared structure type.
    `ptrtype`       Object pointer type.
  ==============  ========================================

**Entity 'IRArrayValue'**

Represents a fixed and typed array type. The entity is consisted of the type
of the encapsulated elements, as well as the size of the array.

**Entity 'IRClosure'**

Represents a scoped function declaration. Closures allow function declarations
to be hierarchically scoped.

**Entity 'IRParameter'**

Represents a function parameter.

**Entity 'IRBasicBlock'**

Represents a basic block within a function. Each basic block is uniquely
identified by its label. A function body can be consisted of one or multiple
basic blocks.

**Entity 'IRInstruction'**

Represents a single instruction statement. Each instruction is consisted of an
opcode, an instruction value (primary operand value), an instruction value type,
an optional target (for instructions that return values), one or multiple
operands, and an optional set of labeled jump locations.

**Enumeration 'IROpcode'**

The set of opcodes defined in the IR.

**Entity 'IRValue'**

Represents a constant value.

**Enumeration 'IROperand'**

Represents an operand in an instruction statement.

**Enumeration 'IROperandType'**

Type of an instruction operand, can be either a "variable" reference or a
constant.

  ==============  ==============================
       Type         Description
  ==============  ==============================
    `constant`      Literal constant.
    `ref`           Variable reference.
  ==============  ==============================

**Entity 'IRLabel'**

Represents a labeled jump location used in an instruction.

----

IR Instruction Set
^^^^^^^^^^^^^^^^^^

This section describes the IR's instruction set.

'alloca' Instruction
####################

`target = alloca <type>`

Allocates a object on the stack, and returns a pointer that references the
object.

'load' Instruction
##################

`target = load <type> <oprd>`

Reads the value from a variable.

'store' Instruction
###################

`store <type> #constant-value <dst>`

Writes a value to a referenced variable.

'getattr' Instruction
#####################

`target = getattr #constant-string <oprd>`

Retrieves the attribute of an object and returns a pointer that references the
value.

'setattr' Instruction
#####################

`setattr #constant-string <src> <dst>`

Sets the attribute value `src` to `dst`.

'delattr' Instruction
#####################

`delattr #constant-string <dst>`

Deletes the attribute value in `dst`.

'getelement' Instruction
########################

`target = getelement <type> <src> <idx>`

Gets the element from an array with an index value.

'putelement' Instruction
########################

`putelement <src> <dst> <idx>`

Inserts the element specified by `src` into the array `dst` at index `idx`.

'len' Instruction
#################

`target = len <oprd>`

Gets the length of an array.

'ret' Instruction
#################

`ret`

`ret <type> <oprd>`

Returns control back to previous frame.

'br' Instruction
################

`br <cond> [ label #iftrue, label #iffalse ]`

Branches to either one of two labels depending on a conditional value.


'switch2' Instruction
#####################

`switch2 <value> <case1>, <case2>, .... [ label #case1, label #case2, ... ]`

Switch statement. Jumps to one of a set of labels based on a target value, and
a set of predicates specified as the rest of the operands.

'pos' Instruction
#################

`target = pos <type> <oprd>`

Evaluates to the positive expression of the specified operand.

'neg' Instruction
#################

`target = neg <type> <oprd>`

Evaluates to the negative expression of the specified operand.

'inc' Instruction
#################

`target = inc <type> <oprd>`

Increases the value by 1 of the specified instruction.

'dec' Instruction
#################

`target = dec <type> <oprd>`

Decreases the value by 1 of the specified instruction.

'add' Instruction
#################

`target = add <type> <oprd1> <oprd2>`

Adds the values of two expressions.

'sub' Instruction
#################

`target = sub <type> <oprd1> <oprd2>`

Subtracts the values of two expressions.

'mul' Instruction
#################

`target = mul <type> <oprd1> <oprd2>`

Multiplies the values of two expressions.

'div' Instruction
#################

`target = div <type> <oprd1> <oprd2>`

Divides the values of two expressions.

'mod' Instruction
#################

`target = mod <type> <oprd1> <oprd2>`

Computes the modulus value of two expressions.

'bnot' Instruction
##################

`target = bnot <type> <oprd>`

Computes the bitwise NOT evaluation of a value.

'band' Instruction
##################

`target = band <type> <oprd1> <oprd2>`

Computes the bitwise AND evaluation of two values.

'bor' Instruction
#################

`target = bor <type> <oprd1> <oprd2>`

Computes the bitwise OR evaluation of two values.

'bxor' Instruction
##################

`target = bxor <type> <oprd1> <oprd2>`

Computes the bitwise XOR evaluation of two values.

'bls' Instruction
#################

`target = bls <type> <oprd>`

Computes the bitwise-left-shift evaluation of the specified value.

'brs' Instruction
#################

`target = brs <type> <oprd>`

Computes the bitwise-right-shift evaluation of the specified value.

'eq' Instruction
################

`target = eq <oprd1> <oprd2>`

Computes the equality evaluation of two values.

'neq' Instruction
#################

`target = neq <oprd1> <oprd2>`

Computes the non-equality evaluation of two values.

'gt' Instruction
################

`target = gt <oprd1> <oprd2>`

Computes the greater-than evaluation of two values.

'lt' Instruction
################

`target = lt <oprd1> <oprd2>`

Computes the less-than evaluation of two values.

'gte' Instruction
#################

`target = gte <oprd1> <oprd2>`

Computes the greater-or-equal-to evaluation of two values.

'lte' Instruction
#################

`target = lte <type> <oprd1> <oprd2>`

Computes the less-or-equal-to evaluation of two values.

'lnot' Instruction
##################

`target = lnot <type> <oprd>`

Computes the logical NOT evaluation of a value.

'land' Instruction
##################

`target = land <type> <oprd1> <oprd2>`

Computes the logical AND evaluation of two values.

'lor' Instruction
#################

`target = lor <type> <oprd1> <oprd2>`

Computes the logical OR evaluation of two values.

'cmp' Instruction
#################

`target = cmp <oprd1> <oprd2>`

Equality comparison between two operands. Results in `-1` if the left-hand-side
is considered less than the right-hand-side, `0` if they are evaluated to be
equal, and `1` otherwise.

'call' Instruction
##################

`target = call <ret-type> <call-target>(<oprd1>, <oprd2>, ...)`

Invokes a function call by calling the specified call target.


coreVM Toolchain
----------------

The coreVM repo comes with a set of tools used for facilitating developments
and debugging when working with coreVM. These reside under the top level
`tools/ <https://github.com/yanzhengli/coreVM/tree/dev/tools>`_ directory.

Below are descriptions of each of the tools:

diff_benchmarks.py
^^^^^^^^^^^^^^^^^^

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
^^^^^^^^^^^^^^^^

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
^^^^^^

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
^^^^^^

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

    module name : Dummy_IR
    format version : v0.1.0
    target version : v0.1.0
    path : ./dummy_ir.ir
    author : Yanzheng Li
    timestamp : 1472959465

    structure Person {
        string name
        ui8 age
        Person* sibling
        Location* address
    }

    structure Location {
        string street_address
        string* country
        string zipcode
    }

    def structtype* find_friends(structtype* person, ui32 id) {
    entry:
        %sibling = alloca ui8 16, ui8 16 
        br ui32 32, %cond1 %cond2  [ label #br1, label #br2, label #br3 ]
    cond1:
        %name = getattr string "name", ui8 16 
    }

    def string compute(ui32 lhs_val, dpf rhs_val, array* values) :find_friends {
    entry:
        %sum = add ui8 16, %lhs_val %rhs_val 
        store string "val", array [ 6 * i32 ] %values 
    }

ir_asm
^^^^^^

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
^^^^^^^^

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

ir_tools_integrity.py
^^^^^^^^^^^^^^^^^^^^^

Tests the integrity between the coreVM IR assembler and disassembler.

Usage:

  .. code::

    python tools/ir_tools_integrity.py

Example:

  .. code::

    python tools/ir_tools_integrity.py

    Running IR tools integrity checker...
    python tools/ir_gen.py --output sample.ir
    ./build/tools/ir_dis --input sample.ir --output sample.ir.txt
    ./build/tools/ir_asm --input sample.ir.txt --output sample2.ir
    ./build/tools/ir_dis --input sample2.ir --output sample2.ir.txt
    diff sample.ir.txt sample2.ir.txt
    PASS
