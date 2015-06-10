# The MIT License (MIT)

# Copyright (c) 2015 Yanzheng Li

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

MICRO=micro


BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/main.cc

BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/$(MICRO)/instr_benchmarks_fixture.cc

BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/$(MICRO)/allocators_benchmark.cc
BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/$(MICRO)/object_container_benchmark.cc
BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/$(MICRO)/process_benchmark.cc
BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/$(MICRO)/object_instrs_benchmark.cc
BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/$(MICRO)/control_instrs_benchmark.cc
BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/$(MICRO)/function_instrs_benchmark.cc
BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/$(MICRO)/ntvhndl_arithmetic_instrs_benchmark.cc
BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/$(MICRO)/ntvhndl_creation_instrs_benchmark.cc
BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/$(MICRO)/ntvhndl_conversion_instrs_benchmark.cc
BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/$(MICRO)/ntvhndl_manipulation_instrs_benchmark.cc
BENCHMARK_SOURCES += $(TOP_DIR)/$(BENCHMARKS)/$(MICRO)/native_string_instrs_benchmark.cc
