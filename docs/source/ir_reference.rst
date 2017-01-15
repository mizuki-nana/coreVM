.. Copyright Yanzheng Li. All rights reserved.

===================================================
coreVM Intermediate Representation Reference Manual
===================================================

********
Abstract
********

This document is a reference manual for the coreVM Intermediate Representation,
an abstract language that can represent the imperative and declarative semantics
of most programming languages.


********
Overview
********

The coreVM Intermediate Representation, also known as *coreVM IR* for short, is
a `Static Single Assignment <https://en.wikipedia.org/wiki/Static_single_assignment_form>`_
(SSA) based language representation that can capture the constructs and
semantics of programming languages in a high-level and generalized form.
Its representation is consisted of both statically defined types and constructs,
as well as abstractions for dynamic types and constructs that can only be
resolved during runtime. Compared to some other language intermediate
representations, such as LLVM IR, coreVM's syntax and semantics are much
simpler, and consequently it is not designed to capture all the low-level
constructs. The coreVM IR is designed this way such that it opens up
opportunities for interpreted source code executions, compilations, and
just-in-time compilations in various scenarios. Moreover, its flexible language
semantics makes it possible to integrate with processes such as profile-guided
type inference, representation lowering, and other techniques for both online
and offline source code compilation and execution.

coreVM IR's format can be represented in two forms: a binary encoding format
and a textual representation format designed for readable human interpretations.
Please refer to the `coreVM Toolchain <reference.html#corevm-toolchain>`_
section to learn more about the tools for IR format conversions.


***********************
IR Format and Structure
***********************

IR Format Schema
================

coreVM IR's semantics is defined as a schema based on the
`Apache Avro data serialization system <https://avro.apache.org/docs/current/>`_.
Serialization and de-serialization of its binary form are also based on the
mechanisms provided by the system.

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
              "type": "long"
            },
            {
              "name": "target_version",
              "type": "long"
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
            "name": "IRTypeDecl",
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
                    "name": "IRTypeField",
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
                        "type": {
                          "type": "record",
                          "name": "IRIdentifierType",
                          "fields": [
                            {
                              "name": "type",
                              "type": {
                                "type": "enum",
                                "name": "IRIdentifierTypeType",
                                "symbols": [
                                  "IdentifierType_Identifier",
                                  "IdentifierType_Array",
                                  "IdentifierType_ValueType"
                                ]
                              }
                            },
                            {
                              "name": "value",
                              "type": [
                                "string",
                                {
                                  "type": "record",
                                  "name": "IRArrayType",
                                  "fields": [
                                    {
                                      "type": "corevm.ir.IRIdentifierType",
                                      "name": "type"
                                    },
                                    {
                                      "type": "int",
                                      "name": "len"
                                    }
                                  ]
                                },
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
                                    "object"
                                  ]
                                }
                              ]
                            }
                          ]
                        }
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
                "type": "corevm.ir.IRIdentifierType"
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
                        "type": "corevm.ir.IRIdentifierType"
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
                                "name": "type",
                                "type": [
                                  "null",
                                  "corevm.ir.IRIdentifierType"
                                ]
                              },
                              {
                                "name": "options",
                                "type": {
                                  "type": "array",
                                  "items": {
                                    "type": "string"
                                  }
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
                                          "string",
                                          {
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
                                                  "string"
                                                ]
                                              }
                                            ]
                                          }
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

----

IR Format Structure
===================

This section describes the entities defined in the coreVM IR schema.

Entity 'IRModule'
-----------------

Highest level of entity in IR, encapsulates all the data and metadata associated
with a module, which corresponds to a physical translation unit.

.. table::

  ==============  =====================  ======================================
      Field               Type                        Description
  ==============  =====================  ======================================
    `meta`          `IRModuleMeta`         Set of metadata of the module.
    `types`         set<`IRTypeDecl`>      Set of type definitions.
    `closures`      set<`IRClosure`>       Set of function definitions.
  ==============  =====================  ======================================

Entity 'IRModuleMeta'
---------------------

Entity that captures all the metadata of a module. Metadata are records of datum
that are not needed for any processes to be performed on the module, but they
are essential for capturing the source of origin as well as information for
version compatibility checks.

Below are the allowed fields encapsulated in the metadata:

