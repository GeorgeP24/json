//
// Created by 郭毅 on 2019/12/4.
//

#ifndef MYJSON_VALUE_H
#define MYJSON_VALUE_H

#include "mjson.h"
#include "jsonexception.h"
#include <variant>

namespace mjson {
    using std::variant;

    class Value {
    public:
        explicit Value(nullptr_t) : _val(nullptr) {}

        explicit Value(bool val) : _val(val) {}

        explicit Value(double val) : _val(val) {}

        explicit Value(const string &val) : _val(val) {}

        explicit Value(const Json::_object &val) : _val(val) {}

        explicit Value(const Json::_array &val) : _val(val) {}


    public:
        explicit Value(string &&val) : _val(std::move(val)) {}

        explicit Value(Json::_array &&val) : _val(std::move(val)) {}

        explicit Value(Json::_object &&val) : _val(std::move(val)) {}

        ~Value() = default;


    public:
        type getType() const noexcept;

    public:
        size_t size() const;

        const Json &operator[](size_t) const;

        Json &operator[](size_t);

        const Json &operator[](const string &) const;

        Json &operator[](const string &);

    public:
        nullptr_t toNull() const;

        bool toBool() const;

        double toDouble() const;

        const string &toString() const;

        const Json::_array &toArray() const;

        const Json::_object &toObject() const;

    private:
        variant<nullptr_t, bool, double, string, Json::_array, Json::_object> _val;
    };
}

#endif //MYJSON_VALUE_H
