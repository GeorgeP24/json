//
// Created by 郭毅 on 2019/12/4.
//

#ifndef MYJSON_JSONEXCEPTION_H
#define MYJSON_JSONEXCEPTION_H

#include <stdexcept>
using std::runtime_error;
namespace mjson{
    class JsonException : public runtime_error{
    public:
        explicit JsonException(const string &err_msg):runtime_error(err_msg){}

    public:
        const char* what() const noexcept override {return runtime_error::what();}
    };

}
#endif //MYJSON_JSONEXCEPTION_H
