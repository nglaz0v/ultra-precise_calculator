#ifndef VALUE_H
#define VALUE_H

#include <string>

class Value final {
private:
    char *var;

public:
    static unsigned int exactness;

    Value();
    explicit Value(const char *str);
    ~Value();

    Value(const Value &);
    Value &operator=(const Value &);
    Value(Value &&) noexcept = default;
    Value &operator=(Value &&);

    friend Value operator+(Value d1, Value d2);
    friend Value operator-(Value d1, Value d2);
    friend Value operator*(Value d1, Value d2);
    friend Value operator/(Value d1, Value d2);
    friend Value operator^(Value d1, Value d2);
    friend Value operator-(Value d);

    Value factorial();
    Value sin();
    Value cos();
    Value tan();
    Value sqrt();

    operator const char *() const { return var; }

    explicit Value(const std::string &str)
        : Value(str.c_str())
    {
    }
};

#endif // VALUE_H