.. table::

  ====================  ==========  ===================================================
    Field                  Type          Description
  ====================  ==========  ===================================================
    `name`                string      Name of module.
    `format_version`      long        Version of the IR format.
    `target_version`      long        Target version of coreVM.
    `path`                string      Absolute file path of module.
    `author`              string      Author of module.
    `timestamp`           long        UNIX timestamp of when the module was authored.
  ====================  ==========  ===================================================

Entity 'IRTypeDecl'
-------------------

Represents a type definition.

.. table::

  ============  ======================  ===========================================
     Field               Type                          Description
  ============  ======================  ===========================================
    `name`        string                  Name of type definition.
    `fields`      set<`IRTypeField`>      Set of fields encapsulated in the type.
  ============  ======================  ===========================================

Entity 'IRTypeField'
--------------------

Represents a single field in a type definition.

.. table::

  ================  =====================  ================================
    Field                   Type               Description
  ================  =====================  ================================
    `identifier`      string                 Name of field.
    `ref_type`        `IRValueRefType`       Reference type of the field.
    `type`            `IRIdentifierType`     Type of the field.
  ================  =====================  ================================

Enumeration 'IRValueRefType'
----------------------------

Represents a set of ways that a value can be referenced. Currently a value can
be referenced either via by-value or by-pointer.

.. table::

  =============  =================================
       Type             Description
  =============  =================================
    `value`         Reference-by-value.
    `pointer`       Reference-by-pointer.
  =============  =================================

Enumeration 'IRValueType'
-------------------------

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
    `object`        Dynamic object type.
  ==============  ========================================

Entity 'IRArrayType'
--------------------

Represents a fixed size array type. The entity is consisted of the type of the
encapsulated elements, as well as the size of the array.

.. table::

  ==========  ======================  ======================================
    Field             Type                         Description
  ==========  ======================  ======================================
    `type`      `IRIdentifierType`      Type of the encapsulated elements.
    `len`       long                    Number of elements in the array.
  ==========  ======================  ======================================

enum 'IRIdentifierTypeType'
---------------------------

Represents the type of an identifiable type. Can be either a custom type,
primitive type, or array type.

.. table::

  ===============================  =====================================
       Type                                    Description
  ===============================  =====================================
    `IdentifierType_Identifier`      Represents a custom type.
    `IdentifierType_Array`           Represents an array type.
    `IdentifierType_ValueType`       Represents a constant value type.
  ===============================  =====================================

Entity 'IRIdentifierType'
-------------------------

Represents a type that can be associated with an identifier. Can be either a
custom type, primitive type, or array type.

.. table::

  ===========  =========================  ================================================================================
     Field               Type                                              Description
  ===========  =========================  ================================================================================
    `type`      `IRIdentifierTypeType`      Type of the type represented (i.e. custom type, primitive type, array type).
    `value`     Unioned structure.          Representation of the underlying type.
  ===========  =========================  ================================================================================

Entity 'IRClosure'
------------------

Represents a scoped function declaration. Closures allow function declarations
to be hierarchically scoped.

.. table::

  =================  =======================  ================================================
        Field                 Type                              Description
  =================  =======================  ================================================
    `name`             string                   Name of the function.
    `parent`           Unioned structure.       Optional parent closure.
    `rettype`          `IRIdentifierType`       Type of the function return value.
    `ret_reftype`      `IRValueRefType`         Reference type of the function return value.
    `parameters`       set<`IRParameter`>       A set of parameters of the function.
    `blocks`           set<`IRBasicBlock`>      A set of basic blocks in the function.
  =================  =======================  ================================================

Entity 'IRParameter'
--------------------

Represents a function parameter.

.. table::

  ================  ======================  ====================================
       Field                 Type                       Description
  ================  ======================  ====================================
    `identifier`      string                  Name of the parameter.
    `ref_type`        `IRValueRefType`        Reference type of the parameter.
    `type`            `IRIdentifierType`      Type of the parameter.
  ================  ======================  ====================================

Entity 'IRBasicBlock'
---------------------

Represents a basic block within a function. Each basic block is uniquely
identified by its label. A function body can be consisted of one or multiple
basic blocks.

