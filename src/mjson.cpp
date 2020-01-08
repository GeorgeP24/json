//
// Created by 郭毅 on 2019/12/4.
//

#include "mjson.h"
#include "value.h"
#include "parse.h"

namespace mjson {
    using std::make_unique;

    Json::Json(nullptr_t) {
        _jsonvalue = make_unique<Value>(nullptr);
    }

    Json::Json(bool val) {
        _jsonvalue = make_unique<Value>(val);
    }

    Json::Json(double val) {
        _jsonvalue = make_unique<Value>(val);
    }

    Json::Json(const string &val) {
        _jsonvalue = make_unique<Value>(val);

    }

    Json::Json(string &&val) {
        _jsonvalue = make_unique<Value>(std::move(val));
    }

    Json::Json(const Json::_array &val) {
        _jsonvalue = make_unique<Value>(val);
    }

    Json::Json(Json::_array &&val) {
        _jsonvalue = make_unique<Value>(std::move(val));
    }

    Json::Json(const Json::_object &val) {
        _jsonvalue = make_unique<Value>(val);
    }

    Json::Json(Json::_object &&val) {

    }

    Json::Json(const Json &rhs) {
        switch (rhs.getType()) {
            case type::Null:
                _jsonvalue = make_unique<Value>(nullptr);
                break;
            case type::Number:
                _jsonvalue = make_unique<Value>(rhs.toDouble());
                break;
            case type::Array:
                _jsonvalue = make_unique<Value>(rhs.toArray());
                break;
            case type::Bool:
                _jsonvalue = make_unique<Value>(rhs.toBool());
                break;
            case type::String:
                _jsonvalue = make_unique<Value>(rhs.toString());
                break;
            case type::Object:
                _jsonvalue = make_unique<Value>(rhs.toObject());
                break;
        }
    }

    Json &Json::operator=(Json &rhs) noexcept {
        Json temp(rhs);
        swap(temp);
        return *this;
    }

    Json::Json(Json &&) noexcept = default;

    Json &Json::operator=(Json &&) noexcept = default;


    Json Json::parse(const string &content, string err) noexcept {
        try {
            Parser p(content);
            return p.parse();
        }
        catch (JsonException &e) {
            err = e.what();
            return Json(nullptr);
        }
    }

    string Json::serialize() const noexcept {
        switch (_jsonvalue->getType()) {
            case type::Null:
                return "null";
            case type::Bool:
                return _jsonvalue->toBool() ? "true" : "false";
            case type::Number:
                char buf[32];
                snprintf(buf, sizeof(buf), "%.17g", _jsonvalue->toDouble());
                return buf;
            case type::String:
                return serializeString();
            case type::Array:
                return serializeArray();
            case type::Object:
                return serializeObject();
        }
    }

    type Json::getType() const noexcept {
        return _jsonvalue->getType();
    }

    bool Json::isNull() const noexcept {
        return getType() == type::Null;
    }

    bool Json::isNumber() const noexcept {
        return getType() == type::Number;
    }

    bool Json::isString() const noexcept {
        return getType() == type::String;
    }

    bool Json::isObject() const noexcept {
        return getType() == type::Object;
    }

    bool Json::isArray() const noexcept {
        return getType() == type::Array;
    }

    bool Json::toBool() const {
        return _jsonvalue->toBool();
    }

    double Json::toDouble() const {
        return _jsonvalue->toDouble();
    }

    const string &Json::toString() const {
        return _jsonvalue->toString();
    }

    const Json::_array &Json::toArray() const {
        return _jsonvalue->toArray();
    }

    const Json::_object &Json::toObject() const {
        return _jsonvalue->toObject();
    }

    size_t Json::size() const {
        return _jsonvalue->size();
    }

    const Json &Json::operator[](size_t pos) const {
        return _jsonvalue->operator[](pos);;
    }

    Json &Json::operator[](size_t pos) {
        return _jsonvalue->operator[](pos);
    }

    const Json &Json::operator[](const string &str) const {
        return _jsonvalue->operator[](str);
    }

    Json &Json::operator[](const string &str) {
        return _jsonvalue->operator[](str);
    }

    void Json::swap(Json &rhs) noexcept {
        using std::swap;
        swap(_jsonvalue, rhs._jsonvalue);
    }

    string Json::serializeString() const noexcept {
        std::string res = "\"";
        for (auto e : _jsonvalue->toString()) {
            switch (e) {
                case '\"':
                    res += "\\\"";
                    break;
                case '\\':
                    res += "\\\\";
                    break;
                case '\b':
                    res += "\\b";
                    break;
                case '\f':
                    res += "\\f";
                    break;
                case '\n':
                    res += "\\n";
                    break;
                case '\r':
                    res += "\\r";
                    break;
                case '\t':
                    res += "\\t";
                    break;
                default:
                    if (static_cast<unsigned char>(e) < 0x20) {
                        char buf[7];
                        sprintf(buf, "\\u%04X", e);
                        res += buf;
                    } else
                        res += e;
            }
        }
        return res + '"';
    }

    string Json::serializeObject() const noexcept {
        std::string res = "{ ";
        bool first = 1;// indicate now is the first object
        for (const std::pair<std::string, Json> &p : _jsonvalue->toObject()) {
            if (first)
                first = 0;
            else
                res += ", ";
            res += "\"" + p.first + "\"";
            res += ": ";
            res += p.second.serialize();
        }
        return res + " }";
    }

    string Json::serializeArray() const noexcept {
        std::string res = "[ ";
        for (size_t i = 0; i != _jsonvalue->size(); ++i) {
            if (i > 0)
                res += ", ";
            res += (*this)[i].serialize();
        }
        return res + " ]";
    }

    Json::~Json() = default;

    bool operator==(const Json &lhs, const Json &rhs) {
        if (lhs.getType() != rhs.getType())
            return false;
        switch (lhs.getType()) {
            case type::Null:
                return true;
            case type::Bool:
                return lhs.toBool() == rhs.toBool();
            case type::Number:
                return lhs.toDouble() == rhs.toDouble();
            case type::String:
                return lhs.toString() == rhs.toString();
            case type::Array:
                return lhs.toArray() == rhs.toArray();
            default:
                return lhs.toObject() == rhs.toObject();
        }
    }
}