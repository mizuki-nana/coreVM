/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2017 Yanzheng Li

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/
#include "ir/verifier.h"
#include "ir/format.h"
#include "ir/ir_module_index.h"
#include "ir/ir_parser_driver.h"

#include <gtest/gtest.h>


// -----------------------------------------------------------------------------

class VerifierUnitTest : public ::testing::Test
{
protected:
  void check_verification(const char* IR_STRING, const char* expected_msg = NULL)
  {
    corevm::ir::IRParserDriver driver;
    const int res = driver.parse_from_string(IR_STRING);

    ASSERT_EQ(0, res);

    const corevm::IRModule& module = driver.module();

    corevm::ir::Verifier verifier(module);

    std::string msg;
    if (!expected_msg)
    {
      ASSERT_EQ(true, verifier.run(msg));
    }
    else
    {
      ASSERT_EQ(false, verifier.run(msg));
      ASSERT_STREQ(expected_msg, msg.c_str());
    }
  }
};

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestInitialization)
{
  corevm::IRModule module;
  corevm::ir::Verifier verifier(module);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithSingleValidTypeDefinition)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithSingleInvalidTypeDefinition)
{
  const char* IR_STRING =
    "type Person {"
    "    Address address;"
    "}";

  check_verification(IR_STRING,
    "Unknown type used for type member Person::address");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithTwoValidTypeDefinitions)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "    Location location;"
    "}"
    ""
    "type Location {"
    "    string address;"
    "    dpf lat;"
    "    dpf lon;"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneInvalidTypeDefinition)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "    Location location;"
    "}"
    ""
    "type Location {"
    "    string address;"
    "    dpf lat;"
    "    dpf lon;"
    "    Zipcode zipcode;"
    "}";

  check_verification(IR_STRING,
    "Unknown type used for type member Location::zipcode");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneInvalidTypeDefinitionWithRecursiveInvalidTypeUsage)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "    Location location;"
    "}"
    ""
    "type Location {"
    "    string address;"
    "    dpf lat;"
    "    dpf lon;"
    "    array [ 10 * Address ] addresses;"
    "}"
    ""
    "type Address {"
    "    string street;"
    "    string city;"
    "    string state;"
    "    string country;"
    "    array [ 2 * Zipcode ] zipcodes;"
    "}";

  check_verification(IR_STRING,
    "Unknown type used for type member Address::zipcodes");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestGetIndex)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "}";

  corevm::ir::IRParserDriver driver;
  const int res = driver.parse_from_string(IR_STRING);
  ASSERT_EQ(0, res);
  const corevm::IRModule& module = driver.module();
  corevm::ir::Verifier verifier(module);

  std::unique_ptr<corevm::ir::IRModuleIndex> index;
  ASSERT_EQ(nullptr, index);

  verifier.get_index(index);

  ASSERT_NE(nullptr, index);

  ASSERT_EQ(1, index->type_index.size());
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithSingleValidFunctionDefinition)
{
  const char* IR_STRING =
    "def void doNothing() {"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithTwoValidFunctionDefinitions)
{
  const char* IR_STRING =
    "def void doNothing() {"
    "}"
    ""
    "def string helloWorld() {"
    "entry:"
    "    ret string \"Hello world\";"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithInvalidParent)
{
  const char* IR_STRING =
    "def void doNothing() {"
    "}"
    ""
    "def string helloWorld() : doSomething {"
    "entry:"
    "    ret string \"Hello world\";"
    "}";

  check_verification(IR_STRING,
    "Invalid parent of function of \"helloWorld\": \"doSomething\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithInvalidReturnType)
{
  const char* IR_STRING =
    "def MyType doNothing() {"
    "}";

  check_verification(IR_STRING,
    "Invalid return type for function \"doNothing\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithOneInvalidParameterType)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "    string location;"
    "}"
    "def void doNothing() {"
    "}"
    ""
    "def string helloWorld(Person person, Location location) {"
    "entry:"
    "    ret string \"Hello world\";"
    "}";

  check_verification(IR_STRING,
    "Invalid type for parameter \"location\" of function \"helloWorld\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithDuplicateParameters)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "}"
    ""
    "def string helloWorld(Person person, string person) {"
    "entry:"
    "    ret string \"Hello world\";"
    "}";

  check_verification(IR_STRING,
    "Duplicate parameter \"person\" in function \"helloWorld\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithDuplicateBasicBlocks)
{
  const char* IR_STRING =
    "def string helloWorld(string person) {"
    "entry:"
    "    ret string \"Hello world\";"
    "entry:"
    "    ret string \"We should not be here\";"
    "}";

  check_verification(IR_STRING,
    "Duplicate basic block \"entry\" in function \"helloWorld\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithInvalidSSAForm)
{
  const char* IR_STRING =
    "def string helloWorld(string person) {"
    "entry:"
    "    %x = alloca [auto] string;"
    "bb2:"
    "    %x = alloca [auto] uint64;"
    "}";

  check_verification(IR_STRING,
    "Duplicate instruction target \"x\" in function \"helloWorld\" under block \"bb2\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithInvalidInstrType)
{
  const char* IR_STRING =
    "def string helloWorld(string person) {"
    "entry:"
    "    %x = alloca [auto] Person;"
    "}";

  check_verification(IR_STRING,
    "Invalid type used in instruction \"alloca\" in function \"helloWorld\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithUndeclaredOperand)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "    string location;"
    "}"
    ""
    "def string helloWorld(i32 i) {"
    "entry:"
    "    %x = alloca [auto] Person;"
    "    setattr string \"age\" %i %person;"
    "}";

  check_verification(IR_STRING,
    "Undeclared operand used in instruction \"setattr\" in function \"helloWorld\" under block \"entry\": \"person\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithInvalidInstrLabel)
{
  const char* IR_STRING =
    "def string helloWorld(i32 i) {"
    "entry:"
    "    br %i [ label #end, label #end ];"
    "}";

  check_verification(IR_STRING,
    "Invalid label used in instruction \"br\" in function \"helloWorld\" under block \"entry\": \"end\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithInvalidInstrOptionCount)
{
  const char* IR_STRING =
    "def void helloWorld() {"
    "entry:"
    "    %x = alloca [ auto static ] string;"
    "}";

  check_verification(IR_STRING,
    "Instruction \"alloca\" in function \"helloWorld\" under block \"entry\" has incorrect number of options");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithInvalidInstrOperandCount)
{
  const char* IR_STRING =
    "def void helloWorld() {"
    "entry:"
    "    %x = alloca [ auto ] string ui32 5;"
    "}";

  check_verification(IR_STRING,
    "Instruction \"alloca\" in function \"helloWorld\" under block \"entry\" has incorrect number of operands");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithInvalidInstrLabelCount)
{
  const char* IR_STRING =
    "def void helloWorld(i32 i) {"
    "entry:"
    "    br %i [ label #goto, label #goto, label #goto ];"
    "goto:"
    "    ret void;"
    "}";

  check_verification(IR_STRING,
    "Instruction \"br\" in function \"helloWorld\" under block \"entry\" has incorrect number of labels");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithIncompatibleInstrAndOperandType)
{
  const char* IR_STRING =
    "def void helloWorld(i32 i) {"
    "entry:"
    "    %x = load string %i;"
    "}";

  check_verification(IR_STRING,
    "Incompatible operand type in instruction \"load\" in function \"helloWorld\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithInvalidOptionForAllocaInstr)
{
  const char* IR_STRING =
    "def void helloWorld() {"
    "entry:"
    "    %i = alloca [ hello ] i32;"
    "}";

  check_verification(IR_STRING,
    "Unrecognized option in instruction \"alloca\" in function \"helloWorld\" under block \"entry\": \"hello\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithTwoIncompatibleOperands)
{
  const char* IR_STRING =
    "def void helloWorld() {"
    "entry:"
    "    %i = alloca [ auto ] i32;"
    "    %same = eq %i string \"Hello wold\";"
    "}";

  check_verification(IR_STRING,
    "Incompatible operand types for instruction \"eq\" in function \"helloWorld\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithTwoCompatibleOperands)
{
  const char* IR_STRING =
    "def void helloWorld() {"
    "entry:"
    "    %i = alloca [ auto ] i32;"
    "    %same = eq %i i32 5;"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithTwoIncompatibleOperandsInSwitch2Instr)
{
  const char* IR_STRING =
    "def void helloWorld() {"
    "entry:"
    "    %i = alloca [ auto ] i32;"
    "    switch2 %i dpf 3.14 [ label #end ];"
    "end:"
    "    ret void;"
    "}";

  check_verification(IR_STRING,
    "Operands at index 0 and 1 have incompatible type in instruction \"switch2\" in function \"helloWorld\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithInvalidReturnTypeInInstruction)
{
  const char* IR_STRING =
    "def string helloWorld() {"
    "entry:"
    "    ret i32 5;"
    "}";

  check_verification(IR_STRING,
    "Invalid return type for instruction \"ret\" in function \"helloWorld\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneFunctionDefinitionWithValidReturnType)
{
  const char* IR_STRING =
    "def string helloWorld() {"
    "entry:"
    "    ret string \"Hello world\";"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithCallInstructionWithInvalidFunctionName)
{
  const char* IR_STRING =
    "def string helloWorld() {"
    "entry:"
    "    ret string \"Hello world\";"
    "}"
    ""
    "def void main() {"
    "entry:"
    "    call i32 string \"worldHello\";"
    "}";

  check_verification(IR_STRING,
    "Invalid callee in instruction \"call\" in function \"main\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithCallInstructionWithValidFunctionName)
{
  const char* IR_STRING =
    "def string helloWorld() {"
    "entry:"
    "    ret string \"Hello world\";"
    "}"
    ""
    "def void main() {"
    "entry:"
    "    call string string \"helloWorld\";"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithCallInstructionWithInvalidInstructionType)
{
  const char* IR_STRING =
    "def string helloWorld() {"
    "entry:"
    "    ret string \"Hello world\";"
    "}"
    ""
    "def void main() {"
    "entry:"
    "    call i32 string \"helloWorld\";"
    "}";

  check_verification(IR_STRING,
    "Invalid type of instruction \"call\" in function \"main\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithCallInstructionWithInvalidArgumentType)
{
  const char* IR_STRING =
    "def string helloWorld(i32 i) {"
    "entry:"
    "    ret string \"Hello world\";"
    "}"
    ""
    "def void main() {"
    "entry:"
    "    call string string \"helloWorld\" dpf 3.14 ;"
    "}";

  check_verification(IR_STRING,
    "Invalid argument type in instruction \"call\" in function \"main\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithCallInstructionWithInvalidArgumentCount)
{
  const char* IR_STRING =
    "def string helloWorld(i32 i, i64 k) {"
    "entry:"
    "    ret string \"Hello world\";"
    "}"
    ""
    "def void main() {"
    "entry:"
    "    call string string \"helloWorld\" i32 5;"
    "}";

  check_verification(IR_STRING,
    "Invalid number of arguments in instruction \"call\" in function \"main\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithValidCallInstruction)
{
  const char* IR_STRING =
    "def string helloWorld(i32 i, i64 k) {"
    "entry:"
    "    ret string \"Hello world\";"
    "}"
    ""
    "def void main() {"
    "entry:"
    "    call string string \"helloWorld\" i32 5 i64 10;"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestGetattrInstrWithInvalidFieldAccessor)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "    ui32 age;"
    "}"
    ""
    ""
    "def void main() {"
    "entry:"
    "    %person = alloca [ auto ] Person;"
    "    %addr = getattr string \"address\" %person;"
    "}";

  check_verification(IR_STRING,
    "Invalid type accessor constant for first operand in instruction \"getattr\" in function \"main\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestGetattrInstrWithValidFieldAccessor)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "    ui32 age;"
    "}"
    ""
    ""
    "def void main() {"
    "entry:"
    "    %person = alloca [ auto ] Person;"
    "    %age = getattr string \"age\" %person;"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestSetattrInstrWithInvalidFieldAccessor)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "    ui32 age;"
    "}"
    ""
    ""
    "def void main(ui32 age) {"
    "entry:"
    "    %person = alloca [ auto ] Person;"
    "    setattr string \"address\" %age %person;"
    "}";

  check_verification(IR_STRING,
    "Invalid type accessor constant for first operand in instruction \"setattr\" in function \"main\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestSetattrInstrWithInvalidSourceType)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "    ui32 age;"
    "}"
    ""
    ""
    "def void main(ui32 age) {"
    "entry:"
    "    %person = alloca [ auto ] Person;"
    "    setattr string \"age\" dpf 3.14 %person;"
    "}";

  check_verification(IR_STRING,
    "Mismatch between source type and target field type in instruction \"setattr\" in function \"main\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestSetattrInstrWithValidOperands)
{
  const char* IR_STRING =
    "type Person {"
    "    string name;"
    "    ui32 age;"
    "}"
    ""
    ""
    "def void main(ui32 age) {"
    "entry:"
    "    %person = alloca [ auto ] Person;"
    "    setattr string \"age\" %age %person;"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestLoadInstrWithCompatibleOperands)
{
  const char* IR_STRING =
    "def void main(ui32 age) {"
    "entry:"
    "    %x = load i64 %age;"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestLoadInstrWithIncompatibleOperands)
{
  const char* IR_STRING =
    "def void main(ui32 age) {"
    "entry:"
    "    %x = load string %age;"
    "}";

  check_verification(IR_STRING,
    "Incompatible operand type in instruction \"load\" in function \"main\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestStoreInstrWithCompatibleOperands)
{
  const char* IR_STRING =
    "def void main() {"
    "entry:"
    "    %i = alloca [auto] i32;"
    "    store ui64 ui64 55 %i;"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestStoreInstrWithIncompatibleOperands)
{
  const char* IR_STRING =
    "def void main() {"
    "entry:"
    "    %i = alloca [auto] i32;"
    "    store string string \"Hello world\" %i;"
    "}";

  check_verification(IR_STRING,
    "Incompatible operand type in instruction \"store\" in function \"main\" under block \"entry\"");
}

// -----------------------------------------------------------------------------

