.. Copyright Yanzheng Li. All rights reserved.

Reference Manual
================

This reference manual contains useful information for developers who wish to
make use of coreVM's functionalities.


Bytecode Format and Encoding
----------------------------

coreVM employs a binary encoding format that is versioned and structured.
Internally referred as "structured bytecode", the encoding scheme makes both
encoding and decoding straightforward while aiming to encode bytecode data in an
efficient manner.

First of all, coreVM relies on the `Apache Avro data serialization system <https://avro.apache.org/docs/current/>`_
for bytecode serialization and deserialization, which offers a diverse range of
language supports, including C++, Java, and Python. This offers tremendous
flexibilities to language developers. For example, the bytecode decoder in
coreVM is written in C++, and the Python compiler in
`Project Pya <roadmap.html#project-pyta>`_ is implemented in Python.

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

Below are explanations on the fields in the schema.

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

The name of the author whom created this bytecode.


**Field "string_literal_table"**

An array of string literals used in this bytecode.


**Field "fpt_literal_table"**

An array of floating-point literals used in this bytecode.


**Field "__MAIN__"**

Highest level of bytecode execution related data. An array of "closures".


**Field "__MAIN__.name"**

Name of a closure.


**Field "__MAIN__.id"**

Integer identifier of a closure that uniquely identifies itself in the bytecode.


**Field "__MAIN__.parent_id"**

Optional integer identifier of a closure's parent in the bytecode.


**Field "__MAIN__.vector"**

An array of instructions of a code block.


**Field "__MAIN__.vector.code"**

Integer code of an instruction. Please see the "Instruction Set" section below
for more details regarding the coreVM instruction set.


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
  form that cover a huge range of functionalities. Each instruction is
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
  * :ref:`string-type-instructions`
  * :ref:`array-type-instructions`
  * :ref:`map-type-instructions`


.. _object-instructions:

Object instructions
^^^^^^^^^^^^^^^^^^^

Instructions that interact with dynamic objects.

