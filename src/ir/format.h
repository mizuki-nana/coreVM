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


#ifndef SRC_IR_FORMAT_H_1199108877__H_
#define SRC_IR_FORMAT_H_1199108877__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace corevm {
struct IRModuleMeta {
    std::string name;
    int64_t format_version;
    int64_t target_version;
    std::string path;
    std::string author;
    int64_t timestamp;
    IRModuleMeta() :
        name(std::string()),
        format_version(int64_t()),
        target_version(int64_t()),
        path(std::string()),
        author(std::string()),
        timestamp(int64_t())
        { }
};

enum IRValueRefType {
    value,
    pointer,
};

enum IRIdentifierTypeType {
    IdentifierType_Identifier,
    IdentifierType_Array,
    IdentifierType_ValueType,
};

struct IRArrayType;
enum IRValueType {
    voidtype,
    boolean,
    i8,
    ui8,
    i16,
    ui16,
    i32,
    ui32,
    i64,
    ui64,
    spf,
    dpf,
    string,
    object,
};

struct _corevm_ir_schema_json_Union__0__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    std::string get_string() const;
    void set_string(const std::string& v);
    IRArrayType get_IRArrayType() const;
    void set_IRArrayType(const IRArrayType& v);
    IRValueType get_IRValueType() const;
    void set_IRValueType(const IRValueType& v);
    _corevm_ir_schema_json_Union__0__();
};

struct IRIdentifierType {
    typedef _corevm_ir_schema_json_Union__0__ value_t;
    IRIdentifierTypeType type;
    value_t value;
    IRIdentifierType() :
        type(IRIdentifierTypeType()),
        value(value_t())
        { }
};

struct IRArrayType {
    IRIdentifierType type;
    int32_t len;
    IRArrayType() :
        type(IRIdentifierType()),
        len(int32_t())
        { }
};

// enum IRValueType {
//     voidtype,
//     boolean,
//     i8,
//     ui8,
//     i16,
//     ui16,
//     i32,
//     ui32,
//     i64,
//     ui64,
//     spf,
//     dpf,
//     string,
//     object,
// };

struct IRTypeField {
    std::string identifier;
    IRValueRefType ref_type;
    IRIdentifierType type;
    IRTypeField() :
        identifier(std::string()),
        ref_type(IRValueRefType()),
        type(IRIdentifierType())
        { }
};

struct IRTypeDecl {
    std::string name;
    std::vector<IRTypeField > fields;
    IRTypeDecl() :
        name(std::string()),
        fields(std::vector<IRTypeField >())
        { }
};

struct _corevm_ir_schema_json_Union__1__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    std::string get_string() const;
    void set_string(const std::string& v);
    _corevm_ir_schema_json_Union__1__();
};

struct IRParameter {
    std::string identifier;
    IRValueRefType ref_type;
    IRIdentifierType type;
    IRParameter() :
        identifier(std::string()),
        ref_type(IRValueRefType()),
        type(IRIdentifierType())
        { }
};

struct _corevm_ir_schema_json_Union__2__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    std::string get_string() const;
    void set_string(const std::string& v);
    _corevm_ir_schema_json_Union__2__();
};

enum IROpcode {
    alloca,
    load,
    store,
    getattr,
    setattr,
    delattr,
    getelement,
    putelement,
    len,
    ret,
    br,
    switch2,
    pos,
    neg,
    inc,
    dec,
    add,
    sub,
    mul,
    div,
    mod,
    bnot,
    band,
    bor,
    bxor,
    bls,
    brs,
    eq,
    neq,
    gt,
    lt,
    gte,
    lte,
    lnot,
    land,
    lor,
    cmp,
    call,
};

struct _corevm_ir_schema_json_Union__3__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    IRIdentifierType get_IRIdentifierType() const;
    void set_IRIdentifierType(const IRIdentifierType& v);
    _corevm_ir_schema_json_Union__3__();
};

enum IROperandType {
    constant,
    ref,
};

struct _corevm_ir_schema_json_Union__4__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    bool get_bool() const;
    void set_bool(const bool& v);
    int32_t get_int() const;
    void set_int(const int32_t& v);
    int64_t get_long() const;
    void set_long(const int64_t& v);
    float get_float() const;
    void set_float(const float& v);
    double get_double() const;
    void set_double(const double& v);
    std::string get_string() const;
    void set_string(const std::string& v);
    _corevm_ir_schema_json_Union__4__();
};

