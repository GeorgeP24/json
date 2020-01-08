//
// Created by 郭毅 on 2019/12/4.
//

#ifndef MYJSON_MJSON_H
#define MYJSON_MJSON_H

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace mjson {
    using std::string;
    using std::vector;
    using std::nullptr_t;
    enum class type {
        Bool,
        Null,
        String,
        Array,
        Object,
        Number
    };

    class Value;

    class Json final {
    public:
        using _array = std::vector<Json>;
        using _object = std::unordered_map<string, Json>;
    public:
        //cstr
        Json() : Json(nullptr) {};

        Json(int val) : Json(1.0 * val) {};

        Json(const char *cstr) : Json(std::string(cstr)) {};
    public:
        //cstr
        Json(nullptr_t);

        Json(bool val);

        Json(double val);

        Json(const string &val);

        Json(string &&val);

        Json(const _array &val);

        Json(_array &&val);

        Json(const _object &val);

        Json(_object &&val);

        Json(void *) = delete;

        ~Json();

    public:
        template<class M, typename std::enable_if<
                std::is_constructible<std::string, decltype(std::declval<M>().begin()->first)>::value
                && std::is_constructible<Json, decltype(std::declval<M>().begin()->second)>::value,
                int>::type = 0>
        Json(const M &m) : Json(_object(m.begin(), m.end())) {}

        // Implicit constructor: vector-like objects (std::list, std::vector, std::set, etc)
        template<class V, typename std::enable_if<
                std::is_constructible<Json, decltype(*std::declval<V>().begin())>::value,
                int>::type = 0>
        Json(const V &v) : Json(_array(v.begin(), v.end())) {}

    public:
        //copy cstr and move cstr
        Json(const Json &);

        Json &operator=(Json &) noexcept;

        Json(Json &&) noexcept;

        Json &operator=(Json &&) noexcept;

    public:
        static Json parse(const string &content, string err) noexcept;

        string serialize() const noexcept;

    public:
        type getType() const noexcept;

        bool isNull() const noexcept;

        bool isNumber() const noexcept;

        bool isString() const noexcept;

        bool isObject() const noexcept;

        bool isArray() const noexcept;

    public:
        bool toBool() const;

        double toDouble() const;

        const string &toString() const;

        const _array &toArray() const;

        const _object &toObject() const;

    public:
        size_t size() const;

        const Json &operator[](size_t) const;

        Json &operator[](size_t);

        const Json &operator[](const string &) const;

        Json &operator[](const string &);

    private:
        void swap(Json &) noexcept;

        string serializeString() const noexcept;

        string serializeObject() const noexcept;

        string serializeArray() const noexcept;

    private:
        std::unique_ptr<Value> _jsonvalue;
    };

    inline std::ostream &operator<<(std::ostream &os, const Json &json) {
        return os << json.serialize();
    }

    bool operator==(const Json &, const Json &);

    inline bool operator!=(const Json &l, const Json &r) {
        return !(l == r);
    }
}


#endif //MYJSON_MJSON_H