.. table::

  ============  ========  ============  ===============
    Mnemonic      Code      Operands      Description
  ============  ========  ============  ===============
  NEW           0         0             Creates a new object and place it on top of the stack.
  LDOBJ         1         1             Load an object by its key and push it onto stack.
  STOBJ         2         1             Pops the object on top of the stack and stores it with a key into the frame.
  STOBJN        3         2             Pops the object on top of the stack and stores it with a key into the `n`-th frame on the call stack from the top. A value of 0 means the top frame.
  GETATTR       4         1             Pop the object at the top of the stack, get its attribute and push it onto the stack.
  SETATTR       5         1             Pop the object at the top of the stack as the attribute, pop the next object as the target, and sets the attribute on the target.
  DELATTR       6         1             Pop the object at the top of the stack, and deletes its attribute and push it back onto the stack.
  HASATTR2      7         0             Determines if the object on top of the stack has an attribute, with the attribute name being the string value of the element on top of the eval stack. Places the result on top of the eval stack.
  GETATTR2      8         0             Gets an attribute from the object on top of the stack, with the attribute ame being the string value of the element on top of the eval stack. Pops the object off the stack and places the result on top of the stack.
  SETATTR2      9         0             Pop the object at the top of the stack as the attribtue value, and set it as an attribute value on the next object on top of the stack, using the attribute name that is the string value of the element on top of the eval stack.
  DELATTR2      10        0             Deletes an attribute from the object on top of the stack, with the attribute name being the string value of the element on top of the eval stack.
  POP           11        0             Pops the object on top of the stack.
  LDOBJ2        12        1             Load an invisible object by a key and push it onto the stack.
  STOBJ2        13        1             Pops the object on top of the stack and stores it with a key into the frame as an invisible object.
  DELOBJ        14        1             Deletes an object from the current scope.
  DELOBJ2       15        1             Deletes an invisible object from the current scope.
  GETHNDL       16        0             Copies the native handle of the top object of the stack and push it on top of the eval-stack.
  SETHNDL       17        0             Pops off the native handle off the eval-stack and assigns it to the top object of the stack.
  GETHNDL2      18        1             Copies of the native type handle of the named object in the current frame, and pushes it on top of the eval stack.
  CLRHNDL       19        0             Clears the native handle from the top object of the stack.
  CPYHNDL       20        1             Copies the native type handle associated from the object on top of the stack onto the next object on the stack. The first operand is a value specifying the type of conversion to perform on the native type handle copied.
  CPYREPR       21        0             Copies the string representation of the native type handle from the object on top of the stack onto the next object onto the stack.
  ISTRUTHY      22        0             Computes the truthy value of the native type handle associated with the object on top of the stack, and push the result on top of the eval stack.
  OBJEQ         23        0             Pops off the top two objects on the stack and tests if they are the same object.
  OBJNEQ        24        0             Pops off the top two objects on the stack and tests if they are different objects.
  SETCTX        25        1             Sets the closure context of the object. The first operand is the closure ID.
  CLDOBJ        26        2             Conditionally loads an object associated with the variable key value represented by either `oprd1` or `oprd2`, by evaluating the boolean equivalent of the object on top of the evaluation stack. Loads `oprd1` if the value evaluates to true, `oprd2` otherwise.
  SETATTRS      27        2             Pops off the object on top of the stack, and convert its native type handle to a native map. Then use its key-value pairs as attribute name-value pairs to set on the next object on the top of the stack. The first operand is a boolean value specifying whether each mapped object should be cloned before set on the target object. The second operand is a boolean value indicating if the native map values should be overriden with the cloned object IDs.
  RSETATTRS     28        1             Reverse set attributes. Set the object on top of stack as the attribute values onto the objects pointed to as values in the native map equivalent on top of the eval stack.
  SETATTRS2     29        1             Pops off the object on top of the stack, and set copies of all of its attributes onto the next on the stack. For each of the copied objects, set the second object on the stack as an attribute using the first operand as the attribute key.
  PUTOBJ        30        0             Pops the object on top of the stack, and pushes its value onto the top of the current evaluation stack.
  GETOBJ        31        0             Pops the top of the eval stack, and put its value on the object stack.
  SWAP          32        0             Swaps the top two objects on top of the stack.
  SETFLGC       33        1             Sets the `IS_NOT_GARBAGE_COLLECTIBLE` flag on the object on top of the stack. The first operand is a boolean value used to set the value of the flag. A value of `1` sets the flag, `0` otherwise.
  SETFLDEL      34        1             Sets the `IS_INDELIBLE` flag on the object on top of the stack. The first operand is a boolean vlaue used to set the value of the flag. A value of `1` sets the flag, `0` otherwise.
  SETFLCALL     35        1             Sets the `IS_NON_CALLABLE` flag on the object on top of the stack. The first operand is a boolean value used to set the value of the flag. A value of `1` sets the flag, `0` otherwise.
  SETFLMUTE     36        1             Sets the `IS_IMMUTABLE` flag on the object on top of the stack. The first operand is a boolean value used to set the value of the flag. A value of `1` sets the flag, `0` otherwise.
  ============  ========  ============  ===============


.. _control-instructions:

Control instructions
^^^^^^^^^^^^^^^^^^^^

Instructions that direct control flow of executions.

.. table::

  ============  ========  ============  ===============
    Mnemonic      Code      Operands      Description
  ============  ========  ============  ===============
  PINVK         37        0             Prepares the invocation of a function. Creates a new frame on top of the call stack, and sets its closure context using the context of the object on top of the stack.
  INVK          38        0             Invokes the vector of the object on top of the stack.
  RTRN          39        0             Unwinds from the current call frame and jumps to the previous one.
  JMP           40        1             Unconditionally jumps to a particular instruction address.
  JMPIF         41        1             Conditionally jumps to a particular instruction address only if the top element on the eval stacks evaluates to True.
  JMPR          42        1             Unconditionally jumps to an instruction with an offset starting from the beginning of the current frame.
  EXC           43        1             Pop the object at the top and raise it as an exception. The first operand is a boolean value indicating whether the runtime should search for a catch site in the current closure. A value of `false` will make the runtime pop the current frame.
  EXCOBJ        44        0             Gets the exception object associated with the current frame, and pushes it on top of the stack.
  CLREXC        45        0             Clears the exception object associated with the frame on top of the call stack.
  JMPEXC        46        2             Jumps to the specified address, based on the state of the exception object associated with the frame on top of the call stack. The first operand is the number of addresses to jump over starting from the current program counter. The second operand specifies whether or not to jump based on if the top of stack frame has an exception object. A value of `1` specifies the jump if the frame has an exception object, `0` otherwise.
  EXIT          47        1             Halts the execution of instructions and exits the program (with an optional exit code).
  ============  ========  ============  ===============