struct IRValue {
    typedef _corevm_ir_schema_json_Union__4__ value_t;
    IRValueType type;
    value_t value;
    IRValue() :
        type(IRValueType()),
        value(value_t())
        { }
};

struct _corevm_ir_schema_json_Union__5__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    std::string get_string() const;
    void set_string(const std::string& v);
    IRValue get_IRValue() const;
    void set_IRValue(const IRValue& v);
    _corevm_ir_schema_json_Union__5__();
};

struct IROperand {
    typedef _corevm_ir_schema_json_Union__5__ value_t;
    IROperandType type;
    value_t value;
    IROperand() :
        type(IROperandType()),
        value(value_t())
        { }
};

struct IRLabel {
    std::string name;
    IRLabel() :
        name(std::string())
        { }
};

struct _corevm_ir_schema_json_Union__6__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    std::vector<IRLabel > get_array() const;
    void set_array(const std::vector<IRLabel >& v);
    _corevm_ir_schema_json_Union__6__();
};

struct IRInstruction {
    typedef _corevm_ir_schema_json_Union__2__ target_t;
    typedef _corevm_ir_schema_json_Union__3__ type_t;
    typedef _corevm_ir_schema_json_Union__6__ labels_t;
    target_t target;
    IROpcode opcode;
    type_t type;
    std::vector<std::string > options;
    std::vector<IROperand > oprds;
    labels_t labels;
    IRInstruction() :
        target(target_t()),
        opcode(IROpcode()),
        type(type_t()),
        options(std::vector<std::string >()),
        oprds(std::vector<IROperand >()),
        labels(labels_t())
        { }
};

struct IRBasicBlock {
    std::string label;
    std::vector<IRInstruction > body;
    IRBasicBlock() :
        label(std::string()),
        body(std::vector<IRInstruction >())
        { }
};

struct IRClosure {
    typedef _corevm_ir_schema_json_Union__1__ parent_t;
    std::string name;
    parent_t parent;
    IRIdentifierType rettype;
    IRValueRefType ret_reftype;
    std::vector<IRParameter > parameters;
    std::vector<IRBasicBlock > blocks;
    IRClosure() :
        name(std::string()),
        parent(parent_t()),
        rettype(IRIdentifierType()),
        ret_reftype(IRValueRefType()),
        parameters(std::vector<IRParameter >()),
        blocks(std::vector<IRBasicBlock >())
        { }
};

struct IRModule {
    IRModuleMeta meta;
    std::vector<IRTypeDecl > types;
    std::vector<IRClosure > closures;
    IRModule() :
        meta(IRModuleMeta()),
        types(std::vector<IRTypeDecl >()),
        closures(std::vector<IRClosure >())
        { }
};

inline
std::string _corevm_ir_schema_json_Union__0__::get_string() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::string >(value_);
}

inline
void _corevm_ir_schema_json_Union__0__::set_string(const std::string& v) {
    idx_ = 0;
    value_ = v;
}

inline
IRArrayType _corevm_ir_schema_json_Union__0__::get_IRArrayType() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<IRArrayType >(value_);
}

inline
void _corevm_ir_schema_json_Union__0__::set_IRArrayType(const IRArrayType& v) {
    idx_ = 1;
    value_ = v;
}

inline
IRValueType _corevm_ir_schema_json_Union__0__::get_IRValueType() const {
    if (idx_ != 2) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<IRValueType >(value_);
}

inline
void _corevm_ir_schema_json_Union__0__::set_IRValueType(const IRValueType& v) {
    idx_ = 2;
    value_ = v;
}

inline
std::string _corevm_ir_schema_json_Union__1__::get_string() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::string >(value_);
}

inline
void _corevm_ir_schema_json_Union__1__::set_string(const std::string& v) {
    idx_ = 1;
    value_ = v;
}

inline
std::string _corevm_ir_schema_json_Union__2__::get_string() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::string >(value_);
}

inline
void _corevm_ir_schema_json_Union__2__::set_string(const std::string& v) {
    idx_ = 1;
    value_ = v;
}

inline
IRIdentifierType _corevm_ir_schema_json_Union__3__::get_IRIdentifierType() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<IRIdentifierType >(value_);
}

inline
void _corevm_ir_schema_json_Union__3__::set_IRIdentifierType(const IRIdentifierType& v) {
    idx_ = 1;
    value_ = v;
}

inline
bool _corevm_ir_schema_json_Union__4__::get_bool() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<bool >(value_);
}