.. table::

  ===========  ========================  ===============================================
     Field               Type                             Description
  ===========  ========================  ===============================================
    `label`      string                    Label that identifies the basic block.
    `body`       set<`IRInstruction`>      Set of instructions under this basic block.
  ===========  ========================  ===============================================

Entity 'IRInstruction'
----------------------

Represents a single instruction statement. Each instruction is consisted of an
opcode, an optional instruction value type, an optional target (for instructions
that return values), an optional set of operands, and an optional set of label
locations.

.. table::

  ============  ======================  =====================================
     Field               Type                        Description
  ============  ======================  =====================================
    `target`      string                  Optional assignment target.
    `opcode`      `IROpcode`              Opcode of the instruction.
    `type`        `IRIdentifierType`      Optional type of the instruction.
    `options`     set<string>             Set of instruction options.
    `oprds`       set<`IROperand`>        Set of instruction operands.
    `labels`      set<`IRLabel`>          Set of instruction labels.
  ============  ======================  =====================================

Enumeration 'IROpcode'
----------------------

Represents the set of opcodes defined in the IR. Please refer to the
:ref:`ir-instruction-set`.

Entity 'IRValue'
----------------

Represents a constant value.

.. table::

  ===========  ======================  =========================
     Field              Type                  Description
  ===========  ======================  =========================
    `type`       `IRValueType`           Type of the value.
    `value`      Unioned structure.      The underlying value.
  ===========  ======================  =========================

Enumeration 'IROperand'
-----------------------

Represents an operand in an instruction statement.

.. table::

  ============  ======================  ===============================
     Field               Type                     Description
  ============  ======================  ===============================
    `type`        `IRIdentifierType`      Type of the operand.
    `value`       Unioned structure.      Typed value of the operand.
  ============  ======================  ===============================

Enumeration 'IROperandType'
---------------------------

Type of an instruction operand, can be either a "variable" reference or a
constant.

.. table::

  ==============  ==============================
       Type         Description
  ==============  ==============================
    `constant`      Literal constant.
    `ref`           Variable reference.
  ==============  ==============================

Entity 'IRLabel'
----------------

Represents a labeled jump location used in an instruction.

.. table::

  ==========  ==========  ======================
    Field        Type          Description
  ==========  ==========  ======================
    `name`      string      Name of the label.
  ==========  ==========  ======================

----

.. _ir-instruction-set:

******************
IR Instruction Set
******************

This section describes the IR's instruction set.


'alloca' Instruction
====================

Syntax:
-------

.. code-block:: none

    %target = alloca <type>

    %target = alloca [static|auto] <type>

Synopsis:
---------

Allocates memory and creates an instance of the specified type, and returns the
pointer of the created instance.

Object creation can be of two types. One is based on static allocation,
specified by the `static` keyword, in which memories are allocated on the stack,
and the lifetime of the object is bounded by its scope. The second type of
creation, specified by the `auto` keyword, is based on dynamically allocated
memories, and the object's lifespan is automatically managed by the runtime.


'load' Instruction
==================

Syntax:
-------

.. code-block:: none

    %target = load <type> %oprd

Synopsis:
---------

Reads content from memory, and converts to a target type.


'store' Instruction
===================

Syntax:
-------

.. code-block:: none

    store <type> <src> %dst

Synopsis:
---------

Writes content of the specified type from source to destination memory address.


'getattr' Instruction
=====================

Syntax:
-------

.. code-block:: none

    %target = getattr #constant-string %oprd

Synopsis:
---------

Retrieves the attribute value of an object and returns a pointer that references
the value.


'setattr' Instruction
=====================

Syntax:
-------

.. code-block:: none

    setattr #constant-string <src> %target-object

Synopsis:
---------

Sets the attribute value from source onto target object.


'delattr' Instruction
=====================

Syntax:
-------

.. code-block:: none

    delattr #constant-string %dst

Synopsis:
---------

Remove a particular attribute of a specified object.


'getelement' Instruction
========================

Syntax:
-------

.. code-block:: none

    %target = getelement <type> %src <idx>

Synopsis:
---------

Retrieves the element from an array with an index value.


'putelement' Instruction
========================

Syntax:
-------

.. code-block:: none

    putelement <src> %dst <idx>

Synopsis:
---------

Sets a specified element to an array at an index.


'len' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = len %oprd

Synopsis:
---------

