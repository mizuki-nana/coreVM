.. Copyright Yanzheng Li. All rights reserved.

coreVM documentation
====================

Overview
--------

With the proliferation of dynamic programming languages being a dominating trend in the software industry, many developers have taken the endeavor to invent their own, in attempts to modernize the designs and implementations of many legacy systems, and how modern software systems can be developed. However, such tasks are usually massive undertakings as the steps involved in developing a custom runtime for a dynamic programming language tend to be extremely complex, error prone and resource consuming, thus ending up being a very intimidating part in the process. Although there are candidates of generic runtimes for dynamic languages that are available, many of them lack the robustness, efficiency and versatility that can meet the language requirements.

coreVM is a language-agnostic virtual machine designed to provide a runtime for dynamic programming languages. The design strives to achieve extremely versatile interfaces and modern runtime functionalities, which unfetters language developers from the burden of developing custom runtimes and the tedious compiler integrations involved. Instead of re-inventing the wheel, developers can focus on the design and development of language features. coreVM strives to provide versatile capabilities that can meet the demands of most dynamic programming languages through the following:

  * An instruction set that can broadly cover most demands.
  * A set of APIs for extensions, customizations, and optimizations for special needs.
  * A well thought and extensible architecture.

The points above together illustrate the core philosophy of the project that language developers should be able to use coreVM as a powerful platform for developing modern dynamic programming languages in an intuitive and straightforward manner.Hopefully, as the project evolves, it will help guide language developers devise novel language designs and implementations that can shape the next generation of computing.


Contents
--------

.. toctree::
   :maxdepth: 2

   design.rst
   roadmap.rst
   focus.rst


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

