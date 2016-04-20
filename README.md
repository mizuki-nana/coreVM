![logo](/resources/banners/v1/coreVM_banner_v1_RES_LOW.jpg)

*Turn On, Boot Up, Jack In...*

A virtual machine designed to revolutionize the development of modern dynamic
programming languages and to refine the art of programming.

[![Build status](https://api.travis-ci.org/yanzhengli/coreVM.png)](https://travis-ci.org/yanzhengli/coreVM)

* [Overview] (#overview)
* [Get Started] (#get-started)
* [Moonshots] (#moonshots)
* [Contribute](#contribute)
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
can focus on the design and development of language features. coreVM strives to
provide versatile capabilities that can meet the demands of most dynamic
programming languages through the following:

* An instruction set that is generic enough to cover most demands.
* A set of APIs for extensions, customizations, and optimizations for special needs.
* A well thought and extensible architecture.

The points above together form the core philosophy of the project that language
developers should be able to use coreVM as a powerful platform for developing
modern dynamic programming languages either by designing from scratch or
synthesizing from existing ones, all in an intuitive and straightforward manner.
And hopefully, it will help guide language developers to come up with
unconventional language designs and implementations that define the next
generation of computing.


## Get Started

Here is a list of recommended steps to get started on the project:

1. Get familiar with the project by checking out these useful wiki pages:
  * [Design and Architecture](https://github.com/yanzhengli/coreVM/wiki/Design-and-Architecture)
  * [Dependencies and Build Environments](https://github.com/yanzhengli/coreVM/wiki/Dependencies-and-Build-Environments)

2. Build all dependency libraries:
> `git submodule update --init && make -C libs full=1`

3. Build from source and have fun:

  3.1. Build binaries and run tests:
  > `make`

  3.2. Build all, including benchmarks, sanity tests, and dynamic analysis tests (optional):
  > `make full=2`

  3.3 Play around with the Python compiler and run individual Python tests ([more below](#python-variant-based-on-corevm)), for example:
  > `python python/pyta.py python/tests/int.py`

4. Checkout more wiki pages if you are interested:
  * [Current State and Roadmap](https://github.com/yanzhengli/coreVM/wiki/Current-State-and-Roadmap)
  * [Focus Areas](https://github.com/yanzhengli/coreVM/wiki/Focus-Areas)


## Moonshots
While the ultimate goal of this project is to facilitate the design and
development of modern dynamic programming languages for language developers,
I personally have two ideas for the use of this project that I have envisioned
for the foreseeable future.

### Support the HEX programming language
One is to use coreVM as the underlying runtime for the
[HEX programming language](http://www.github.com/yanzhengli/hex) that I
worked on a while ago. This was actually one of the initial goals of the project
before its inception.

### Python variant based on coreVM
The second idea is to have a port of Python that runs on coreVM, as I've always
wanted to join the Python family that's comprised of many flavors of the
language. Currently the effort is codenamed _Pyta_, but hopefully I will come
up with a good name of my variant by then.

As of now, _Pyta_ implements a tiny subset of the core Python language. The
implementation of the language lives under [python/src](python/src), and the
corresponding test suite lives under [python/tests](python/tests). There is a
Python compiler that lives under [python/](python/), which compiles Python
source code into coreVM bytecode. In addition, there is a command-line program
[python/pyta.py](python/pyta.py) that can directly execute a given Python
program by invoking the compiler to generate coreVM bytecode, and feeds the
output to the VM.

The development of the VM and the language complements each other well, as the
language serves as a great target to test and benchmark the VM, at the same time
the VM provides more capabilities to facilitate the development of the language
as it evolves. The parallel developments of the two forms a positive feedback
loop that accelerates the growth of both.

To run the entire Python test suite after compilation, run:
> `python python/bootstrap_tests.py`

To run an individual Python program, such as [python/tests/int.py](python/tests/int.py),
run:
> `python python/pyta.py python/tests/int.py`

Over time, it will be interesting to compare the differences in stability,
performance, and many other aspects across the major flavors of the language,
such as [CPython](https://www.python.org/), [PyPy](http://pypy.org/),
[Jython](http://www.jython.org/), [IronPython](http://ironpython.net/), and
[Pyston](https://github.com/dropbox/pyston).


## Contribute
Please read the following wiki pages if you'd like to contribute to the project:

* [Styles and Guidelines](https://github.com/yanzhengli/coreVM/wiki/Styles-and-Guidelines)
* [Development and Release Workflow](https://github.com/yanzhengli/coreVM/wiki/Development-and-Release-Workflow)


## License
coreVM is licensed under [The MIT License](http://opensource.org/licenses/MIT).


## Blog Posts
Follow the latest news and updates on the [coreVM Official Blog](https://medium.com/corevm-official-blog).


## Inquiries
For technical issues and bugs, please file issues [here](https://github.com/yanzhengli/coreVM/issues/new).
For all other inquiries, please e-mail <a href="mailto:yanzheng819@gmail.com">yanzheng819@gmail.com</a>.