.. _function-instructions:

Function instructions
^^^^^^^^^^^^^^^^^^^^^

Instructions related to functions and call invocations.

.. table::

  ============  ========  ============  ===============
    Mnemonic      Code      Operands      Description
  ============  ========  ============  ===============
  PUTARG        48        0             Pops the top object off the stack and assign it as the next argument for the next call.
  PUTKWARG      49        1             Pops the top object off the stack and assign it as the next keyword-argument for the next call.
  PUTARGS       50        0             Pops the top object off the stack, retrieves its native type handle as a native type array, and then iterate through each array element, use it as an object ID to retrieve an object from the heap, and assigns it as the next argument for the next call.
  PUTKWARGS     51        0             Pops the top object off the stack, retrieves its native type handle as a native type map, and then iterate through each key-value pair, use the value as an object ID to retrieve an object from the heap, and use the key as an encoding ID to assign the object as the next keyword-argument for the next call.
  GETARG        52        1             Pops off the first argument for the current call and put it on the current frame using the encoding key specified in the first operand.
  GETKWARG      53        2             If the top frame has the keyword-argument pair with the key specified as the first operand, pops off the pair and stores the value into the frame using the key. And, advance the program counter by the value specified in the second operand.
  GETARGS       54        0             Pops off all the arguments for the current call, insert them into a native-list and push it on top of eval-stack.
  GETKWARGS     55        0             Pops off all the keyword-arguments for the current call, insert them into a native-map and push it on top of eval-stack.
  HASARGS       56        0             Determines if there are any arguments remaining on the current frame, and pushes the result onto the top of the eval stack.
  ============  ========  ============  ===============


.. _runtime-instructions:

Runtime instructions
^^^^^^^^^^^^^^^^^^^^

Instructions related to a wide range of runtime functionalities.

.. table::

  ============  ========  ============  ===============
    Mnemonic      Code      Operands      Description
  ============  ========  ============  ===============
  GC            57        0             Manually performs garbage collection.
  DEBUG         58        1             Show debug information. The first operand is the set of debug options: 1. Show instructions in canonical form.
  DBGFRM        59        1             Show debug information on the current frame. The first operand is the set of debug options: 1. Show instructions in canonical form.
  DBGMEM        60        1             Show information of current process memory usages. The first operand is the set of options: 1. Show peak virtual memory size and resident set size.
  DBGVAR        61        1             Show information of a variable.
  PRINT         62        2             Converts the native type handle associated with the object on top of the stack into a native string, and prints it to std output. The second operand is a boolean value specifying whether a trailing new line character should be printed. Defaults to `false`.
  SWAP2         63        0             Swaps the top two elements on the evaluation stack.
  ============  ========  ============  ===============


.. _arithmetic-and-logic-instructions:

Arithmetic and logic instructions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instructions that deal with arithmetic and logical operations.

