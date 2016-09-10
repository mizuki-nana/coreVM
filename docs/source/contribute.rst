.. Copyright Yanzheng Li. All rights reserved.

Contribute
==========

Welcome to the wonderful world of coreVM! Please read through this page if you
are interested in contributing to the project and would like to learn more.

Ways to Get Involved
--------------------

There are many ways to contribute to the project. What you choose to work on
depends on your interests and passion, as well as skills and experience. Here
are some of the areas that you can make a contribution:

Fixes and Improvements for coreVM
#################################

Whether you want to make a code change for a bug you discovered or have an idea
of improvement, take a first step to check out the project's
`Reference Manual <reference.html>`_ to learn more about how to build the
project and its internals, and the :ref:`coding-styles-and-conventions` section
below to learn about the coding styles and conventions used in the codebase.

If you wish to report a bug, you can file an issue on the project's
`issue page <https://github.com/yanzhengli/coreVM/issues>`_, and if you'd like
to submit a patch of code changes, please open up a pull request
`here <https://github.com/yanzhengli/coreVM/pulls>`_.

Documentation Improvement
#########################

The project's documentation is vital as it serves as a bridge of communication
to people. There are always room for improvements in the documentation, whether
it is the pages that you are reading right now, documentations in the source
code, or in the project's GitHub
`wiki pages <https://github.com/yanzhengli/coreVM/wiki>`_ or other mediums.

Share a Thought and Chat
########################

Whether you have question(s) regarding the project, have an idea or thought that
you'd like to share, or simply want to chat to learn more about the project,
you are always welcome to send an email to `yli@corevm.org <yli@corevm.org>`_.

.. _coding-styles-and-conventions:

Coding Styles and Conventions
-----------------------------

This section defines a set of coding styles and guidelines that are recommended
to be followed when contributing to code changes in this project's repository.

File Names and Paths
####################

- File names should be succinct, yet descriptive and should be able to reflect
  the meaning of the file. File names should be composed of alphanumerical
  character all in lower cases(in most cases), with words joined by underscores.
- Header files should end with the suffix `.h`, while C++ source files should
  end with the suffix `.cc`.
- All source files related to the public APIs should be included in the
  `src/api/ <https://github.com/yanzhengli/coreVM/tree/master/src/api>`_
  directory, and all other source files should be placed under their
  corresponding directories.
- The following describes the top-level directory structure and explains where
  different types of source files should be placed under:

  - `benchmarks/ <https://github.com/yanzhengli/coreVM/tree/master/benchmarks>`_ - source code of macro and micro benchmarks.
  - `docs/ <https://github.com/yanzhengli/coreVM/tree/master/docs>`_ - project's auto-generated documentation (what you are reading now).
  - `libs/ <https://github.com/yanzhengli/coreVM/tree/master/libs>`_ - dependency libraries.
  - `python/ <https://github.com/yanzhengli/coreVM/tree/master/python>`_ - source code of `Project Pyta <roadmap.html#project-pyta>`_.
  - `schemas/ <https://github.com/yanzhengli/coreVM/tree/master/schemas>`_ - schema definitions.
  - `scripts/ <https://github.com/yanzhengli/coreVM/tree/master/scripts>`_ - repository of various scripts used in builds.
  - `src/ <https://github.com/yanzhengli/coreVM/tree/master/src>`_ - project's main source files.
  - `tests/ <https://github.com/yanzhengli/coreVM/tree/master/tests>`_ - source code of unit tests.
  - `tools/ <https://github.com/yanzhengli/coreVM/tree/master/tools>`_ - source code of various tools.

Coding Styles and Practices
###########################

Include Guards
^^^^^^^^^^^^^^

- Include guards should begin with `COREVM_`, followed by the name of the file
  or class in capital letters, and followed by `_H_`. For example, the header
  file that defines the Core API's configuration class has the include guard
  name of `COREVM_API_CONFIGURATION_H_`.
- In most cases, include guards must start at the beginning of the file before
  any other macros, include directives and definitions. Also, they must be in
  the standard `#ifndef`, `#define`, and `#endif` order.

Include Directives
^^^^^^^^^^^^^^^^^^

- Include directives must be listed in the following order, with each section
  separated by a blank line:

  1. corresponding header
  2. necessary project headers
  3. 3rd party libraries headers
  4. standard libraries headers
  5. system headers

Namespaces and Declarations
^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Namespaces must begin with the most outer namespace of `corevm`, followed by
  an inner namespace for one of the main areas in the project.
- Class, struct and function declarations must be included in header files, and
  under the proper namespaces. All definitions must be placed in source files.

Classes
^^^^^^^

- Use the keyword `explicit` for constructors that take one argument, unless
  implicit conversions are intended.
- Only do work related to initialization/destruction of resources inside
  constructors and destructors respectively.
- Implement copy constructor and assignment operator only if needed.
- Make a class support move semantics if it is expected that instances of that
  class are often passed as rvalues.
- Avoid multi-inheritance if possible, as it often introduces unnecessary code
  complexities and maintenance overheads.