inline
void _corevm_ir_schema_json_Union__4__::set_bool(const bool& v) {
    idx_ = 1;
    value_ = v;
}

inline
int32_t _corevm_ir_schema_json_Union__4__::get_int() const {
    if (idx_ != 2) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<int32_t >(value_);
}

inline
void _corevm_ir_schema_json_Union__4__::set_int(const int32_t& v) {
    idx_ = 2;
    value_ = v;
}

inline
int64_t _corevm_ir_schema_json_Union__4__::get_long() const {
    if (idx_ != 3) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<int64_t >(value_);
}

inline
void _corevm_ir_schema_json_Union__4__::set_long(const int64_t& v) {
    idx_ = 3;
    value_ = v;
}

inline
float _corevm_ir_schema_json_Union__4__::get_float() const {
    if (idx_ != 4) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<float >(value_);
}

inline
void _corevm_ir_schema_json_Union__4__::set_float(const float& v) {
    idx_ = 4;
    value_ = v;
}

inline
double _corevm_ir_schema_json_Union__4__::get_double() const {
    if (idx_ != 5) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<double >(value_);
}

inline
void _corevm_ir_schema_json_Union__4__::set_double(const double& v) {
    idx_ = 5;
    value_ = v;
}

inline
std::string _corevm_ir_schema_json_Union__4__::get_string() const {
    if (idx_ != 6) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::string >(value_);
}

inline
void _corevm_ir_schema_json_Union__4__::set_string(const std::string& v) {
    idx_ = 6;
    value_ = v;
}

inline
std::string _corevm_ir_schema_json_Union__5__::get_string() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::string >(value_);
}

inline
void _corevm_ir_schema_json_Union__5__::set_string(const std::string& v) {
    idx_ = 0;
    value_ = v;
}

inline
IRValue _corevm_ir_schema_json_Union__5__::get_IRValue() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<IRValue >(value_);
}

inline
void _corevm_ir_schema_json_Union__5__::set_IRValue(const IRValue& v) {
    idx_ = 1;
    value_ = v;
}

inline
std::vector<IRLabel > _corevm_ir_schema_json_Union__6__::get_array() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::vector<IRLabel > >(value_);
}

inline
void _corevm_ir_schema_json_Union__6__::set_array(const std::vector<IRLabel >& v) {
    idx_ = 1;
    value_ = v;
}

inline _corevm_ir_schema_json_Union__0__::_corevm_ir_schema_json_Union__0__() : idx_(0), value_(std::string()) { }
inline _corevm_ir_schema_json_Union__1__::_corevm_ir_schema_json_Union__1__() : idx_(0) { }
inline _corevm_ir_schema_json_Union__2__::_corevm_ir_schema_json_Union__2__() : idx_(0) { }
inline _corevm_ir_schema_json_Union__3__::_corevm_ir_schema_json_Union__3__() : idx_(0) { }
inline _corevm_ir_schema_json_Union__4__::_corevm_ir_schema_json_Union__4__() : idx_(0) { }
inline _corevm_ir_schema_json_Union__5__::_corevm_ir_schema_json_Union__5__() : idx_(0), value_(std::string()) { }
inline _corevm_ir_schema_json_Union__6__::_corevm_ir_schema_json_Union__6__() : idx_(0) { }
}
namespace avro {
template<> struct codec_traits<corevm::IRModuleMeta> {
    static void encode(Encoder& e, const corevm::IRModuleMeta& v) {
        avro::encode(e, v.name);
        avro::encode(e, v.format_version);
        avro::encode(e, v.target_version);
        avro::encode(e, v.path);
        avro::encode(e, v.author);
        avro::encode(e, v.timestamp);
    }
    static void decode(Decoder& d, corevm::IRModuleMeta& v) {
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
                    avro::decode(d, v.format_version);
                    break;
                case 2:
                    avro::decode(d, v.target_version);
                    break;
                case 3:
                    avro::decode(d, v.path);
                    break;
                case 4:
                    avro::decode(d, v.author);
                    break;
                case 5:
                    avro::decode(d, v.timestamp);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.name);
            avro::decode(d, v.format_version);
            avro::decode(d, v.target_version);
            avro::decode(d, v.path);
            avro::decode(d, v.author);
            avro::decode(d, v.timestamp);
        }
    }
};

