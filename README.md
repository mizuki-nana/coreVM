![logo](/resources/banners/v1/coreVM_banner_v1_RES_LOW.jpg)

A virtual machine designed to revolutionize the development of modern dynamic
programming languages and to refine the art of programming.

[![Build status](https://travis-ci.org/yanzhengli/coreVM.svg?branch=dev)](https://travis-ci.org/yanzhengli/coreVM)
[![Docs build status](https://readthedocs.org/projects/corevm/badge/?version=latest)](http://corevm.readthedocs.io/en/latest/)

* [Overview] (#overview)
* [Get Started] (#get-started)
* [Moonshots] (#moonshots)
* [Contribute](#contribute)
* [License] (#license)
* [Updates] (#updates)
* [Inquiries] (#inquiries)

## Overview
With the proliferation of dynamic programming languages being a dominating trend
in the software industry, many developers have taken the endeavor to invent
their own in attempts to illustrate their unique visions of how modern software
systems can be developed. However, such tasks are usually massive undertakings
as the steps involved in developing a custom language runtime tend to be
extremely complex, error prone and resource consuming. Consequently, these steps
end up becoming a very intimidating part in the process. Although there are
candidates of generic runtimes for dynamic languages that are available, many of
them lack the robustness, efficiency and versatility that can meet the language
requirements.

coreVM is a language-agnostic virtual machine designed to provide a runtime
platform for dynamic programming languages. Its design strives to achieve
extremely versatile interfaces and modern runtime functionalities, which
unfetters language developers from the burden of developing their own. Instead
of re-inventing the wheel, developers can focus on the design and development of
language features. coreVM strives to provide versatile capabilities that can
meet the demands of most dynamic programming languages through the following:

* An [instruction set](http://corevm.readthedocs.io/en/latest/reference.html#instruction-set) that can broadly cover most demands.
* A [set of APIs](http://corevm.readthedocs.io/en/latest/reference.html#apis) for extensions, customizations, and optimizations for special needs.
* A well thought and extensible [architecture](http://corevm.readthedocs.io/en/latest/design.html).

The points above together illustrate the core philosophy of the project that
language developers should be able to use coreVM as a powerful platform for
developing modern dynamic programming languages in an intuitive and
efficient manner. Hopefully, as the project evolves, it will help guide
language developers devise novel language designs and implementations that can
shape the next generation of computing.


## Get Started

Here is a list of recommended steps to get started on the project:

1. Get familiar with the project by checking out these useful links:
  * [Project Documentation](http://corevm.readthedocs.io/en/latest/)
  * [Design and Architecture](https://github.com/yanzhengli/coreVM/wiki/Design-and-Architecture)
  * [Dependencies and Build Environments](https://github.com/yanzhengli/coreVM/wiki/Dependencies-and-Build-Environments)

2. Build from source and have fun:

  2.1 Build and install all dependencies:
  > `git submodule update --init && make -C libs full=1`

  2.2. Build main binaries and run tests:
  > `make`

  2.3. Build all, including benchmarks, sanity tests, and dynamic analysis tests (optional):
  > `make full=2`

  2.4. Play around with the Python compiler and run individual Python tests ([more below](#python-variant-based-on-corevm)), for example:
  > `python python/pyta.py python/tests/int.py`

3. Checkout more wiki pages if you are interested:
  * [Current State and Roadmap](https://github.com/yanzhengli/coreVM/wiki/Current-State-and-Roadmap)
  * [Focus Areas](https://github.com/yanzhengli/coreVM/wiki/Focus-Areas)


## Moonshots
While the ultimate goal of this project is to facilitate the design and
development of modern dynamic programming languages for language developers,
I personally have two use cases of the project that I have envisioned for the
foreseeable future.

### Support the HEX programming language
One is to use coreVM as the underlying runtime for the
[HEX programming language](http://www.github.com/yanzhengli/hex) that I
worked on a while ago. This was actually one of the main motivations of the
project before its inception.

### Python variant based on coreVM
The second idea is to have a port of Python that runs on coreVM, as I've always
wanted to join the Python family that is comprised of many flavors of the
language. Currently the effort is codenamed _[Project Pyta](https://github.com/yanzhengli/coreVM/wiki/Current-State-and-Roadmap#project-pyta)_,
but hopefully I will come up with a good name of my variant by then.

As of now, _Pyta_ implements a tiny subset of the core Python language. The
implementation of the language lives under [python/src](python/src), and the
corresponding test suite are under [python/tests](python/tests). There is a
Python compiler that lives under [python/compiler](python/compiler), which
compiles Python source code into coreVM bytecode. In addition, there is a
command-line program [python/pyta.py](python/pyta.py) that can directly execute
a given Python program by invoking the compiler to generate coreVM bytecode,
and feeds the output to the VM. Click [here](python/README.md) to view the
project summary page.

The development of the VM and the language complements each other well, as the
language serves as a great target to test and benchmark the VM, and at the same
time the VM provides more capabilities to facilitate the growth of the language
as it evolves. The parallel developments of the two forms a positive feedback
loop that accelerates the growth of both.

To run the entire Python test suite after compilation, run:
> `python python/bootstrap_tests.py`

To run an individual Python program, such as [python/tests/int.py](python/tests/int.py),
run:
> `python python/pyta.py python/tests/int.py`

Over time, it will be interesting to compare _Pyta_'s stability, performance,
and many other aspects with the major flavors of the language, such as
[CPython](https://www.python.org/), [PyPy](http://pypy.org/),
[Jython](http://www.jython.org/), [IronPython](http://ironpython.net/), and
[Pyston](https://github.com/dropbox/pyston).


## Contribute
Please read the following wiki pages if you'd like to contribute to the project:

* [Styles and Guidelines](https://github.com/yanzhengli/coreVM/wiki/Styles-and-Guidelines)
* [Development and Release Workflow](https://github.com/yanzhengli/coreVM/wiki/Development-and-Release-Workflow)


## License
coreVM is licensed under [The MIT License](http://opensource.org/licenses/MIT).


## Updates
Follow the latest news and updates on the project's [official site](http://www.corevm.org/)
and the [coreVM Official Blog](https://medium.com/corevm-official-blog).


## Inquiries
For technical issues and bugs, please file issues [here](https://github.com/yanzhengli/coreVM/issues/new).
For all other inquiries, please e-mail <a href="mailto:yli@corevm.org">yli@corevm.org</a>.