- Have the proper access modifier over class members.
- Only inline functions that are short and have constant time runtimes.
- Use the `const` modifier on member types, parameters, and function signatures
  where appropriate.
- Use `struct` only for passive data wrapper, otherwise use classes instead.

Resource Management
^^^^^^^^^^^^^^^^^^^

- Follow the RAII (Resource Allocation Is Initialization) idiom where possible,
  this helps reduce a lot of complexities associated with resource managements.
- Use smart pointers where there's complex delegation, sharing and
  relinquishment on ownership of resources. However, it's advised to use smart
  pointers from the standard template library or boost.
- Have good comments and documentations at places where the ownership of
  resources are not obvious, or the ways resources are passed around are not
  clear. Specially, the places where a particular resource is initialized and
  destroyed should be mentioned.

Styles and Formatting
^^^^^^^^^^^^^^^^^^^^^

- All source files should use two spaces for single indentation. Tabs are
  prohibited.
- Lines in source files should be at most 80 characters in length in most cases.
  It is acceptable to have few exceptions where that is difficult to achieve.
- When in doubt with regards to code formatting, always refers to existing
  code for reference.
- All class and struct names should be in camel case, all functions and variable
  names should be in underscore case.
- Instance member variables should begin with the `m_` prefix.
- Variables of constant literals should be in all capital letters.
- Namespaces should be declared right next to each other. All namespaces should
  end with the comment `/* end XXX namespace. */`.

  Here's an example to show case the coding styles mentioned:

  .. code-block:: cpp

    #ifndef COREVM_MY_CLASS_H_
    #define COREVM_MY_CLASS_H_

    #include "common.h"

    #include <stdexcept>
    #include <string>

    namespace corevm {
    namespace sample {

    /**
     * An abstraction that demonstrates the use of proper coding styles.
     */
    template <typename ContainerType>
    class MyClass
    {
    public:
        struct Options
        {
          int value;
        };

        using value_type = typename ContainerType::value_type;

        explicit MyClass(const std::string& name);

        MyClass(const MyClass&);

        MyClass& operator=(const MyClass&);

        bool run() throw(std::runtime_error);
    private:
        // Note that constant variables are capitalized.
        static const std::string COMMON_NAME;

        std::string m_name;
    };

    } /* end namespace sample */
    } /* end namespace corevm */

    #endif /* COREVM_MY_CLASS_H_ */

Comments
^^^^^^^^

- The use of comments should be mostly restricted to the following use cases:

  - To illustrate and highlight of why things are done in certain way.
  - To provide high level overview and description of shared interfaces.

- For the first case, where comments are mainly used to explain why certain
  things are done the way they are, or to explain and highlight intricate
  conditions or cases that may not seem obvious, inline comments should be used.
  For example:

  `// This loop starts at one index after the starting point because ...`

- For the second case, where comments are used to provide high level overviews
  of shared interfaces, multi-line comments should be used. For example:

  .. code-block:: cpp

    /**
     * Sets the current program counter to the specified position,
     * assuming the following preconditions are met:
     */

Testing
#######

- Every component introduced should have the associated tests implemented.
  Tests live in the top level directory `tests/ <https://github.com/yanzhengli/coreVM/tree/master/tests>`_.
- Test files should be named with the name of the associated file, followed by
  either the suffix `_unittest.cc` or `_test.cc`.
- It is a good practice to include a line of comment at the top of the test file
  to specify which definition that this test is written for.
- Each test should have a test class defined, which inherits either directly or
  indirectly from `::testing::Test` defined in Google Test. A test class should
  define any test fixtures needed as members, and may have the `void Setup()`
  and `void TearDown()` members implemented to do the appropriate setup and
  teardown work.
- Each test class may have one or more test cases associated. Each test case
  should be named with the prefix `Test`, followed by a combination of words
  that form a sensical definition that can reflect the meaning of that
  particular test case. For example, a particular test case that tests the
  allocation of an allocator on empty memory should be named to something
  similar to `TestAllocationOnEmptyMemory`.
- Each test should have sufficient test cases to cover all cases of a definition
  implementation. It's advised to follow the 3-times rule of thumb of testing,
  which essentially means that the amount of code in the test should be at
  least 3 times of the amount of code in the actual implementation being tested.
- Commits that can only be considered to be pulled in into the master branch if
  all associated tests of a particular definition pass.
- Each pull request and commit triggers a build on Travis CI
  `here <https://travis-ci.org/yanzhengli/coreVM/builds>`__. Only pull requests
  that pass all CI builds can be merged into the master branch.

Commits
#######

- Commit messages should be properly written. The first line of the message
  should be a short line of text that can succinctly describes the changes made.
  An additional commit body message can be included if needed. Commits with
  non-sensical or incomplete commit messages may be rejected or reverted.
- Commits must have the correct author name and e-mail address.
- Commits must be as atomic as possible, meaning they should only include the
  changes for a particular implementation or bug fix, and should not include
  anything unrelated.
- Commits should not include merged changes from other branches that are not
  already in `master` branch.
- If a commit is already being pulled to master, do not amend that commit.
  Push a separate commit.
