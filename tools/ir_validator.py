# The MIT License (MIT)

# Copyright (c) 2016 Yanzheng Li

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import os
import sys

import avro.datafile
import avro.io
import avro.schema


## -----------------------------------------------------------------------------

SCHEMAS_DIR = 'schemas/'
COREVM_IR_SCHEMA_FILENAME = 'corevm_ir_schema.json'
COREVM_IR_SCHEMA_FILEPATH = os.path.join(SCHEMAS_DIR, COREVM_IR_SCHEMA_FILENAME)

## -----------------------------------------------------------------------------

class IRValidator(object):

    def __init__(self):
        try:
            with open(COREVM_IR_SCHEMA_FILEPATH, 'r') as schema_file:
                self.ir_schema = avro.schema.parse(schema_file.read())
        except Exception as ex:
            sys.stderr.write('Error parsing schema:\n')
            raise ex

    def run(self):
        # TODO: [COREVM-548] Implement IR verification pass
        pass

## -----------------------------------------------------------------------------

def main():
    validator = IRValidator()
    validator.run()

## -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()

