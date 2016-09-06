.. Copyright Yanzheng Li. All rights reserved.

Just-in-Time Compilation
========================

coreVM makes use of a set of techniques to achieve just-in-time compilation.
The first step is to use a runtime analyzer to profile bytecode execution in
order to determine the necessary code blocks or parts of the code blocks that
have the need to be just-in-time compiled with the appropriate runtime
tradeoffs. The second step following that is to sample the runtime behavior of
the identified blocks and infer the types of the objects referred throughout the
identified code paths.

Once the runtime analyses are complete, coreVM uses the data gathered to
generate the `coreVM intermediate representation <reference.html#corevm-intermediate-representation>`_
of the code blocks that are to be compiled. Next, the IR representation goes
through stages of lowering processes to be translated into a lower-level
representation, which is then translated into LLVM IR.

Next, coreVM makes use of the JIT API provided by LLVM to compile the IR into
native machine code. And finally, it uses on-stack replacement to bootstrap and
execute the compiled native code.

The diagram below illustrates coreVM's just-in-time compilation pipeline.

.. figure:: ../../resources/corevm_jit_compilation_mechanism.png

----

Currently the JIT compilation pipeline is still a work-in-progress. More details
will be documented here once the feature is complete.
