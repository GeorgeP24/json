//
// Created by 郭毅 on 2019/12/2.
//

#ifndef MYJSON_PARSE_H
#define MYJSON_PARSE_H

# include "mjson.h"
# include "JsonException.h"


namespace mjson {

    constexpr bool is1to9(char ch) { return ch >= '1' && ch <= '9'; }

    constexpr bool is0to9(char ch) { return ch >= '0' && ch <= '9'; }

    class Parser {
    public:
        explicit Parser(const char *cstr) noexcept : _begin(cstr), _end(cstr) {}

        explicit Parser(const string &content) noexcept : _begin(content.c_str()), _end(content.c_str()) {}

    public:
        //copy
        Parser(const Parser &) = delete;

        Parser &operator=(const Parser &) = delete;

    private:
        void parseWhitespace() noexcept;

        unsigned parse4hex();

        string encodeUTF8(unsigned u) noexcept;

        string parseRawString();

        void error(const string &msg) const;

    private:
        Json parseValue();

        Json parseLiteral(const string &literal);

        Json parseNumber();

        Json parseString();

        Json parseArray();

        Json parseObject();

    public:
        Json parse();

    private:
        const char *_begin;
        const char *_end;
    };
}
#endif //MYJSON_PARSE_H
