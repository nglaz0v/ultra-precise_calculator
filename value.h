#ifndef VALUE_H
#define VALUE_H

class Value final {
private:
    char *var;

public:
    static unsigned int exactness;

    explicit Value();
    explicit Value(const char *str);
    ~Value();

    Value(const Value &V);
    Value &operator=(const Value &V);
    Value(Value &&) noexcept = default;
    Value &operator=(Value &&) noexcept = default;

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
};

#endif // VALUE_H