template<> struct codec_traits<corevm::IRValueRefType> {
    static void encode(Encoder& e, corevm::IRValueRefType v) {
		if (v < corevm::value || v > corevm::pointer)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for corevm::IRValueRefType and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, corevm::IRValueRefType& v) {
		size_t index = d.decodeEnum();
		if (index < corevm::value || index > corevm::pointer)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for corevm::IRValueRefType and cannot be decoded";
			throw avro::Exception(error.str());
		}
        v = static_cast<corevm::IRValueRefType>(index);
    }
};

template<> struct codec_traits<corevm::IRIdentifierTypeType> {
    static void encode(Encoder& e, corevm::IRIdentifierTypeType v) {
		if (v < corevm::IdentifierType_Identifier || v > corevm::IdentifierType_ValueType)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for corevm::IRIdentifierTypeType and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, corevm::IRIdentifierTypeType& v) {
		size_t index = d.decodeEnum();
		if (index < corevm::IdentifierType_Identifier || index > corevm::IdentifierType_ValueType)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for corevm::IRIdentifierTypeType and cannot be decoded";
			throw avro::Exception(error.str());
		}
        v = static_cast<corevm::IRIdentifierTypeType>(index);
    }
};

template<> struct codec_traits<corevm::IRArrayType> {
    static void encode(Encoder& e, const corevm::IRArrayType& v) {
        avro::encode(e, v.type);
        avro::encode(e, v.len);
    }
    static void decode(Decoder& d, corevm::IRArrayType& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.type);
                    break;
                case 1:
                    avro::decode(d, v.len);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.type);
            avro::decode(d, v.len);
        }
    }
};

template<> struct codec_traits<corevm::IRValueType> {
    static void encode(Encoder& e, corevm::IRValueType v) {
		if (v < corevm::voidtype || v > corevm::object)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for corevm::IRValueType and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, corevm::IRValueType& v) {
		size_t index = d.decodeEnum();
		if (index < corevm::voidtype || index > corevm::object)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for corevm::IRValueType and cannot be decoded";
			throw avro::Exception(error.str());
		}
        v = static_cast<corevm::IRValueType>(index);
    }
};

template<> struct codec_traits<corevm::_corevm_ir_schema_json_Union__0__> {
    static void encode(Encoder& e, corevm::_corevm_ir_schema_json_Union__0__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_string());
            break;
        case 1:
            avro::encode(e, v.get_IRArrayType());
            break;
        case 2:
            avro::encode(e, v.get_IRValueType());
            break;
        }
    }
    static void decode(Decoder& d, corevm::_corevm_ir_schema_json_Union__0__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 3) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                std::string vv;
                avro::decode(d, vv);
                v.set_string(vv);
            }
            break;
        case 1:
            {
                corevm::IRArrayType vv;
                avro::decode(d, vv);
                v.set_IRArrayType(vv);
            }
            break;
        case 2:
            {
                corevm::IRValueType vv;
                avro::decode(d, vv);
                v.set_IRValueType(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<corevm::IRIdentifierType> {
    static void encode(Encoder& e, const corevm::IRIdentifierType& v) {
        avro::encode(e, v.type);
        avro::encode(e, v.value);
    }
    static void decode(Decoder& d, corevm::IRIdentifierType& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.type);
                    break;
                case 1:
                    avro::decode(d, v.value);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.type);
            avro::decode(d, v.value);
        }
    }
};

template<> struct codec_traits<corevm::IRTypeField> {
    static void encode(Encoder& e, const corevm::IRTypeField& v) {
        avro::encode(e, v.identifier);
        avro::encode(e, v.ref_type);
        avro::encode(e, v.type);
    }
    static void decode(Decoder& d, corevm::IRTypeField& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.identifier);
                    break;
                case 1:
                    avro::decode(d, v.ref_type);
                    break;
                case 2:
                    avro::decode(d, v.type);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.identifier);
            avro::decode(d, v.ref_type);
            avro::decode(d, v.type);
        }
    }
};

template<> struct codec_traits<corevm::IRTypeDecl> {
    static void encode(Encoder& e, const corevm::IRTypeDecl& v) {
        avro::encode(e, v.name);
        avro::encode(e, v.fields);
    }
    static void decode(Decoder& d, corevm::IRTypeDecl& v) {
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
                    avro::decode(d, v.fields);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.name);
            avro::decode(d, v.fields);
        }
    }
};