.. table::

  ============  ========  ============  ===============
    Mnemonic      Code      Operands      Description
  ============  ========  ============  ===============
  POS           64        0             Apply the positive operation on the top element on the evaluation stack.
  NEG           65        0             Apply the negation operation on the top element on the evaluation stack.
  INC           66        0             Apply the increment operation on the top element on the evaluation stack.
  DEC           67        0             Apply the decrement operation on the top element on the evaluation stack.
  ABS           68        0             Apply the `abs` operation on the top element on the evaluation stack.
  SQRT          69        0             Apply the `sqrt` operation on the top element on the evaluation stack.
  ADD           70        0             Pops the top two elements on the eval stack, applies the addition operation and push result onto eval stack.
  SUB           71        0             Pops the top two elements on the eval stack, applies the subtraction operation and push result onto eval stack.
  MUL           72        0             Pops the top two elements on the eval stack, applies the multiplication operation and push result onto eval stack.
  DIV           73        0             Pops the top two elements on the eval stack, applies the division operation and push result onto eval stack.
  MOD           74        0             Pops the top two elements on the eval stack, applies the modulus operation and push result onto eval stack.
  POW           75        0             Pops the top two elements on the eval stack, applies the power operation and push result onto eval stack.
  BNOT          76        0             Applies the bitwise NOT operation on the top element on the evaluation stack.
  BAND          77        0             Pops the top two elements on the eval stack, applies the bitwise AND operation and push result onto eval stack.
  BOR           78        0             Pops the top two elements on the eval stack, applies the bitwise OR operation and push result onto eval stack.
  BXOR          79        0             Pops the top two elements on the eval stack, applies the bitwise XOR operation and push result onto eval stack.
  BLS           80        0             Pops the top two elements on the eval stack, applies the bitwise left shift operation and push result onto eval stack.
  BRS           81        0             Pops the top two elements on the eval stack, applies the bitwise right shift operation and push result onto eval stack.
  EQ            82        0             Pops the top two elements on the eval stack, applies the equality operation and push result onto eval stack.
  NEQ           83        0             Pops the top two elements on the eval stack, applies the inequality operation and push result onto eval stack.
  GT            84        0             Pops the top two elements on the eval stack, applies the greater than operation and push result onto eval stack.
  LT            85        0             Pops the top two elements on the eval stack, applies the less than operation and push result onto eval stack.
  GTE           86        0             Pops the top two elements on the eval stack, applies the greater or equality operation and push result onto eval stack.
  LTE           87        0             Pops the top two elements on the eval stack, applies the less or equality operation and push result onto eval stack.
  LNOT          88        0             Apply the logic NOT operation on the top element on the evaluation stack.
  LAND          89        0             Pops the top two elements on the eval stack, applies the logical AND operation and push result onto eval stack.
  LOR           90        0             Pops the top two elements on the eval stack, applies the logical OR operation and push result onto eval stack.
  CMP           91        0             Pops the top two elements on the eval stack, applies the "cmp" operation and push result onto eval stack.
  ============  ========  ============  ===============


.. _native-type-creation-instructions:

Native type creation instructions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instructions for creating native type handles.

.. table::

  ============  ========  ============  ===============
    Mnemonic      Code      Operands      Description
  ============  ========  ============  ===============
  INT8          92        1             Creates an instance of type `int8` and place it on top of eval stack.
  UINT8         93        1             Creates an instance of type `uint8` and place it on top of eval stack.
  INT16         94        1             Creates an instance of type `int16` and place it on top of eval stack.
  UINT16        95        1             Creates an instance of type `uint16` and place it on top of eval stack.
  INT32         96        1             Creates an instance of type `int32` and place it on top of eval stack.
  UINT32        97        1             Creates an instance of type `uint32` and place it on top of eval stack.
  INT64         98        1             Creates an instance of type `int64` and place it on top of eval stack.
  UINT64        99        1             Creates an instance of type `uint64` and place it on top of eval stack.
  BOOL          100       1             Creates an instance of type `bool` and place it on top of eval stack.
  DEC1          101       1             Creates an instance of type `dec` and place it on top of eval stack. The first operand represents the index of the floating-point literal stored in the corresponding compartment.
  DEC2          102       1             Creates an instance of type `dec2` and place it on top of eval stack. The first operand represents the index of the floating-point literal stored in the corresponding compartment.
  STR           103       1             Creates an instance of type `str` and place it on top of eval stack.
  ARY           104       0             Creates an instance of type `array` and place it on top of eval stack.
  MAP           105       0             Creates an instance of type `map` and place it on top of eval stack.
  ============  ========  ============  ===============


.. _native-type-conversion-instructions:

Native type conversion instructions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instructions for conversions between native type handles.

