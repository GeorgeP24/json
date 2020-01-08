//
// Created by 郭毅 on 2019/12/2.
//

#include "../include/parse.h"
#include <cassert>

namespace mjson {
    void mjson::Parser::parseWhitespace() noexcept {
        while (*_end == ' ' || *_end == '\t' || *_end == '\r' || *_end == '\n')
            ++_end;
        _begin = _end;
        string a;
    }

    unsigned mjson::Parser::parse4hex() {
        unsigned u = 0;
        for (int i = 0; i != 4; ++i) {
            auto ch = static_cast<unsigned>(toupper(*++_end));
            u <<= 4;
            if (ch >= '0' && ch <= '9')
                u |= (ch - '0');
            else if (ch >= 'A' && ch <= 'F')
                u |= ch - 'A' + 10;
            else
                error("INVALID UNICODE HEX");
        }
        return u;
    }

    std::string mjson::Parser::encodeUTF8(unsigned u) noexcept {
        std::string utf8;
        if (u <= 0x7F)  // 0111,1111
            utf8.push_back(static_cast<char>(u & 0xff));
        else if (u <= 0x7FF) {
            utf8.push_back(static_cast<char>(0xc0 | ((u >> 6) & 0xff)));
            utf8.push_back(static_cast<char>(0x80 | (u & 0x3f)));
        } else if (u <= 0xFFFF) {
            utf8.push_back(static_cast<char>(0xe0 | ((u >> 12) & 0xff)));
            utf8.push_back(static_cast<char>(0x80 | ((u >> 6) & 0x3f)));
            utf8.push_back(static_cast<char>(0x80 | (u & 0x3f)));
        } else {
            assert(u <= 0x10FFFF);
            utf8.push_back(static_cast<char>(0xf0 | ((u >> 18) & 0xff)));
            utf8.push_back(static_cast<char>(0x80 | ((u >> 12) & 0x3f)));
            utf8.push_back(static_cast<char>(0x80 | ((u >> 6) & 0x3f)));
            utf8.push_back(static_cast<char>(0x80 | (u & 0x3f)));
        }
        return utf8;
    }

    std::string mjson::Parser::parseRawString() {
        std::string str;
        while (1) {
            switch (*++_end) {
                case '\"':
                    _begin = ++_end;
                    return str;
                case '\0':
                    error("MISS QUOTATION MARK");
                default:
                    if (static_cast<unsigned char>(*_end) < 0x20)
                        error("INVALID STRING CHAR");
                    str.push_back(*_end);
                    break;
                case '\\':
                    switch (*++_end) {
                        case '\"':
                            str.push_back('\"');
                            break;
                        case '\\':
                            str.push_back('\\');
                            break;
                        case '/':
                            str.push_back('/');
                            break;
                        case 'b':
                            str.push_back('\b');
                            break;
                        case 'f':
                            str.push_back('\f');
                            break;
                        case 'n':
                            str.push_back('\n');
                            break;
                        case 't':
                            str.push_back('\t');
                            break;
                        case 'r':
                            str.push_back('\r');
                            break;
                        case 'u': {
                            unsigned u1 = parse4hex();
                            if (u1 >= 0xd800 && u1 <= 0xdbff) {  // high surrogate
                                if (*++_end != '\\')
                                    error("INVALID UNICODE SURROGATE");
                                if (*++_end != 'u')
                                    error("INVALID UNICODE SURROGATE");
                                unsigned u2 = parse4hex();  // low surrogate
                                if (u2 < 0xdc00 || u2 > 0xdfff)
                                    error("INVALID UNICODE SURROGATE");
                                u1 = (((u1 - 0xd800) << 10) | (u2 - 0xdc00)) + 0x10000;
                            }
                            str += encodeUTF8(u1);
                        }
                            break;
                        default:
                            error("INVALID STRING ESCAPE");
                    }
                    break;
            }
        }
    }

    void mjson::Parser::error(const std::string &msg) const {
        throw JsonException(msg + ": " + _begin);
    }

    mjson::Json mjson::Parser::parseValue() {
        switch (*_end) {
            case 'n':
                return parseLiteral("null");
            case 't':
                return parseLiteral("true");
            case 'f':
                return parseLiteral("false");
            case '\"':
                return parseString();
            case '[':
                return parseArray();
            case '{':
                return parseObject();
            case '\0':
                error("EXPECT VALUE");
            default:
                return parseNumber();
        }
    }

    mjson::Json mjson::Parser::parseLiteral(const std::string &literal) {
        if (strncmp(_end, literal.c_str(), literal.size()))
            error("INVALID VALUE");
        _end += literal.size();
        _begin = _end;
        switch (literal[0]) {
            case 't':
                return Json(true);
            case 'f':
                return Json(false);
            default:
                return Json(nullptr);
        }
    }

    mjson::Json mjson::Parser::parseNumber() {
        if (*_end == '-') ++_end;
        if (*_end == '0')
            ++_end;
        else {
            if (!is1to9(*_end))
                error("INVALID VALUE");
            while (is0to9(*++_end));   // pass all number character
        }
        if (*_end == '.') {
            if (!is0to9(*++_end))  // there must be a number character after '.'
                error("INVALID VALUE");
            while (is0to9(*++_end));
        }
        if (toupper(*_end) == 'E') {
            ++_end;
            if (*_end == '-' || *_end == '+')
                ++_end;
            if (!is0to9(*_end))
                error("INVALID VALUE");
            while (is0to9(*++_end));
        }
        //When we make sure that the current text is a number,
        //call the library function strtod
        double val = strtod(_begin, nullptr);
        if (fabs(val) == HUGE_VAL)
            error("NUMBER TOO BIG");
        _begin = _end;
        return Json(val);
    }

    Json Parser::parseString() {
        return Json(parseRawString());
    }


    Json Parser::parseObject() {
        Json::_object obj;
        ++_end;
        parseWhitespace();
        if (*_end == '}') {
            _begin = ++_end;
            return Json(obj);
        }
        while (1) {
            parseWhitespace();
            if (*_end != '"')
                error("MISS KEY");
            std::string key = parseRawString();
            parseWhitespace();
            if (*_end++ != ':')
                error("MISS COLON");
            parseWhitespace();
            Json val = parseValue();
            obj.insert({key, val});
            parseWhitespace();
            if (*_end == ',')
                ++_end;
            else if (*_end == '}') {
                _begin = ++_end;
                return Json(obj);
            } else
                error("MISS COMMA OR CURLY BRACKET");
        }
    }

    Json Parser::parseArray() {
        Json::_array arr;
        ++_end;  // skip '['
        parseWhitespace();
        if (*_end == ']') {
            _begin = ++_end;
            return Json(arr);
        }
        while (1) {
            parseWhitespace();
            arr.push_back(parseValue());// recursive
            parseWhitespace();
            if (*_end == ',')
                ++_end;
            else if (*_end == ']') {
                _begin = ++_end;
                return Json(arr);
            } else
                error("MISS COMMA OR SQUARE BRACKET");
        }
    }

    mjson::Json mjson::Parser::parse() {
        // JSON-text = ws value ws
        // ws = *(%x20 / %x09 / %x0A / %x0D)
        parseWhitespace();
        Json json = parseValue();
        parseWhitespace();
        if (*_end)
            // some character still exists after the end whitespace
            error("ROOT NOT SINGULAR");
        return json;
    }
}