Retrieves the length of an array. The result is of type `ui64`.


'ret' Instruction
=================

Syntax:
-------

.. code-block:: none

    ret <type> <oprd>

Synopsis:
---------

Return control flow (and optionally a value) from a function back to the caller.


'br' Instruction
================

Syntax:
-------

.. code-block:: none

    br <cond> [ label #iftrue, label #iffalse ]

Synopsis:
---------

Branches to either one of two labels depending on the boolean representation of
a conditional value.


'switch2' Instruction
=====================

Syntax:
-------

.. code-block:: none

    switch2 <value> <case1>, <case2>, .... [ label #case1, label #case2, ... ]

Synopsis:
---------

Jumps to one of a set of labels based on a target value, and a set of
predicates specified as the rest of the operands.

The number of *case* expressions to be evaluated against the target value must
equal to the number of labels specified. In the case that none of the predicates
evaluate to equate the target value, no branching occurs, and control flows to
the subsequent instruction, if there is one.

'pos' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = pos <type> <oprd>

Synopsis:
---------

Computes the positive expression of the specified operand.


'neg' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = neg <type> <oprd>

Synopsis:
---------

Computes the negative expression of the specified operand.


'inc' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = inc <type> <oprd>

Synopsis:
---------

Increases the value by 1 of the specified instruction.


'dec' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = dec <type> <oprd>

Synopsis:
---------

Decreases the value by 1 of the specified instruction.


'add' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = add <type> <oprd1> <oprd2>

Synopsis:
---------

Performs arithmetic addition of the values of two expressions.


'sub' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = sub <type> <oprd1> <oprd2>

Synopsis:
---------

Performs arithmetic subtraction of the values of two expressions.


'mul' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = mul <type> <oprd1> <oprd2>

Synopsis:
---------

Performs arithmetic multiplication of the values of two expressions.


'div' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = div <type> <oprd1> <oprd2>

Synopsis:
---------

Performs arithmetic division of the values of two expressions.


'mod' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = mod <type> <oprd1> <oprd2>

Synopsis:
---------

Performs arithmetic modulo operation of the values of two expressions.


'bnot' Instruction
==================

Syntax:
-------

.. code-block:: none

    %target = bnot <oprd>

Synopsis:
---------

Computes the bitwise NOT evaluation of a value. The result is of type `ui64`.


'band' Instruction
==================

Syntax:
-------

.. code-block:: none

    %target = band <oprd1> <oprd2>

Synopsis:
---------

Computes the bitwise AND evaluation of two operands. The result is of type
`ui64`.


'bor' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = bor <oprd1> <oprd2>

Synopsis:
---------

Computes the bitwise OR evaluation of two operands. Both operands must be of
integer type. The result is of type `ui64`.


'bxor' Instruction
==================

Syntax:
-------

.. code-block:: none

    %target = bxor <oprd1> <oprd2>

Synopsis:
---------

Computes the bitwise XOR evaluation of two operands. Both operands must be of
integer type. The result is of type `ui64`.


'bls' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = bls <type> <oprd>

Synopsis:
---------

Computes the bitwise-left-shift evaluation of the specified value. Both operands
must be of integer type. The result is of type `ui64`.


'brs' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = brs <type> <oprd>

Synopsis:
---------

Computes the bitwise-right-shift evaluation of the specified value. Both
operands must be of integer type. The result is of type `ui64`.


'eq' Instruction
================

Syntax:
-------

.. code-block:: none

    %target = eq <oprd1> <oprd2>

Synopsis:
---------

Computes the equality evaluation of two operands. The result is of type
`boolean`.


'neq' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = neq <oprd1> <oprd2>

Synopsis:
---------

Computes the non-equality evaluation of two operands. The result is of type
`boolean`.


'gt' Instruction
================

Syntax:
-------

.. code-block:: none

    %target = gt <oprd1> <oprd2>

Synopsis:
---------

Computes the greater-than evaluation of two operands. The result is of type
`boolean`.


'lt' Instruction
================

Syntax:
-------

.. code-block:: none

    %target = lt <oprd1> <oprd2>

Synopsis:
---------

Computes the less-than evaluation of two operands. The result is of type
`boolean`.


'gte' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = gte <oprd1> <oprd2>

Synopsis:
---------

Computes the greater-or-equal-to evaluation of two operands. The result is of
type `boolean`.


'lte' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = lte <oprd1> <oprd2>

Synopsis:
---------

Computes the less-or-equal-to evaluation of two operands. The result is of type
`boolean`.


'lnot' Instruction
==================

Syntax:
-------

.. code-block:: none

    %target = lnot <oprd>

Synopsis:
---------

Computes the logical NOT evaluation of a value. The result is of type `boolean`.


'land' Instruction
==================

Syntax:
-------

.. code-block:: none

    %target = land <oprd1> <oprd2>

Synopsis:
---------

Computes the logical AND evaluation of two operands. The result is of type
`boolean`.


'lor' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = lor <oprd1> <oprd2>

Synopsis:
---------

Computes the logical OR evaluation of two operands. The result is of type
`boolean`.


'cmp' Instruction
=================

Syntax:
-------

.. code-block:: none

    %target = cmp <oprd1> <oprd2>

Synopsis:
---------

Equality comparison between two operands. Results in `-1` if the left-hand-side
is considered less than the right-hand-side, `0` if they are evaluated to be
equal, and `1` otherwise. The result is of type `i32`.


'call' Instruction
==================

Syntax:
-------

.. code-block:: none

    %target = call <ret-type> #call-target <arg1> <arg2> ...

Synopsis:
---------

Invokes a function call by calling the specified call target with a set of
arguments.


*****************************
Textual Representation Syntax
*****************************

The coreVM IR has a readable textual representation that's meant for human
interpretations. It has a syntax that is based on a context-free grammar.

This section describes the key points in the textual representation syntax.

Metadata Syntax
===============

IR module metadata are key-value pairs that are defined at the beginning of a
module. Both keys and values are string identifiers that are surrounded by a
pair of double-quotes, and are separated by a colon.

For example:

.. code-block:: none

    "module name" : "Dummy_IR"
    "format version" : "1000"
    "target version" : "10"
    "path" : "./dummy_ir.ir"
    "author" : "Yanzheng Li"
    "timestamp" : "1472959465"


Type Definition Syntax
======================

Type definitions come after metadata definition. Each definition start with the
`"type"` keyword, followed by a pair of curly braces that surround a set of type
fields. Each type field is defined by the type of the field plus the identifier
of the field. Type fields are separated by semicolons.

Example:

.. code-block:: none

    type Person {
        string name;
        ui8 age;
        Person* sibling;
        Location* address;
        array [ 10 * Person ] friends;
    }


Array Type Syntax
=================

Array types are defined by the keyword `"array"` followed by a pair of square
brackets surrounding the number of elements in the array, a `'*'` character,
followed by the type of the array element.

For example:

.. code-block:: none

    array [ 10 * Person ]


Function Definition Syntax
==========================

Function definitions start with the keyword `"def"`, followed by the return type
of the function, then the name of the function, and then the set of parameters
of the function. Optionally, a scope parent can be specified following the
parameters.

Inside the body of a function, a set of basic blocks each start with the
identifier of the block, followed by one or more instructions.

For example:

.. code-block:: none

    def string compute(ui32 lhs_val, dpf rhs_val, array [ 4 * dpf ]* values) : createPerson {
    entry:
        %sum = add ui64 %lhs_val %rhs_val;
        putelement ui8 16 %values ui32 2;
    }


Instruction Syntax
==================

Each instruction has an optional assginment target. If one exists, an `'='`
character follows. Next is the opcode of the instruction, followed by an
optional type of the instruction, followed by the set of operands of the
instructions. Finally, an optional set of labels are surrounded by a pair of
square brackets.

Basic block labels are string identifiers that are prefixed with the character
`'#'`.

For example:

.. code-block:: none

    br %age [ label #end, label #end ];


Identifier Syntax
=================

Each identifier starts with the character `'%'`, followed by the name of the
identifier. In SSA form, each unique identifier can appear on the left-hand
side of an instruction only once.

For example:

.. code-block:: none

    %sum = add ui64 %lhs_val %rhs_val;


Reference Type Syntax
=====================

Reference types have the `'*'` character after the type.

For example:

.. code-block:: none

    Person* sibling;


Module Syntax Example
=====================

Below is an example of the textual representation of a sample module.

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
