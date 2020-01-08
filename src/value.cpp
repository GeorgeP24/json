//
// Created by 郭毅 on 2019/12/4.
//

#include "value.h"

namespace mjson {
    using std::holds_alternative;
    type Value::getType() const noexcept {
        if (holds_alternative<nullptr_t>(_val))
            return type::Null;
        else if (holds_alternative<bool>(_val))
            return type::Bool;
        else if (holds_alternative<double>(_val))
            return type::Number;
        else if (holds_alternative<string>(_val))
            return type::String;
        else if (holds_alternative<Json::_array>(_val))
            return type::Array;
        else if (holds_alternative<Json::_object>(_val))
            return type::Object;
    }
    size_t Value::size() const {
        if (std::holds_alternative<Json::_array>(_val))
            return std::get<Json::_array>(_val).size();
        else if (std::holds_alternative<Json::_object>(_val))
            return std::get<Json::_object>(_val).size();
        else
            throw JsonException("not array or object");
    }

    const Json &Value::operator[](size_t pos) const {
        if (std::holds_alternative<Json::_array>(_val))
            return std::get<Json::_array>(_val)[pos];
        else
            throw JsonException("not a array");
    }

    Json &Value::operator[](size_t pos) {
        return const_cast<Json &>(static_cast<const Value &>(*this)[pos]);
    }

    const Json &Value::operator[](const string &str) const {
        if (holds_alternative<Json::_object>(_val))
            return std::get<Json::_object>(_val).at(str);
        else
            throw JsonException("not a object");
    }

    Json &Value::operator[](const string &str) {
        return const_cast<Json &>(static_cast<const Value &>(*this)[str]);
    }

    nullptr_t Value::toNull() const {
        try {
            return std::get<nullptr_t>(_val);
        }
        catch (const std::bad_variant_access &) {
            throw JsonException("not null");
        }
    }
    bool Value::toBool() const {
        try {
            return std::get<bool>(_val);
        }
        catch (const std::bad_variant_access &) {
            throw JsonException("not bool");
        }
    }
    double Value::toDouble() const {
        try {
            return std::get<double>(_val);
        }
        catch (const std::bad_variant_access &) {
            throw JsonException("not double");
        }
    }

    const string &Value::toString() const {
        try {
            return std::get<string>(_val);
        }
        catch (const std::bad_variant_access &) {
            throw JsonException("not string");
        }
    }

    const Json::_array &Value::toArray() const {
        try {
            return std::get<Json::_array>(_val);
        }
        catch (const std::bad_variant_access &) {
            throw JsonException("not array");
        }
    }

    const Json::_object &Value::toObject() const {
        try {
            return std::get<Json::_object>(_val);
        }
        catch (const std::bad_variant_access &) {
            throw JsonException("not object");
        }
    }
}