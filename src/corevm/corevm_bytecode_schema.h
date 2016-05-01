/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef COREVM_BYTECODE_SCHEMA_H_2807634017__H_
#define COREVM_BYTECODE_SCHEMA_H_2807634017__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace corevm {
struct instr {
    int64_t code;
    int64_t oprd1;
    int64_t oprd2;
    instr() :
        code(int64_t()),
        oprd1(int64_t()),
        oprd2(int64_t())
        { }

    // Manually added.
    instr(int64_t code_, int64_t oprd1_, int64_t oprd2_)
      :
      code(code_),
      oprd1(oprd1_),
      oprd2(oprd2_)
    {
    }
};

struct loc {
    int64_t index;
    int64_t lineno;
    int64_t col_offset;
    loc() :
        index(int64_t()),
        lineno(int64_t()),
        col_offset(int64_t())
        { }

    // Manually added.
    loc(int64_t index_, int64_t lineno_, int64_t col_offset_)
      :
      index(index_),
      lineno(lineno_),
      col_offset(col_offset_)
    {
    }
};

struct catch_site {
    int64_t from;
    int64_t to;
    int64_t dst;
    catch_site() :
        from(int64_t()),
        to(int64_t()),
        dst(int64_t())
        { }

    // Manually added.
    catch_site(int64_t from_, int64_t to_, int64_t dst_)
      :
      from(from_),
      to(to_),
      dst(dst_)
    {
    }
};

struct closure {
    std::string name;
    int64_t id;
    int64_t parent_id;
    std::vector<instr > vector;
    std::vector<loc > locs;
    std::vector<catch_site > catch_sites;
    closure() :
        name(std::string()),
        id(int64_t(-1)),
        parent_id(int64_t(-1)),
        vector(std::vector<instr >()),
        locs(std::vector<loc >()),
        catch_sites(std::vector<catch_site >())
        { }

    // Manually added.
    closure(
        std::string name_,
        int64_t id_,
        int64_t parent_id_,
        std::vector<instr > vector_,
        std::vector<loc > locs_,
        std::vector<catch_site > catch_sites_)
        :
        name(name_),
        id(id_),
        parent_id(parent_id_),
        vector(vector_),
        locs(locs_),
        catch_sites(catch_sites_)
      {
      }
};

struct bytecode {
    std::string format;
    std::string format_version;
    std::string target_version;
    std::string path;
    int64_t timestamp;
    std::string encoding;
    std::string author;
    std::vector<std::string > string_literal_table;
    std::vector<double > fpt_literal_table;
    std::vector<closure > __MAIN__;
    bytecode() :
        format(std::string()),
        format_version(std::string()),
        target_version(std::string()),
        path(std::string()),
        timestamp(int64_t()),
        encoding(std::string()),
        author(std::string()),
        string_literal_table(std::vector<std::string >()),
        fpt_literal_table(std::vector<double >()),
        __MAIN__(std::vector<closure >())
        { }
};

}
namespace avro {
template<> struct codec_traits<corevm::instr> {
    static void encode(Encoder& e, const corevm::instr& v) {
        avro::encode(e, v.code);
        avro::encode(e, v.oprd1);
        avro::encode(e, v.oprd2);
    }
    static void decode(Decoder& d, corevm::instr& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.code);
                    break;
                case 1:
                    avro::decode(d, v.oprd1);
                    break;
                case 2:
                    avro::decode(d, v.oprd2);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.code);
            avro::decode(d, v.oprd1);
            avro::decode(d, v.oprd2);
        }
    }
};

template<> struct codec_traits<corevm::loc> {
    static void encode(Encoder& e, const corevm::loc& v) {
        avro::encode(e, v.index);
        avro::encode(e, v.lineno);
        avro::encode(e, v.col_offset);
    }
    static void decode(Decoder& d, corevm::loc& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.index);
                    break;
                case 1:
                    avro::decode(d, v.lineno);
                    break;
                case 2:
                    avro::decode(d, v.col_offset);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.index);
            avro::decode(d, v.lineno);
            avro::decode(d, v.col_offset);
        }
    }
};

template<> struct codec_traits<corevm::catch_site> {
    static void encode(Encoder& e, const corevm::catch_site& v) {
        avro::encode(e, v.from);
        avro::encode(e, v.to);
        avro::encode(e, v.dst);
    }
    static void decode(Decoder& d, corevm::catch_site& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.from);
                    break;
                case 1:
                    avro::decode(d, v.to);
                    break;
                case 2:
                    avro::decode(d, v.dst);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.from);
            avro::decode(d, v.to);
            avro::decode(d, v.dst);
        }
    }
};

template<> struct codec_traits<corevm::closure> {
    static void encode(Encoder& e, const corevm::closure& v) {
        avro::encode(e, v.name);
        avro::encode(e, v.id);
        avro::encode(e, v.parent_id);
        avro::encode(e, v.vector);
        avro::encode(e, v.locs);
        avro::encode(e, v.catch_sites);
    }
    static void decode(Decoder& d, corevm::closure& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.name);
                    break;
                case 1:
                    avro::decode(d, v.id);
                    break;
                case 2:
                    avro::decode(d, v.parent_id);
                    break;
                case 3:
                    avro::decode(d, v.vector);
                    break;
                case 4:
                    avro::decode(d, v.locs);
                    break;
                case 5:
                    avro::decode(d, v.catch_sites);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.name);
            avro::decode(d, v.id);
            avro::decode(d, v.parent_id);
            avro::decode(d, v.vector);
            avro::decode(d, v.locs);
            avro::decode(d, v.catch_sites);
        }
    }
};

template<> struct codec_traits<corevm::bytecode> {
    static void encode(Encoder& e, const corevm::bytecode& v) {
        avro::encode(e, v.format);
        avro::encode(e, v.format_version);
        avro::encode(e, v.target_version);
        avro::encode(e, v.path);
        avro::encode(e, v.timestamp);
        avro::encode(e, v.encoding);
        avro::encode(e, v.author);
        avro::encode(e, v.string_literal_table);
        avro::encode(e, v.fpt_literal_table);
        avro::encode(e, v.__MAIN__);
    }
    static void decode(Decoder& d, corevm::bytecode& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.format);
                    break;
                case 1:
                    avro::decode(d, v.format_version);
                    break;
                case 2:
                    avro::decode(d, v.target_version);
                    break;
                case 3:
                    avro::decode(d, v.path);
                    break;
                case 4:
                    avro::decode(d, v.timestamp);
                    break;
                case 5:
                    avro::decode(d, v.encoding);
                    break;
                case 6:
                    avro::decode(d, v.author);
                    break;
                case 7:
                    avro::decode(d, v.string_literal_table);
                    break;
                case 8:
                    avro::decode(d, v.fpt_literal_table);
                    break;
                case 9:
                    avro::decode(d, v.__MAIN__);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.format);
            avro::decode(d, v.format_version);
            avro::decode(d, v.target_version);
            avro::decode(d, v.path);
            avro::decode(d, v.timestamp);
            avro::decode(d, v.encoding);
            avro::decode(d, v.author);
            avro::decode(d, v.string_literal_table);
            avro::decode(d, v.fpt_literal_table);
            avro::decode(d, v.__MAIN__);
        }
    }
};

}
#endif
