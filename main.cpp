#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <experimental/propagate_const>

using namespace std;

class cla {
public:
    cla(int m) : a(m), p(nullptr) { cout << "default cstr" << endl; }

    cla(const cla &c) {
        cout << "copy cstr" << endl;
        a = c.a;
        p = c.p;
    }

    cla(cla &&c) {
        cout << "move cstr" << endl;
        a = c.a;
        p = c.p;
    }

    cla &operator=(const cla &c) {
        cout << "copy operator" << endl;
        if (this != &c) {
            a = c.a;
            p = c.p;
        }
        return *this;
    }

    cla &operator=(const cla &&c) {
        cout << "move operator" << endl;
        if (this != &c) {
            a = c.a;
            p = c.p;
        }
        return *this;
    }

public:
    int a;
    int *p;

};

cla getcls(int i) {
    cla c(i);
    return c;
}

#include <iostream>
#include <utility>
#include <vector>
#include <iterator>

class stream {
public:

    using flags_type = int;

public:

    flags_type flags() const { return flags_; }

    /// 以 newf 替换 flags_ 并返回旧值。
    flags_type flags(flags_type newf) { return std::exchange(flags_, newf); }

private:

    flags_type flags_ = 0;
};

void f() { std::cout << "f()"; }

struct S {
    // 三位的无符号位域，
    // 允许值为 0...7
    unsigned int b : 3;
};

// widget.h（接口）
class widget {
    // 公开成员
private:
    struct impl;  // 实现类的前置声明
    // 一种实现示例：见下文中的其他设计选项和权衡
    experimental::propagate_const<unique_ptr<impl>> pImpl;               // 指向前置声明的实现类  // 唯一所有权的不透明指针
};

// widget.cpp（实现）
struct widget::impl {
    // 实现细节
};

int main() {
    S s = {6};
    ++s.b; // 在位域中存储值 7
    std::cout << s.b << '\n';
    ++s.b; // 值 8 不适合此位域
    ++s.b; // 值 9 不适合此位域

    std::cout << s.b << '\n'; // 正式来说是由实现定义的行为，通常为 0
}