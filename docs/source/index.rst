.. Copyright Yanzheng Li. All rights reserved.

coreVM Documentation
====================

Overview
--------

With the proliferation of high-level general purpose programming languages being
a dominant trend in the software industry, many developers have taken the
endeavor to invent their own in attempts to illustrate their unique visions of
how modern software systems can be developed. However, such tasks are usually
massive undertakings as the steps involved in developing a custom language
runtime tend to be extremely complex, error prone and resource consuming.
Consequently, these steps end up becoming a very intimidating part in the
process. Although there are candidates of generic runtimes for high-level
languages that are available, many of them lack the robustness, efficiency and
versatility that can meet the language requirements.

coreVM is a language-agnostic runtime framework. Its design strives to achieve
extremely versatile interfaces and modern runtime functionalities, which
unfetters language developers from the burden of developing their own. Instead
of re-inventing the wheel, developers can focus on the design and development of
language features. coreVM strives to provide versatile capabilities that can
meet the demands of most high-level programming languages through the following:

  * An `instruction set <reference.html#instruction-set>`_ that can broadly cover fundamental language functionalities.
  * An `abstract language representation <http://corevm.readthedocs.io/en/latest/ir_reference.html>`_ that can capture high-level language constructs.
  * A `set of APIs <reference.html#apis>`_ for extensions, customizations, and optimizations for special needs.
  * A well thought and extensible `architecture <design.html#design-and-architecture>`_.

The points above together illustrate the core philosophy of the project that
coreVM can be a powerful platform for developers to devise modern high-level
programming languages. Hopefully, as the project evolves, it will help guide
language developers devise novel language designs and implementations that can
shape the next generation of computing.


Contents
--------

.. toctree::
   :maxdepth: 2

   design.rst
   jit.rst
   reference.rst
   ir_reference.rst
   roadmap.rst
   focus.rst
   contribute.rst


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

