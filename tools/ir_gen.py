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
        with open(self.output_file, 'wb') as fd:
            writer = avro.datafile.DataFileWriter(
                fd, avro.io.DatumWriter(), self.schema)
            writer.append(self.ir())
            writer.close()

    def ir(self):
        return {
            'meta': self.ir_meta(),
            'types': [
                self.ir_type_decl_1(),
                self.ir_type_decl_2(),
                self.ir_type_decl_3()
            ],
            'closures': [
                self.ir_closure_1(),
                self.ir_closure_2(),
                self.ir_closure_3()
            ]
        }

    def ir_meta(self):
        return {
            'name': 'Dummy_IR',
            'format_version': 1000,
            'target_version': 10,
            'path': './dummy_ir.ir',
            'author': 'Yanzheng Li',
            'timestamp': 1472959465
        }

    def ir_type_decl_1(self):
        return {
            'name': 'Person',
            'fields': [
                {
                    'identifier': 'name',
                    'ref_type': 'value',
                    'type': {
                        'type': 'IdentifierType_ValueType',
                        'value': 'string'
                    }
                },
                {
                    'identifier': 'age',
                    'ref_type': 'value',
                    'type': {
                        'type': 'IdentifierType_ValueType',
                        'value': 'ui8'
                    }
                },
                {
                    'identifier': 'sibling',
                    'ref_type': 'pointer',
                    'type': {
                        'type': 'IdentifierType_Identifier',
                        'value': 'Person'
                    }
                },
                {
                    'identifier': 'address',
                    'ref_type': 'pointer',
                    'type': {
                        'type': 'IdentifierType_Identifier',
                        'value': 'Location'
                    }
                },
                {
                    'identifier': 'friends',
                    'ref_type': 'value',
                    'type': {
                        'type': 'IdentifierType_Array',
                        'value': {
                            'len': 10,
                            'type': {
                                'type': 'IdentifierType_Identifier',
                                'value': 'Person'
                            }
                        }
                    }
                }
            ]
        }

    def ir_type_decl_2(self):
        return {
            'name': 'Location',
            'fields': [
                {
                    'identifier': 'street_address',
                    'ref_type': 'value',
                    'type': {
                        'type': 'IdentifierType_ValueType',
                        'value': 'string'
                    }
                },
                {
                    'identifier': 'country',
                    'ref_type': 'pointer',
                    'type': {
                        'type': 'IdentifierType_ValueType',
                        'value': 'string'
                    }
                },
                {
                    'identifier': 'zipcode',
                    'ref_type': 'value',
                    'type': {
                        'type': 'IdentifierType_ValueType',
                        'value': 'string'
                    }
                }
            ]
        }

    def ir_type_decl_3(self):
        return {
            'name': 'NullType',
            'fields': []
        }

    def ir_closure_1(self):
        return {
            'name': 'createPerson',
            'rettype': {
                'type': 'IdentifierType_Identifier',
                'value': 'Person'
            },
            'ret_reftype': 'pointer',
            'parameters': [
                {
                    'identifier': 'name',
                    'ref_type': 'pointer',
                    'type': {
                        'type': 'IdentifierType_ValueType',
                        'value': 'string'
                    },
                },
                {
                    'identifier': 'age',
                    'ref_type': 'value',
                    'type': {
                        'type': 'IdentifierType_ValueType',
                        'value': 'ui32'
                    },
                }
            ],
            'blocks': [
                {
                    'label': 'entry',
                    'body': [
                        {
                            'target': 'person',
                            'type': {
                                'type': 'IdentifierType_Identifier',
                                'value': 'Person'
                            },
                            'opcode': 'alloca',
                            'options': [
                                'auto'
                            ],
                            'oprds': []
                        },
                        {
                            'opcode':  'setattr',
                            'options': [],
                            'oprds': [
                                {
                                    'type': 'constant',
                                    'value': {
                                        'type': 'string',
                                        'value': 'age'
                                    }
                                },
                                {
                                    'type': 'ref',
                                    'value': 'age'
                                },
                                {
                                    'type': 'ref',
                                    'value': 'person'
                                }
                            ],
                            'labels': []
                        },
                        {
                            'opcode': 'br',
                            'options': [],
                            'oprds': [
                                {
                                    'type': 'ref',
                                    'value': 'age'
                                },
                            ],
                            'labels': [
                                {
                                    "name": "end"
                                },
                                {
                                    "name": "end"
                                },
                            ]
                        }
                    ]
                },
                {
                    'label': 'end',
                    'body': [
                        {
                            'type': {
                                'type': 'IdentifierType_Identifier',
                                'value': 'Person'
                            },
                            'opcode': 'ret',
                            'options': [],
                            'oprds': [
                                {
                                    'type': 'ref',
                                    'value': 'person'
                                },
                            ],
                            'labels': []
                        }
                    ]
                }
            ]
        }

    def ir_closure_2(self):
        return {
            'name': 'compute',
            'parent': 'createPerson',
            'rettype': {
                'type': 'IdentifierType_ValueType',
                'value': 'string'
            },
            'ret_reftype': 'value',
            'parameters': [
                {
                    'identifier': 'lhs_val',
                    'ref_type': 'value',
                    'type': {
                        'type': 'IdentifierType_ValueType',
                        'value': 'ui32'
                    }
                },
                {
                    'identifier': 'rhs_val',
                    'ref_type': 'value',
                    'type': {
                        'type': 'IdentifierType_ValueType',
                        'value': 'dpf'
                    }
                },
                {
                    'identifier': 'values',
                    'ref_type': 'pointer',
                    'type': {
                        'type': 'IdentifierType_Array',
                        'value': {
                            'len': 4,
                            'type': {
                                'type': 'IdentifierType_ValueType',
                                'value': 'dpf'
                            }
                        }
                    }
                }
            ],
            'blocks': [
                {
                    'label': 'entry',
                    'body': [
                        {
                            'target': 'sum',
                            'type': {
                                'type': 'IdentifierType_ValueType',
                                'value': 'ui64'
                            },
                            'opcode': 'add',
                            'options': [],
                            'oprds': [
                                {
                                    'type': 'ref',
                                    'value': 'lhs_val'
                                },
                                {
                                    'type': 'ref',
                                    'value': 'rhs_val'
                                }
                            ]
                        },
                        {
                            'opcode': 'putelement',
                            'options': [],
                            'oprds': [
                                {
                                    'type': 'constant',
                                    'value': {
                                        'type': 'ui8',
                                        'value': 16
                                    }
                                },
                                {
                                    'type': 'ref',
                                    'value': 'values' 
                                },
                                {
                                    'type': 'constant',
                                    'value': {
                                        'type': 'ui32',
                                        'value': 2
                                    }
                                },
                            ]
                        }
                    ]
                }
            ]
        }

    def ir_closure_3(self):
        return {
            'name': 'doNothing',
            'rettype': {
                'type': 'IdentifierType_ValueType',
                'value': 'voidtype'
            },
            'ret_reftype': 'value',
            'parameters': [],
            'blocks': []
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