template<> struct codec_traits<corevm::_corevm_ir_schema_json_Union__1__> {
    static void encode(Encoder& e, corevm::_corevm_ir_schema_json_Union__1__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_string());
            break;
        }
    }
    static void decode(Decoder& d, corevm::_corevm_ir_schema_json_Union__1__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                std::string vv;
                avro::decode(d, vv);
                v.set_string(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<corevm::IRParameter> {
    static void encode(Encoder& e, const corevm::IRParameter& v) {
        avro::encode(e, v.identifier);
        avro::encode(e, v.ref_type);
        avro::encode(e, v.type);
    }
    static void decode(Decoder& d, corevm::IRParameter& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.identifier);
                    break;
                case 1:
                    avro::decode(d, v.ref_type);
                    break;
                case 2:
                    avro::decode(d, v.type);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.identifier);
            avro::decode(d, v.ref_type);
            avro::decode(d, v.type);
        }
    }
};

template<> struct codec_traits<corevm::_corevm_ir_schema_json_Union__2__> {
    static void encode(Encoder& e, corevm::_corevm_ir_schema_json_Union__2__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_string());
            break;
        }
    }
    static void decode(Decoder& d, corevm::_corevm_ir_schema_json_Union__2__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                std::string vv;
                avro::decode(d, vv);
                v.set_string(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<corevm::IROpcode> {
    static void encode(Encoder& e, corevm::IROpcode v) {
		if (v < corevm::alloca || v > corevm::call)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for corevm::IROpcode and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, corevm::IROpcode& v) {
		size_t index = d.decodeEnum();
		if (index < corevm::alloca || index > corevm::call)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for corevm::IROpcode and cannot be decoded";
			throw avro::Exception(error.str());
		}
        v = static_cast<corevm::IROpcode>(index);
    }
};

template<> struct codec_traits<corevm::_corevm_ir_schema_json_Union__3__> {
    static void encode(Encoder& e, corevm::_corevm_ir_schema_json_Union__3__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_IRIdentifierType());
            break;
        }
    }
    static void decode(Decoder& d, corevm::_corevm_ir_schema_json_Union__3__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                corevm::IRIdentifierType vv;
                avro::decode(d, vv);
                v.set_IRIdentifierType(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<corevm::IROperandType> {
    static void encode(Encoder& e, corevm::IROperandType v) {
		if (v < corevm::constant || v > corevm::ref)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for corevm::IROperandType and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, corevm::IROperandType& v) {
		size_t index = d.decodeEnum();
		if (index < corevm::constant || index > corevm::ref)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for corevm::IROperandType and cannot be decoded";
			throw avro::Exception(error.str());
		}
        v = static_cast<corevm::IROperandType>(index);
    }
};

template<> struct codec_traits<corevm::_corevm_ir_schema_json_Union__4__> {
    static void encode(Encoder& e, corevm::_corevm_ir_schema_json_Union__4__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_bool());
            break;
        case 2:
            avro::encode(e, v.get_int());
            break;
        case 3:
            avro::encode(e, v.get_long());
            break;
        case 4:
            avro::encode(e, v.get_float());
            break;
        case 5:
            avro::encode(e, v.get_double());
            break;
        case 6:
            avro::encode(e, v.get_string());
            break;
        }
    }
    static void decode(Decoder& d, corevm::_corevm_ir_schema_json_Union__4__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 7) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                bool vv;
                avro::decode(d, vv);
                v.set_bool(vv);
            }
            break;
        case 2:
            {
                int32_t vv;
                avro::decode(d, vv);
                v.set_int(vv);
            }
            break;
        case 3:
            {
                int64_t vv;
                avro::decode(d, vv);
                v.set_long(vv);
            }
            break;
        case 4:
            {
                float vv;
                avro::decode(d, vv);
                v.set_float(vv);
            }
            break;
        case 5:
            {
                double vv;
                avro::decode(d, vv);
                v.set_double(vv);
            }
            break;
        case 6:
            {
                std::string vv;
                avro::decode(d, vv);
                v.set_string(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<corevm::IRValue> {
    static void encode(Encoder& e, const corevm::IRValue& v) {
        avro::encode(e, v.type);
        avro::encode(e, v.value);
    }
    static void decode(Decoder& d, corevm::IRValue& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.type);
                    break;
                case 1:
                    avro::decode(d, v.value);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.type);
            avro::decode(d, v.value);
        }
    }
};

