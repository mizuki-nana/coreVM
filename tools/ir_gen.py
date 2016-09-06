# The MIT License (MIT)
#
# Copyright (c) 2016 Yanzheng Li
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

## Generates dummy coreVM IR and writes to a specified output file.


import optparse
import os
import sys

import avro.datafile
import avro.io
import avro.schema


class IRGen(object):

    SCHEMAS_DIR = 'schemas/'
    COREVM_IR_SCHEMA_FILE = 'corevm_ir_schema.json'
    COREVM_IR_SCHEMA = os.path.join(SCHEMAS_DIR, COREVM_IR_SCHEMA_FILE)

    def __init__(self, output_file):
        self.output_file = output_file
        with open(self.COREVM_IR_SCHEMA, 'r') as schema_file:
            self.schema = avro.schema.parse(schema_file.read())

    def run(self):
        with open(self.output_file, 'w') as fd:
            writer = avro.datafile.DataFileWriter(
                fd, avro.io.DatumWriter(), self.schema)
            writer.append(self.ir())
            writer.close()

    def ir(self):
        return {
            'meta': self.ir_meta(),
            'types': [
                self.ir_struct_decl_1(),
                self.ir_struct_decl_2()
            ],
            'closures': [
                self.ir_closure()
            ]
        }

    def ir_meta(self):
        return {
            'name': 'Dummy_IR',
            'format_version': 'v0.1.0',
            'target_version': 'v0.1.0',
            'path': './dummy_ir.ir',
            'author': 'Yanzheng Li',
            'timestamp': 1472959465
        }

    def ir_struct_decl_1(self):
        return {
            'name': 'Person',
            'fields': [
                {
                    'identifier': 'name',
                    'ref_type': 'value',
                    'type': 'string'
                },
                {
                    'identifier': 'age',
                    'ref_type': 'value',
                    'type': 'ui8'
                },
                {
                    'identifier': 'sibling',
                    'ref_type': 'pointer',
                    'type': 'Person'
                },
                {
                    'identifier': 'address',
                    'ref_type': 'pointer',
                    'type': 'Location'
                }
            ]
        }

    def ir_struct_decl_2(self):
        return {
            'name': 'Location',
            'fields': [
                {
                    'identifier': 'street_address',
                    'ref_type': 'value',
                    'type': 'string'
                },
                {
                    'identifier': 'country',
                    'ref_type': 'pointer',
                    'type': 'string'
                },
                {
                    'identifier': 'zipcode',
                    'ref_type': 'value',
                    'type': 'string'
                }
            ]
        }

    def ir_closure(self):
        return {
            'name': 'find_friends',
            'id': 1,
            'parent_id': 0,
            'rettype': 'structtype',
            'ret_reftype': 'pointer',
            'parameters': [
                {
                    'identifier': 'person',
                    'ref_type': 'pointer',
                    'type': 'structtype'
                },
                {
                    'identifier': 'id',
                    'ref_type': 'value',
                    'type': 'ui32'
                }
            ],
            'blocks': [
                {
                    'label': 'entry',
                    'body': [
                        {
                            'target': 'sibling',
                            'type': 'ui8',
                            'opcode': 'alloca',
                            'opcodeval': {
                                'type': 'ui8',
                                'value': 16
                            },
                            'oprds': [
                                {
                                    'type': 'constant',
                                    'value': {
                                        'type': 'ui8',
                                        'value': 16
                                    }
                                }
                            ]
                        },
                        {
                            'type': 'voidtype',
                            'opcode': 'br',
                            'opcodeval': {
                                'type': 'ui32',
                                'value': 32
                            },
                            'oprds': [
                                {
                                    'type': 'ref',
                                    'value': 'cond1'
                                },
                                {
                                    'type': 'ref',
                                    'value': 'cond2'
                                }
                            ],
                            'labels': [
                                {
                                    "name": "br1"
                                },
                                {
                                    "name": "br2"
                                },
                                {
                                    "name": "br3"
                                }
                            ]
                        }
                    ]
                },
                {
                    'label': 'cond1',
                    'body': [
                        {
                            'target': 'name',
                            'type': 'ptrtype',
                            'opcode': 'getattr',
                            'opcodeval': {
                                'type': 'string',
                                'value': 'name'
                            },
                            'oprds': [
                                {
                                    'type': 'constant',
                                    'value': {
                                        'type': 'ui8',
                                        'value': 16
                                    }
                                }
                            ],
                            'labels': []
                        }
                    ]
                }
            ]
        }

## -----------------------------------------------------------------------------

def main():
    parser = optparse.OptionParser(
        usage='usage: %prog [options]',
        version='%prog v0.1',
        description='Generates dummy coreVM IR')

    parser.add_option(
        '-o',
        '--output',
        action='store',
        dest='output_file',
        help='Output file'
    )

    options, _ = parser.parse_args()

    if not options.output_file:
        sys.stderr.write('Output file not specified\n')
        sys.exit(-1)

    IRGen(options.output_file).run()

## -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()