.. table::

  ============  ========  ============  ===============
    Mnemonic      Code      Operands      Description
  ============  ========  ============  ===============
  TOINT8        106       0             Converts the element on top of the eval stack to type `int8`.
  TOUINT8       107       0             Converts the element on top of the eval stack to type `uint8`.
  TOINT16       108       0             Converts the element on top of the eval stack to type `int16`.
  TOUINT16      109       0             Converts the element on top of the eval stack to type `uint16`.
  TOINT32       110       0             Converts the element on top of the eval stack to type `int32`.
  TOUINT32      111       0             Converts the element on top of the eval stack to type `uint32`.
  TOINT64       112       0             Converts the element on top of the eval stack to type `int64`.
  TOUINT64      113       0             Converts the element on top of the eval stack to type `uint64`.
  TOBOOL        114       0             Converts the element on top of the eval stack to type `bool`.
  TODEC1        115       0             Converts the element on top of the eval stack to type `dec`.
  TODEC2        116       0             Converts the element on top of the eval stack to type `dec2`
  TOSTR         117       0             Converts the element on top of the eval stack to type `string`.
  TOARY         118       0             Converts the element on top of the eval stack to type `array`.
  TOMAP         119       0             Converts the element on top of the eval stack to type `map`.
  ============  ========  ============  ===============


.. _native-type-manipulation-instructions:

Native type manipulation instructions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instructions for manipulating native type handles.

.. table::

  ============  ========  ============  ===============
    Mnemonic      Code      Operands      Description
  ============  ========  ============  ===============
  TRUTHY        120       0             Computes a boolean truthy value based on the top element on the eval stack, and puts it on top of the stack.
  REPR          121       0             Computes the string equivalent representation of the element on top of the eval stack, and push it on top of the stack.
  HASH          122       0             Computes the non-crytographic hash value of the element on top of the eval stack, and push the result on top of the eval stack.
  SLICE         123       0             Computes the portion of the element on the top 3rd element of the eval stack as a sequence, using the 2nd and 1st top elements as the `start` and `stop` values as the indices range [start, stop).
  STRIDE        124       0             Computes a new sequence of the element on the 2nd top eval stack as a sequence, using the top element as the `stride` interval.
  REVERSE       125       0             Computes the reverse of the element on top of the eval stack as a sequence.
  ROUND         126       0             Rounds the second element on top of the eval stack using the number converted from the element on top of the eval stack.
  ============  ========  ============  ===============


.. _string-type-instructions:

String type instructions
^^^^^^^^^^^^^^^^^^^^^^^^

Instructions for manipulating native type handles of the native string type.

.. table::

  ============  ========  ============  ===============
    Mnemonic      Code      Operands      Description
  ============  ========  ============  ===============
  STRLEN        127       0             Pops the top element on the eval stack, and performs the "string size" operation.
  STRAT         128       0             Pops the top two elements on the eval stack, and performs the "string at" operation.
  STRCLR        129       0             Pops the top element on the eval stack, and performs the "string clear" operation.
  STRAPD        130       0             Pops the top two elements on the eval stack, and performs the "string append" operation.
  STRPSH        131       0             Pops the top two elements on the eval stack, and performs the "string pushback" operation.
  STRIST        132       0             Pops the top three elements on the eval stack, and performs the "string insertion" operation.
  STRIST2       133       0             Pops the top three elements on the eval stack, and performs the "string insertion" operation.
  STRERS        134       0             Pops the top two elements on the eval stack, and performs the "string erase" operation.
  STRERS2       135       0             Pops the top two elements on the eval stack, and performs the "string erase" operation.
  STRRPLC       136       0             Pops the top four elements on the eval stack, and performs the "string replace" operation.
  STRSWP        137       0             Pops the top two elements on the eval stack, and performs the "string swap" operation.
  STRSUB        138       0             Pops the top two elements on the eval stack, and performs the "string substring" operation.
  STRSUB2       139       0             Pops the top three elements on the eval stack, and performs the "string substring" operation.
  STRFND        140       0             Pops the top two elements on the eval stack, and performs the "string find" operation.
  STRFND2       141       0             Pops the top three elements on the eval stack, and performs the "string find" operation.
  STRRFND       142       0             Pops the top two elements on the eval stack, and performs the "string rfind" operation.
  STRRFND2      143       0             Pops the top three elements on the eval stack, and performs the "string rfind2" operation.
  ============  ========  ============  ===============


.. _array-type-instructions:

Array type instructions
^^^^^^^^^^^^^^^^^^^^^^^

Instructions for manipulating native type handles of the native array type.