template<> struct codec_traits<corevm::_corevm_ir_schema_json_Union__5__> {
    static void encode(Encoder& e, corevm::_corevm_ir_schema_json_Union__5__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_string());
            break;
        case 1:
            avro::encode(e, v.get_IRValue());
            break;
        }
    }
    static void decode(Decoder& d, corevm::_corevm_ir_schema_json_Union__5__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                std::string vv;
                avro::decode(d, vv);
                v.set_string(vv);
            }
            break;
        case 1:
            {
                corevm::IRValue vv;
                avro::decode(d, vv);
                v.set_IRValue(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<corevm::IROperand> {
    static void encode(Encoder& e, const corevm::IROperand& v) {
        avro::encode(e, v.type);
        avro::encode(e, v.value);
    }
    static void decode(Decoder& d, corevm::IROperand& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.type);
                    break;
                case 1:
                    avro::decode(d, v.value);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.type);
            avro::decode(d, v.value);
        }
    }
};

template<> struct codec_traits<corevm::IRLabel> {
    static void encode(Encoder& e, const corevm::IRLabel& v) {
        avro::encode(e, v.name);
    }
    static void decode(Decoder& d, corevm::IRLabel& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.name);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.name);
        }
    }
};

template<> struct codec_traits<corevm::_corevm_ir_schema_json_Union__6__> {
    static void encode(Encoder& e, corevm::_corevm_ir_schema_json_Union__6__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_array());
            break;
        }
    }
    static void decode(Decoder& d, corevm::_corevm_ir_schema_json_Union__6__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                std::vector<corevm::IRLabel > vv;
                avro::decode(d, vv);
                v.set_array(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<corevm::IRInstruction> {
    static void encode(Encoder& e, const corevm::IRInstruction& v) {
        avro::encode(e, v.target);
        avro::encode(e, v.opcode);
        avro::encode(e, v.type);
        avro::encode(e, v.options);
        avro::encode(e, v.oprds);
        avro::encode(e, v.labels);
    }
    static void decode(Decoder& d, corevm::IRInstruction& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.target);
                    break;
                case 1:
                    avro::decode(d, v.opcode);
                    break;
                case 2:
                    avro::decode(d, v.type);
                    break;
                case 3:
                    avro::decode(d, v.options);
                    break;
                case 4:
                    avro::decode(d, v.oprds);
                    break;
                case 5:
                    avro::decode(d, v.labels);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.target);
            avro::decode(d, v.opcode);
            avro::decode(d, v.type);
            avro::decode(d, v.options);
            avro::decode(d, v.oprds);
            avro::decode(d, v.labels);
        }
    }
};

template<> struct codec_traits<corevm::IRBasicBlock> {
    static void encode(Encoder& e, const corevm::IRBasicBlock& v) {
        avro::encode(e, v.label);
        avro::encode(e, v.body);
    }
    static void decode(Decoder& d, corevm::IRBasicBlock& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.label);
                    break;
                case 1:
                    avro::decode(d, v.body);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.label);
            avro::decode(d, v.body);
        }
    }
};

template<> struct codec_traits<corevm::IRClosure> {
    static void encode(Encoder& e, const corevm::IRClosure& v) {
        avro::encode(e, v.name);
        avro::encode(e, v.parent);
        avro::encode(e, v.rettype);
        avro::encode(e, v.ret_reftype);
        avro::encode(e, v.parameters);
        avro::encode(e, v.blocks);
    }
    static void decode(Decoder& d, corevm::IRClosure& v) {
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
                    avro::decode(d, v.parent);
                    break;
                case 2:
                    avro::decode(d, v.rettype);
                    break;
                case 3:
                    avro::decode(d, v.ret_reftype);
                    break;
                case 4:
                    avro::decode(d, v.parameters);
                    break;
                case 5:
                    avro::decode(d, v.blocks);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.name);
            avro::decode(d, v.parent);
            avro::decode(d, v.rettype);
            avro::decode(d, v.ret_reftype);
            avro::decode(d, v.parameters);
            avro::decode(d, v.blocks);
        }
    }
};

template<> struct codec_traits<corevm::IRModule> {
    static void encode(Encoder& e, const corevm::IRModule& v) {
        avro::encode(e, v.meta);
        avro::encode(e, v.types);
        avro::encode(e, v.closures);
    }
    static void decode(Decoder& d, corevm::IRModule& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.meta);
                    break;
                case 1:
                    avro::decode(d, v.types);
                    break;
                case 2:
                    avro::decode(d, v.closures);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.meta);
            avro::decode(d, v.types);
            avro::decode(d, v.closures);
        }
    }
};

}
#endif