.. table::

  ============  ========  ============  ===============
    Mnemonic      Code      Operands      Description
  ============  ========  ============  ===============
  ARYLEN        144       0             Pops the top element on the eval stack, and performs the "array size" operation.
  ARYEMP        145       0             Pops the top element on the eval stack, and performs the "array empty" operation.
  ARYAT         146       0             Pops the top two elements on the eval stack, and performs the "array at" operation.
  ARYFRT        147       0             Pops the top element on the eval stack, and performs the "array front" operation.
  ARYBAK        148       0             Pops the top element on the eval stack, and performs the "array back" operation.
  ARYPUT        149       0             Pops the top three elements on the eval stack, and performs the "array put" operation.
  ARYAPND       150       0             Pops the top two elements on the eval stack, and performs the "array append" operation.
  ARYERS        151       0             Pop the top two elements on the eval stack, and performs the "array erase" operation.
  ARYPOP        152       0             Pops the top element on the eval stack, and performs the "array pop" operation.
  ARYSWP        153       0             Pops the top two elements on the eval stack, and performs the "array swap" operation.
  ARYCLR        154       0             Pops the top element on the eval stack, and performs the "array clear" operation.
  ARYMRG        155       0             Pops the top two elements on the eval stack, converts them to arrays, merge them into one single array, and put it back to the eval stack.
  ============  ========  ============  ===============


.. _map-type-instructions:

Map type instructions
^^^^^^^^^^^^^^^^^^^^^

Instructions for manipulating native type handles of the native map type.

.. table::

  ============  ========  ============  ===============
    Mnemonic      Code      Operands      Description
  ============  ========  ============  ===============
  MAPLEN        156       0             Pops the top element on the eval stack, and performs the "map size" operation.
  MAPEMP        157       0             Pops the top element on the eval stack, and performs the "map empty" operation.
  MAPFIND       158       0             Pops the top two elements on the eval stack, and performs the "map find" operation.
  MAPAT         159       0             Pops the top two elements on the eval stack, and performs the "map at" operation.
  MAPPUT        160       0             Pops the top three elements on the eval stack, and performs the "map put" operation.
  MAPSET        161       1             Converts the top element on the eval stack to a native map, and insert a key-value pair into it, with the key represented as the first operand, and the value as the object on top of the stack.
  MAPERS        162       0             Pops the top element on the eval stack, and performs the "map erase" operation.
  MAPCLR        163       0             Pops the top element on the eval stack, and performs the "map clear" operation.
  MAPSWP        164       0             Pops the top two elements on the eval stack, and performs the "map swap" operation.
  MAPKEYS       165       0             Inserts the keys of the map on top of the eval stack into an array, and place it on top of the eval stack.
  MAPVALS       166       0             Inserts the values of the map on top of the eval stack into an array, and place it on top of the eval stack.
  MAPMRG        167       0             Pops the top two elements on the eval stack, converts them to maps, merge them into one single map, and put it back to the eval stack.
  ============  ========  ============  ===============


----


APIs
----

The coreVM library provides a set of powerful APIs that offer additional
capabilities beyond the functionalities from the instruction set. They provide
greater flexibilities and more granular controls to the execution of bytecodes
to developers.

The library is consisted of the following APIs:

  * :ref:`entry-api`
  * Debugging and Profiling API (coming soon)
  * Embedder API (coming soon)
  * Extension API (coming soon)
  * Threading API (coming soon)


.. _entry-api:

Entry API
^^^^^^^^^

The *Entry API* is the gateway to coreVM's fundamental functionalities.


**Bytecode execution configuration**

Header: `corevm/api/configuration.h`

.. cpp:class:: corevm::api::Configuration

  An encapsulation of a set of configuration parameters for bytecode execution.

  .. cpp:function:: Configuration()
    :noindex:

    Constructor.

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

  .. cpp:function:: void set_log_mode(const std::string&)
    :noindex:

    Sets the logging mode. A default value is used if not specified.

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

    Gets the the logging mode.


**Invoking bytecode execution**

Header: `corevm/api/entry.h`

.. cpp:function:: int corevm::api::invoke_from_file(const char* filepath, const Configuration& config)
  
  Executes the bytecode stored in `filepath`, along with the specified
  configuration object.

  Returns 0 on successful execution, non-zero values otherwise.
