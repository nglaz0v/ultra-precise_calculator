#include "value.h"

#include <cstring>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

//! string inversion
void reverse(char *&str);
//! alignment of numbers by adding zeros
void leading_zeros(char *&d1, char *&d2);
//! the absolute value of a number
void abs(char *&d);
//! d1 < d2
bool strlt(const char *d1, const char *d2);
//! d1 > d2
bool strgt(const char *d1, const char *d2);
//! truncate zeros before decimal point
void cut_zeros(char *&d);
//! adding zero for numbers starting with a dot '.'
void first_zero(char *&d);
//! remove '.' at the end of the line
void del_point(char *&d);
//! precision calculation
char *epsilon(unsigned int exact);

unsigned int Value::exactness = 5;

Value::Value()
    : var(nullptr)
{
}

Value::Value(const char *str)
{
    if (str != nullptr) {
        var = new char[strlen(str) + 1];
        strcpy(var, str);
    } else {
        var = nullptr;
    }
}

Value::Value(const Value &V)
{
    if (V.var != nullptr) {
        var = new char[strlen(V.var) + 1];
        strcpy(var, V.var);
    } else {
        var = nullptr;
    }
}

Value::Value(Value &&V) noexcept
{
    var = V.var;
    V.var = nullptr;
}

Value::~Value()
{
    if (var != nullptr) {
        delete[] var;
        var = nullptr;
    }
}

Value &Value::operator=(const Value &V)
{
    if (this == &V) return *this;
    delete[] var;
    if (V.var != nullptr) {
        var = new char[strlen(V.var) + 1];
        strcpy(var, V.var);
    } else {
        var = nullptr;
    }
    return *this;
}

Value &Value::operator=(Value &&V) noexcept
{
    if (this == &V) return *this;
    delete[] var;
    var = V.var;
    V.var = nullptr;
    return *this;
}

Value operator+(Value d1, Value d2)
{
    Value result;
    bool minus = false; // result is negative

    char *d_1 = strchr(d1.var, '-');
    char *d_2 = strchr(d2.var, '-');
    if ((d_1 != nullptr) && (d_2 != nullptr)) {
        abs(d1.var);
        abs(d2.var);
        minus = true;
    } else {
        if (d_1 != nullptr) {
            abs(d1.var);
            result = d2 - d1;
            return result;
        } else {
            if (d_2 != nullptr) {
                abs(d2.var);
                result = d1 - d2;
                return result;
            }
        }
    }

    leading_zeros(d1.var, d2.var);

    // cout << "d1=" << d1.var << endl;
    // cout << "d2=" << d2.var << endl;

    first_zero(d1.var);
    first_zero(d2.var);

    reverse(d1.var);
    reverse(d2.var);

    // cout << "d1=" << d1.var << endl;
    // cout << "d2=" << d2.var << endl;

    const unsigned int d1_len = strlen(d1.var);
    const unsigned int d2_len = strlen(d2.var);

    result.var = new char[d1_len + 1];
    result.var[d1_len] = '\0';
    unsigned int i = 0;
    unsigned int j = 0;
    bool overflow = false;
    for (i = 0; i < d2_len; i++) {
        // cout << "i=" << i << endl;

        const auto i1 = d1.var[i] - 0x30;
        // cout << "i1=" << i1 << endl;

        const auto i2 = d2.var[i] - 0x30;
        // cout << "i2=" << i2 << endl;

        if (d1.var[i] == '.') {
            result.var[j++] = '.';
            continue;
        }

        auto ii = i1 + i2;
        if (overflow) ii++;
        if (ii < 10) {
            result.var[j++] = ii + 0x30;
            overflow = false;
        } else {
            result.var[j++] = ii - 10 + 0x30;
            overflow = true;
        }
    }

    if (overflow) {
        // cout << "overflow" << endl;
        char *d = new char[d1_len + 1];
        strcpy(d, result.var);
        // cout << "d=" << d << endl;
        delete[] result.var;
        result.var = new char[d1_len + 1 + 1];
        strcpy(result.var, d);
        strcat(result.var, "1");
        delete[] d;
    }

    reverse(result.var);

    del_point(result.var);

    first_zero(result.var); // number starts with '.'

    if (minus) result = -result;

    // cout << "result '+' = " << result.var << endl;
    return result;
}

Value operator-(Value d1, Value d2)
{
    Value result;
    bool minus = false; // result is negative

    char *d_1 = strchr(d1.var, '-');
    char *d_2 = strchr(d2.var, '-');
    if ((d_1 != nullptr) && (d_2 != nullptr)) {
        abs(d1.var);
        abs(d2.var);
        minus = true;
    } else {
        if (d_1 != nullptr) {
            abs(d1.var);
            result = d1 + d2;
            result = -result;
            return result;
        } else {
            if (d_2 != nullptr) {
                abs(d2.var);
                result = d1 + d2;
                return result;
            }
        }
    }

    leading_zeros(d1.var, d2.var);

    // cout << "d1=" << d1.var << endl;
    // cout << "d2=" << d2.var << endl;

    first_zero(d1.var);
    first_zero(d2.var);

    bool negative = false; // result is negative
    if (strcmp(d1.var, d2.var) == -1) {
        char *d = new char[strlen(d2.var) + 1];
        strcpy(d, d2.var);
        delete[] d2.var;
        d2.var = new char[strlen(d1.var) + 1];
        strcpy(d2.var, d1.var);
        delete[] d1.var;
        d1.var = new char[strlen(d) + 1];
        strcpy(d1.var, d);
        delete[] d;
        negative = true;
    }

    reverse(d1.var);
    reverse(d2.var);

    // cout << "d1=" << d1.var << endl;
    // cout << "d2=" << d2.var << endl;

    const unsigned int d1_len = strlen(d1.var);
    const unsigned int d2_len = strlen(d2.var);

    result.var = new char[d1_len + 1];
    result.var[d1_len] = '\0';
    unsigned int i = 0;
    unsigned int j = 0;
    bool overflow = false;
    for (i = 0; i < d2_len; i++) {
        // cout << "i=" << i << endl;

        const auto i1 = d1.var[i] - 0x30;
        // cout << "i1=" << i1 << endl;

        const auto i2 = d2.var[i] - 0x30;
        // cout << "i2=" << i2 << endl;

        if (d1.var[i] == '.') {
            result.var[j++] = '.';
            continue;
        }

        auto ii = i1 - i2;
        if (overflow) ii--;
        if (ii >= 0) {
            result.var[j++] = ii + 0x30;
            overflow = false;
        } else {
            result.var[j++] = ii + 10 + 0x30;
            overflow = true;
        }
    }

    if ((result.var[d1_len - 1] == '0') && (d1_len > 1)) // strip extra zeros before '.'
    {
        reverse(result.var);
        cut_zeros(result.var);
        reverse(result.var);
    }

    if (negative) {
        // cout << "negative" << endl;
        char *d = new char[strlen(result.var) + 1];
        strcpy(d, result.var);
        // cout << "d=" << d << endl;
        delete[] result.var;
        result.var = new char[strlen(d) + 1 + 1];
        strcpy(result.var, d);
        strcat(result.var, "-");
        delete[] d;
    }

    reverse(result.var);

    del_point(result.var);

    first_zero(result.var); // number starts with '.'

    if (minus) result = -result;

    // cout << "result '-' = " << result.var << endl;
    return result;
}

Value operator*(Value d1, Value d2)
{
    char a1[11][11] = {
        // 0    1    2    3    4    5    6    7    8    9
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'}, // 0
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'}, // 1
        {'0', '0', '0', '0', '0', '1', '1', '1', '1', '1'}, // 2
        {'0', '0', '0', '0', '1', '1', '1', '2', '2', '2'}, // 3
        {'0', '0', '0', '1', '1', '2', '2', '2', '3', '3'}, // 4
        {'0', '0', '1', '1', '2', '2', '3', '3', '4', '4'}, // 5
        {'0', '0', '1', '1', '2', '3', '3', '4', '4', '5'}, // 6
        {'0', '0', '1', '2', '2', '3', '4', '4', '5', '6'}, // 7
        {'0', '0', '1', '2', '3', '4', '4', '5', '6', '7'}, // 8
        {'0', '0', '1', '2', '3', '4', '5', '6', '7', '8'}  // 9
    };
    char a2[11][11] = {
        // 0    1    2    3    4    5    6    7    8    9
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'}, // 0
        {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}, // 1
        {'0', '2', '4', '6', '8', '0', '2', '4', '6', '8'}, // 2
        {'0', '3', '6', '9', '2', '5', '8', '1', '4', '7'}, // 3
        {'0', '4', '8', '2', '6', '0', '4', '8', '2', '6'}, // 4
        {'0', '5', '0', '5', '0', '5', '0', '5', '0', '5'}, // 5
        {'0', '6', '2', '8', '4', '0', '6', '2', '8', '4'}, // 6
        {'0', '7', '4', '1', '8', '5', '2', '9', '6', '3'}, // 7
        {'0', '8', '6', '4', '2', '0', '8', '6', '4', '2'}, // 8
        {'0', '9', '8', '7', '6', '5', '4', '3', '2', '1'}  // 9
    };

    Value result;

    bool minus = false;
    char *d_1 = strchr(d1.var, '-');
    char *d_2 = strchr(d2.var, '-');
    if (((d_1 != nullptr) || (d_2 != nullptr)) && !((d_1 != nullptr) && (d_2 != nullptr))) minus = true;
    abs(d1.var);
    abs(d2.var);

    // cout << "d11=" << d1.var << endl;
    // cout << "d22=" << d2.var << endl;

    first_zero(d1.var);
    first_zero(d2.var);

    unsigned int d1_len = strlen(d1.var);
    unsigned int d2_len = strlen(d2.var);

    result.var = new char[d1_len + d2_len + 1];
    strcpy(result.var, "0");
    if ((strpbrk(d1.var, "123456789") == nullptr) || (strpbrk(d2.var, "123456789") == nullptr)) // zero
        return result;

    char *n = nullptr;
    int pos1 = 0;
    n = strchr(d1.var, '.');
    if (n != nullptr) // determine the number of characters after '.'
    {
        pos1 = n - d1.var;
        pos1 = d1_len - pos1 - 1;
        if (d1.var[0] == '.') pos1 = d1_len - 1;
        // cout << "pos1=" << pos1 << endl;

        char *d = new char[d1_len + 1];
        strcpy(d, d1.var);
        // cout << "d=" << d << endl;
        delete[] d1.var;

        d1.var = new char[d1_len];
        unsigned int i = 0;
        unsigned int j = 0;
        while (i < d1_len) {
            if (d[i] != '.')
                d1.var[j++] = d[i++];
            else
                i++;
        }
        // cout << strlen(d) << endl;
        d1.var[d1_len - 1] = '\0';
        delete[] d;
        d1_len--;
    }
    // cout << d1.var << endl;

    if (d1.var[0] == '0') { // remove extra zeros
        char *d = new char[d1_len + 1];
        strcpy(d, d1.var);
        delete[] d1.var;
        d1.var = new char[d1_len];
        unsigned int i = 0;
        unsigned int j = 0;
        while (d[i] == '0') i++;
        while (i < d1_len) { d1.var[j++] = d[i++]; }
        d1.var[j] = '\0';
        delete[] d;
        d1_len = j;
    }

    int pos2 = 0;
    n = strchr(d2.var, '.');
    if (n != nullptr) { // determine the number of characters after '.'
        pos2 = n - d2.var;
        pos2 = d2_len - pos2 - 1;
        if (d2.var[0] == '.') pos2 = d2_len - 1;
        // cout << "pos2=" << pos2 << endl;

        char *d = new char[d2_len + 1];
        strcpy(d, d2.var);
        // cout << "d=" << d << endl;
        delete[] d2.var;

        d2.var = new char[d2_len];
        unsigned int i = 0;
        unsigned int j = 0;
        while (i < d2_len) {
            if (d[i] != '.')
                d2.var[j++] = d[i++];
            else
                i++;
        }
        // cout << strlen(d) << endl;
        d2.var[d2_len - 1] = '\0';
        delete[] d;
        d2_len--;
    }
    // cout << d2.var << endl;

    if (d2.var[0] == '0') { // remove extra zeros
        char *d = new char[d2_len + 1];
        strcpy(d, d2.var);
        delete[] d2.var;
        d2.var = new char[d2_len];
        unsigned int i = 0;
        unsigned int j = 0;
        while (d[i] == '0') i++;
        while (i < d2_len) { d2.var[j++] = d[i++]; }
        d2.var[j] = '\0';
        delete[] d;
        d2_len = j;
    }

    if ((d2_len > d1_len) || ((d2_len == d1_len) && (strcmp(d2.var, d1.var) == 1))) { // d2 > d1
        char *d = new char[d1_len + 1];
        strcpy(d, d1.var);
        delete[] d1.var;
        d1.var = new char[d2_len + 1];
        strcpy(d1.var, d2.var);
        delete[] d2.var;
        d2.var = new char[d1_len + 1];
        strcpy(d2.var, d);
        delete[] d;
    }
    d1_len = strlen(d1.var);
    d2_len = strlen(d2.var);
    // cout << "d11=" << d1.var << endl;
    // cout << "d22=" << d2.var << endl;

    Value prom;
    Value j1;
    Value j2;
    j1.var = new char[3];
    j2.var = new char[3];
    prom.var = new char[d1_len + d2_len + 1];
    strcpy(prom.var, "");
    char *zer = new char[d2_len + 1]; // to add zeros
    strcpy(zer, "");
    reverse(d1.var);
    reverse(d2.var);

    for (unsigned int j = 0; j < d2_len; j++) {
        strcpy(prom.var, "");
        strcpy(j1.var, "0");
        strcpy(j2.var, "0");

        for (unsigned int i = 0; i < d1_len; i++) {
            auto i1 = d1.var[i] - 0x30;
            // cout << "i1=" << i1 << endl;

            auto i2 = d2.var[j] - 0x30;
            // cout << "i2=" << i2 << endl;

            /*ii=i1*i2;
            i1=ii/10;
            i2=ii%10;*/

            // j2.var[0]=i2+0x30;
            j2.var[0] = a2[i1][i2];
            j2.var[1] = '\0';

            // j2=j2+j1;
            j2.var[0] = j2.var[0] + j1.var[0] - 0x30;
            if (j2.var[0] > '9') {
                j2.var[1] = j2.var[0] - 10;
                j2.var[0] = '1';
                j2.var[2] = '\0';
            }

            // j1.var[0]=i1+0x30;
            j1.var[0] = a1[i1][i2];
            j1.var[1] = '\0';

            i1 = j1.var[0] - 0x30;
            i2 = j2.var[0] - 0x30;

            if (strlen(j2.var) == 2) { // result is more than 10
                j1.var[0] = j1.var[0] + j2.var[0] - 0x30;
                j2.var[0] = j2.var[1];
                j2.var[1] = '\0';
            }
            strcat(prom.var, j2.var);
        }
        strcat(prom.var, j1.var);
        reverse(prom.var);
        strcat(prom.var, zer);
        strcat(zer, "0");

        result = result + prom;
    }
    delete[] zer;

    /*	while (strcmp(counter.var,d2.var)!=0) // multiplication
        {
            //cout << "strcmp=" << strcmp(counter.var,d2.var) << endl;
            result=result+d1;
            counter=counter+one;
            //cout << "result=" << result.var << " counter=" << counter.var << " d2=" << d2.var << endl;
        }
    */
    unsigned int pos = 0;
    pos = pos1 + pos2;
    // cout << "pos=" << pos << endl;

    if (pos < strlen(result.var)) { // define position '.' as a result
        char *d = new char[strlen(result.var) + 1];
        strcpy(d, result.var);
        delete[] result.var;
        result.var = new char[strlen(d) + 1 + 1];
        unsigned int i = 0;
        unsigned int j = 0;
        while (i < strlen(d)) {
            if (j == strlen(d) - pos)
                result.var[j++] = '.';
            else
                result.var[j++] = d[i++];
        }
        result.var[j] = '\0';
        delete[] d;
    } else {
        char *d = new char[pos + 2 + 1];
        unsigned int i = 0;
        for (i = 0; i < pos + 2; i++) d[i] = '0';
        d[pos + 2] = '\0';
        // cout << "d=" << d << endl;
        reverse(d);
        reverse(result.var);
        i = 0;
        unsigned int j = 0;
        while (i < strlen(result.var)) { d[j++] = result.var[i++]; }
        while (j < pos) { d[j++] = '0'; }
        d[j++] = '.';
        d[j++] = '0';
        d[j] = '\0';
        reverse(d);
        // cout << "d=" << d << endl;
        delete[] result.var;
        result.var = new char[strlen(d) + 1];
        strcpy(result.var, d);
        delete[] d;
    }

    cut_zeros(result.var);

    if (minus) result = -result;

    // cout << "result '*' = " << result.var << endl;
    return result;
}

Value operator/(Value d1, Value d2)
{
    Value result;

    bool minus = false;
    char *d_1 = strchr(d1.var, '-');
    char *d_2 = strchr(d2.var, '-');
    if (((d_1 != nullptr) || (d_2 != nullptr)) && !((d_1 != nullptr) && (d_2 != nullptr))) minus = true;
    abs(d1.var);
    abs(d2.var);

    unsigned int precision = Value::exactness;
    // cout << "d11=" << d1.var << endl;
    // cout << "d22=" << d2.var << endl;

    first_zero(d1.var);
    first_zero(d2.var);

    result.var = new char[2];
    strcpy(result.var, "");
    if (strpbrk(d1.var, "123456789") == nullptr) return Value("0");
    if (strpbrk(d2.var, "123456789") == nullptr) {
        cerr << "\a\nDivision by zero" << endl;
        exit(1);
    }

    auto d1_len = strlen(d1.var);
    auto d2_len = strlen(d2.var);
    // auto dd1 = d1_len;
    // auto dd2 = d2_len;
    // auto ddd1 = d1_len;
    // auto ddd2 = d2_len;
    int ppp;
    // ddd1=strchr(d1.var,'.')-d1.var;
    // ddd2=strchr(d2.var,'.')-d2.var;

    char *n = nullptr;
    unsigned int pos1 = 0;
    n = strchr(d1.var, '.');
    if (n != nullptr) {
        pos1 = n - d1.var;
        // ddd1 = pos1;
        pos1 = d1_len - pos1 - 1;
        if (d1.var[0] == '.') pos1 = d1_len - 1;
        // cout << "pos1=" << pos1 << endl;

        char *d = new char[d1_len + 1];
        strcpy(d, d1.var);
        // cout << "d=" << d << endl;
        delete[] d1.var;

        d1.var = new char[d1_len];
        unsigned int i = 0;
        unsigned int j = 0;
        while (i < d1_len) {
            if (d[i] != '.')
                d1.var[j++] = d[i++];
            else
                i++;
        }
        // cout << strlen(d) << endl;
        d1.var[d1_len - 1] = '\0';
        delete[] d;
        d1_len--;
    }
    // cout << d1.var << endl;

    unsigned int pos2 = 0;
    n = strchr(d2.var, '.');
    if (n != nullptr) {
        pos2 = n - d2.var;
        // ddd2 = pos2;
        pos2 = d2_len - pos2 - 1;
        if (d2.var[0] == '.') pos2 = d2_len - 1;
        // cout << "pos2=" << pos2 << endl;

        char *d = new char[d2_len + 1];
        strcpy(d, d2.var);
        // cout << "d=" << d << endl;
        delete[] d2.var;

        d2.var = new char[d2_len];
        unsigned int i = 0;
        unsigned int j = 0;
        while (i < d2_len) {
            if (d[i] != '.')
                d2.var[j++] = d[i++];
            else
                i++;
        }
        // cout << strlen(d) << endl;
        d2.var[d2_len - 1] = '\0';
        delete[] d;
        d2_len--;
    }
    // cout << d2.var << endl;

    cut_zeros(d2.var);

    if (Value::exactness == 1) precision++;
    if (Value::exactness == 0) precision += 2;

    int h = precision + pos2;
    char *dd = new char[d1_len + 1];
    strcpy(dd, d1.var);
    delete[] d1.var;
    d1.var = new char[d1_len + precision + pos2 + 1];
    strcpy(d1.var, dd);
    delete[] dd;

    int q = pos1;
    while (q < h) {
        strcat(d1.var, "0");
        q++;
    }
    ppp = q;

    Value one("1");
    Value counter;
    counter.var = new char[2];
    strcpy(counter.var, "0");

    Value ost;
    ost.var = new char[2];
    strcpy(ost.var, "");

    Value rez;

    d2_len = strlen(d2.var);
    unsigned int ii = 0;
    unsigned int j = d2_len;
    unsigned int i1 = 0;
    bool first = true;

    cut_zeros(d1.var);
    d1_len = strlen(d1.var);

    if (d1_len <= d2_len) {
        strcpy(result.var, "0");
        return result;
    }

    /*if (ddd2+d2_len+1<dd2)*/
    // ppp=dd2-ddd2-d2_len-1+ddd1;
    /*else
        ppp=1;*/
    // ppp=0;
    ppp = ppp - pos2;

    while (i1 < d1_len - d2_len + 1) {
        int i = ii;
        char *d;
        /*if (i1<ddd1)
            ppp++;*/

        if ((d1.var[i] == '0') && (strcmp(ost.var, "0") == 0)) {
            d = new char[strlen(result.var) + 1];
            strcpy(d, result.var);
            delete[] result.var;
            result.var = new char[strlen(d) + 1 + 1];
            strcpy(result.var, d);
            strcat(result.var, "0");
            strcpy(ost.var, "");
            delete[] d;
            ii++;
            i1++;
            continue;
        }
        if (strcmp(ost.var, "0") == 0) {
            d = new char[strlen(result.var) + 1];
            strcpy(d, result.var);
            delete[] result.var;
            result.var = new char[strlen(d) + 1 + 1];
            strcpy(result.var, d);
            ost.var[0] = d1.var[i];
            ost.var[1] = '\0';
            delete[] d;
            if (!strlt(ost.var, d2.var)) {
                strcpy(counter.var, "0");
                while (!(strlt(ost.var, d2.var))) {
                    ost = ost - d2;
                    // cout << ost.var << endl;
                    counter = counter + one;
                }
                d = new char[strlen(result.var) + 1];
                strcpy(d, result.var);
                delete[] result.var;
                result.var = new char[strlen(d) + strlen(counter.var) + 1];
                strcpy(result.var, d);
                strcat(result.var, counter.var);
                delete[] d;
            } else {
                d = new char[strlen(result.var) + 1];
                strcpy(d, result.var);
                delete[] result.var;
                result.var = new char[strlen(d) + 1 + 1];
                strcpy(result.var, d);
                strcat(result.var, "0");
                ost.var[0] = d1.var[i];
                ost.var[1] = '\0';
                delete[] d;
            }
            ii++;
            i1++;
            continue;
        }
        rez.var = new char[strlen(d2.var) + 1];
        strncpy(rez.var, d1.var + ii, j);
        rez.var[j] = '\0';
        // cout << rez.var << endl;

        d = new char[strlen(ost.var) + 1];
        strcpy(d, ost.var);
        delete[] ost.var;
        ost.var = new char[strlen(d) + strlen(rez.var) + 1];
        strcpy(ost.var, d);
        strcat(ost.var, rez.var);
        delete[] d;
        strcpy(counter.var, "0");
        // cout << strlt(ost.var,d2.var) << endl;

        while (!(strlt(ost.var, d2.var))) {
            ost = ost - d2;
            // cout << ost.var << endl;
            counter = counter + one;
        }
        d = new char[strlen(result.var) + 1];
        strcpy(d, result.var);
        delete[] result.var;
        result.var = new char[strlen(d) + strlen(counter.var) + 1];
        strcpy(result.var, d);

        strcat(result.var, counter.var);
        delete[] d;
        // cout << rez.var << endl;
        if (first)
            ii = ii + strlen(d2.var);
        else
            ii++;
        first = false;
        j = 1;
        i1++;
    }

    reverse(result.var);
    while ((result.var[strlen(result.var) - 1] == '0') && (strlen(result.var) > 1)) {
        // cout << "overflow" << endl;
        char *d = new char[strlen(result.var) + 1];
        strcpy(d, result.var);
        // cout << "d=" << d << endl;
        delete[] result.var;
        result.var = new char[strlen(d)];
        for (unsigned int i = 0; i < strlen(d) - 1; i++) {
            result.var[i] = d[i];
            // cout << result[i] << endl;
        }
        // cout << strlen(d) << endl;
        result.var[strlen(d) - 1] = '\0';
        delete[] d;
    }
    reverse(result.var);

    unsigned int pos;
    /*if (abs(pos1-pos2)<precision)
        pos=precision;
    else*/
    // pos=abs(pos1-pos2);
    // pos=strlen(d1.var)-strlen(d2.var);
    // pos=abs(dd1-dd2);
    // pos=strlen(result.var)-ppp;
    pos = ppp;
    // cout << pos1 << endl;
    // cout << pos2 << endl;
    if (pos < strlen(result.var)) {
        char *d = new char[strlen(result.var) + 1];
        strcpy(d, result.var);
        delete[] result.var;
        result.var = new char[strlen(d) + 1 + 1];
        unsigned int iI = 0;
        unsigned int jJ = 0;
        while (iI < strlen(d)) {
            if (jJ == strlen(d) - pos)
                result.var[jJ++] = '.';
            else
                result.var[jJ++] = d[iI++];
        }
        result.var[jJ] = '\0';
        delete[] d;
    } else {
        char *d = new char[strlen(result.var) + pos + 2 + 1];
        for (unsigned int ij = 0; ij < pos + strlen(result.var); ij++) d[ij] = '0';
        d[pos + strlen(result.var)] = '\0';
        // cout << "d=" << d << endl;
        reverse(d);
        reverse(result.var);
        unsigned int iI = 0;
        unsigned int jJ = 0;
        while (iI < strlen(result.var)) { d[jJ++] = result.var[iI++]; }
        while (jJ < pos) { d[jJ++] = '0'; }
        d[jJ++] = '.';
        d[jJ++] = '0';
        d[jJ] = '\0';
        reverse(d);
        // cout << "d=" << d << endl;
        delete[] result.var;
        result.var = new char[strlen(d) + 1];
        strcpy(result.var, d);
        delete[] d;
    }

    Value tr;
    tr.var = new char[strlen(result.var) + 1];
    strcpy(tr.var, result.var);
    delete[] result.var;
    Value ed;
    ed.var = new char[strlen(tr.var) + 1];
    ed.var[0] = '0';
    ed.var[1] = '.';
    for (unsigned int i = 0; i < precision; i++) ed.var[i + 2] = '0';
    ed.var[precision + 1] = '1';
    ed.var[precision + 2] = '\0';

    if (Value::exactness == 1) {
        delete[] ed.var;
        ed.var = new char[4];
        strcpy(ed.var, "0.1");
    }
    if (Value::exactness == 0) {
        delete[] ed.var;
        ed.var = new char[2];
        strcpy(ed.var, "1");
    }

    if (Value::exactness != 0) {
        if (tr.var[strlen(tr.var) - 1] >= '5') tr = tr + ed;
    } else {
        if (tr.var[strlen(tr.var) - 2] >= '5') tr = tr + ed;
    }
    unsigned int l = strlen(tr.var);
    if (Value::exactness == 1) l--;
    if (Value::exactness == 0) l -= 3;
    result.var = new char[l + 1];
    unsigned int i = 0;
    for (i = 0; i < l; i++) { result.var[i] = tr.var[i]; }
    result.var[i] = '\0';

    if (minus) result = -result;

    // cout << "result '/' = " << result.var << endl;
    return result;
}

Value operator^(Value d1, Value d2)
{
    Value result;
    if (strchr(d2.var, '.') != nullptr) {
        cerr << "\a\nThe exponent must be an integer" << endl;
        exit(1);
    }
    Value counter;
    counter.var = new char[2];
    strcpy(counter.var, "0");
    Value one("1");
    result = one;
    bool rev = false;   // result is a fraction
    bool minus = false; // result is negative
    if (strchr(d2.var, '-') != nullptr) {
        abs(d2.var);
        rev = true;
    }
    if (strchr(d1.var, '-') != nullptr) {
        abs(d1.var);
        unsigned int len = strlen(d2.var) - 1;
        if ((d2.var[len] == '1') || (d2.var[len] == '3') || (d2.var[len] == '5') || (d2.var[len] == '7') ||
            (d2.var[len] == '9'))
            minus = true;
    }
    while (!(strcmp(counter.var, d2.var) == 0)) {
        result = result * d1;
        counter = counter + one;
    }
    if (rev) result = one / result;

    if (minus) result = -result;

    // cout << "result '^' = " << result.var << endl;
    return result;
}

Value operator-(Value d)
{
    unsigned int res_len = strlen(d.var);
    char *tmp = new char[res_len + 1];
    strcpy(tmp, d.var);
    delete[] d.var;
    if (strchr(tmp, '-') == nullptr) {
        d.var = new char[res_len + 1 + 1];
        d.var[0] = '-';
        for (unsigned int i = 0; i < res_len; i++) d.var[i + 1] = tmp[i];
        d.var[res_len + 1] = '\0';
        delete[] tmp;
    } else {
        d.var = new char[res_len];
        reverse(tmp);
        for (unsigned int i = 0; i < res_len - 1; i++) d.var[i] = tmp[i];
        d.var[res_len - 1] = '\0';
        reverse(d.var);
        delete[] tmp;
    }
    return d;
}

Value Value::factorial()
{
    Value result;
    if (strchr(this->var, '.') != nullptr) {
        cerr << "\a\nThe expression under the factorial must be an integer" << endl;
        exit(1);
    }
    Value index;
    Value one("1");
    result = one;
    index = one;
    while (!(strgt(index.var, this->var))) {
        result = result * index;
        /// cout << "index=" << index.var << endl;
        // cout << "result '!' = " << result.var << endl;
        index = index + one;
    }
    delete[] this->var;
    this->var = new char[strlen(result.var) + 1];
    strcpy(this->var, result.var);
    return result;
}

Value Value::sin()
{
    Value result;
    Value eps(epsilon(Value::exactness));
    Value z;
    Value y;
    y.var = new char[2];
    strcpy(y.var, "0");
    z.var = new char[strlen(this->var) + 1];
    strcpy(z.var, this->var);
    Value comp;
    comp.var = new char[2];
    strcpy(comp.var, "1");
    Value i;
    i.var = new char[2];
    strcpy(i.var, "2");
    Value zero("0");
    leading_zeros(comp.var, eps.var);
    while (strgt(comp.var, eps.var)) {
        y = y + z;
        Value del = (i + Value("1")) * i;
        Value mn = Value("-1") * (*this) * (*this);
        z = z * mn;
        z = z / del;
        // z=z*(Value("-1")*(*this)*(*this))/((i+Value("1"))*i);
        i = i + Value("2");

        bool thk = false;
        char *d = new char[strlen(y.var) + 1];
        unsigned int k = 0;
        unsigned int j = 0;
        for (j = 0; j < strlen(y.var); j++) {
            d[j] = y.var[j];
            if (d[j] == '.') thk = true;
            if (thk) k++;
            if (k == Value::exactness + 2) break;
        }
        d[j] = '\0';
        delete[] y.var;
        y.var = new char[strlen(d) + 1];
        strcpy(y.var, d);

        cout << "sin = " << y.var << endl;

        d = new char[strlen(z.var) + 1];
        strcpy(d, z.var);
        abs(d);

        if ((strgt(d, zero.var)) && (strchr(z.var, '-') == nullptr))
            comp = z;
        else
            comp = -z;
        delete[] d;
        leading_zeros(comp.var, eps.var);
    }
    result = y;
    delete[] this->var;
    this->var = new char[strlen(result.var) + 1];
    strcpy(this->var, result.var);
    return result;
}

Value Value::cos()
{
    Value result;
    Value eps(epsilon(Value::exactness));
    Value z;
    Value y;
    y.var = new char[2];
    strcpy(y.var, "1");
    z.var = new char[2];
    strcpy(z.var, "1");
    Value comp;
    comp.var = new char[2];
    strcpy(comp.var, "1");
    Value i;
    i.var = new char[2];
    strcpy(i.var, "1");
    Value zero("0");
    leading_zeros(comp.var, eps.var);
    while (strgt(comp.var, eps.var)) {
        Value del = (i + Value("1")) * i;
        Value mn = Value("-1") * (*this) * (*this);
        z = z * mn;
        z = z / del;
        // z=z*(Value("-1")*(*this)*(*this))/((i+Value("1"))*i);
        y = y + z;
        i = i + Value("2");

        bool thk = false;
        char *d = new char[strlen(y.var) + 1];
        unsigned int k = 0;
        unsigned int j = 0;
        for (j = 0; j < strlen(y.var); j++) {
            d[j] = y.var[j];
            if (d[j] == '.') thk = true;
            if (thk) k++;
            if (k == Value::exactness + 2) break;
        }
        d[j] = '\0';
        delete[] y.var;
        y.var = new char[strlen(d) + 1];
        strcpy(y.var, d);

        cout << "cos = " << y.var << endl;

        d = new char[strlen(z.var) + 1];
        strcpy(d, z.var);
        abs(d);

        if ((strgt(d, zero.var)) && (strchr(z.var, '-') == nullptr))
            comp = z;
        else
            comp = -z;
        delete[] d;
        leading_zeros(comp.var, eps.var);
    }
    result = y;
    delete[] this->var;
    this->var = new char[strlen(result.var) + 1];
    strcpy(this->var, result.var);
    return result;
}

Value Value::tan()
{
    Value s(this->var);
    Value c(this->var);
    s.sin();
    c.cos();
    *this = s / c;
    return *this;
}

Value Value::sqrt()
{
    Value result;
    if (strchr(this->var, '-') != nullptr) {
        cerr << "\a\nThe square root function parameter must be a positive number" << endl;
        exit(1);
    }
    Value eps(epsilon(Value::exactness));
    Value z;
    Value y;
    y.var = new char[2];
    strcpy(y.var, "1");
    z.var = new char[2];
    strcpy(z.var, "0");
    Value comp;
    if (strgt(z.var, y.var))
        comp = z - y;
    else
        comp = y - z;
    leading_zeros(comp.var, eps.var);
    while (strgt(comp.var, eps.var)) {
        z = y;
        Value del = (*this) / y;
        cout << "del=" << del.var << endl;
        y = y + del;
        y = y / Value("2");
        // y=(y+(*this)/y)/Value("2");
        cout << "sqrt = " << y.var << endl;
        if (strgt(z.var, y.var))
            comp = z - y;
        else
            comp = y - z;
        leading_zeros(comp.var, eps.var);
        cut_zeros(eps.var);
    }
    result = y;
    delete[] this->var;
    this->var = new char[strlen(result.var) + 1];
    strcpy(this->var, result.var);
    return result;
}

void reverse(char *&str)
{
    for (int i = 0, j = strlen(str) - 1; i < j; i++, j--) {
        char c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

void leading_zeros(char *&d1, char *&d2)
{
    unsigned int d1_len = strlen(d1);
    unsigned int d2_len = strlen(d2);
    if (strchr(d1, '.') == nullptr) { // adding '.' in d1
        char *d = new char[d1_len + 1];
        strcpy(d, d1);
        delete[] d1;
        d1 = new char[d1_len + 1 + 1];
        strcpy(d1, d);
        strcat(d1, ".");
        delete[] d;
        d1_len++;
    }
    if (strchr(d2, '.') == nullptr) { // adding '.' in d2
        char *d = new char[d2_len + 1];
        strcpy(d, d2);
        delete[] d2;
        d2 = new char[d2_len + 1 + 1];
        strcpy(d2, d);
        strcat(d2, ".");
        delete[] d;
        d2_len++;
    }

    // splitting d1 and d2 into integer and fractional parts
    char *d1bp; // integer part d1
    char *d1ap; // fractional part d1
    char *d2bp; // integer part d2
    char *d2ap; // fractional part d2

    unsigned int d1bl = strcspn(d1, ".");
    unsigned int d1al = d1_len - d1bl - 1;
    unsigned int d2bl = strcspn(d2, ".");
    unsigned int d2al = d2_len - d2bl - 1;

    // cout << d1bl << endl;
    d1bp = new char[d1bl + 1];
    strncpy(d1bp, d1, d1bl);
    d1bp[d1bl] = '\0';
    // cout << "d1bp=" << d1bp << endl;
    // cout << d1al << endl;
    d1ap = new char[d1al + 1];
    if (d1al != 0) {
        for (unsigned int i = 0; i < d1_len - d1bl - 1; i++) d1ap[i] = d1[i + d1bl + 1];
    }
    d1ap[d1al] = '\0';
    // cout << "d1ap=" << d1ap << endl;
    // cout << d2bl << endl;
    d2bp = new char[d2bl + 1];
    strncpy(d2bp, d2, d2bl);
    d2bp[d2bl] = '\0';
    // cout << "d2bp=" << d2bp << endl;
    // cout << d2al << endl;
    d2ap = new char[d2al + 1];
    if (d2al != 0) {
        for (unsigned int i = 0; i < d2_len - d2bl - 1; i++) d2ap[i] = d2[i + d2bl + 1];
    }
    d2ap[d2al] = '\0';
    // cout << "d2ap=" << d2ap << endl;
    if (d1bl > d2bl) { // adding zeros to d2bp
        reverse(d2bp);
        char *d = new char[d2bl + 1];
        strcpy(d, d2bp);
        delete[] d2bp;
        d2bp = new char[d1bl + 1];
        strcpy(d2bp, d);
        delete[] d;
        unsigned int i = 0;
        for (i = d2bl; i < d1bl; i++) d2bp[i] = '0';
        d2bp[i] = '\0';
        reverse(d2bp);
        // cout << "d1bp=" << d1bp << endl;
        // cout << "d2bp=" << d2bp << endl;
    }
    if (d2bl > d1bl) { // adding zeros to d1bp
        reverse(d1bp);
        char *d = new char[d1bl + 1];
        strcpy(d, d1bp);
        delete[] d1bp;
        d1bp = new char[d2bl + 1];
        strcpy(d1bp, d);
        delete[] d;
        unsigned int i = 0;
        for (i = d1bl; i < d2bl; i++) d1bp[i] = '0';
        d1bp[i] = '\0';
        reverse(d1bp);
        // cout << "d1bp=" << d1bp << endl;
        // cout << "d2bp=" << d2bp << endl;
    }
    if (d1al > d2al) { // adding zeros to d2ap
        char *d = new char[d2al + 1];
        strcpy(d, d2ap);
        delete[] d2ap;
        d2ap = new char[d1al + 1];
        strcpy(d2ap, d);
        delete[] d;
        unsigned int i = 0;
        for (i = d2al; i < d1al; i++) d2ap[i] = '0';
        d2ap[i] = '\0';
        // cout << "d1ap=" << d1ap << endl;
        // cout << "d2ap=" << d2ap << endl;
    }
    if (d2al > d1al) { // adding zeros to d1ap
        char *d = new char[d1al + 1];
        strcpy(d, d1ap);
        delete[] d1ap;
        d1ap = new char[d2al + 1];
        strcpy(d1ap, d);
        delete[] d;
        unsigned int i = 0;
        for (i = d1al; i < d2al; i++) d1ap[i] = '0';
        d1ap[i] = '\0';
        // cout << "d1ap=" << d1ap << endl;
        // cout << "d2ap=" << d2ap << endl;
    }
    delete[] d1;
    d1 = new char[strlen(d1bp) + strlen(d1ap) + 2];
    strcpy(d1, d1bp);
    strcat(d1, ".");
    strcat(d1, d1ap);
    delete[] d2;
    d2 = new char[strlen(d2bp) + strlen(d2ap) + 2];
    strcpy(d2, d2bp);
    strcat(d2, ".");
    strcat(d2, d2ap);

    delete[] d1bp;
    delete[] d1ap;
    delete[] d2bp;
    delete[] d2ap;

    // cout << d1 << endl;
    // cout << d2 << endl;
}

void abs(char *&d)
{
    unsigned res_len = strlen(d);
    if (strchr(d, '-') != nullptr) {
        char *tmp = new char[res_len + 1];
        strcpy(tmp, d);
        // cout << "d=" << d << endl;
        delete[] d;
        reverse(tmp);
        d = new char[res_len];
        for (unsigned int i = 0; i < res_len - 1; i++) {
            d[i] = tmp[i];
            // cout << result[i] << endl;
        }
        // cout << strlen(d) << endl;
        d[res_len - 1] = '\0';
        delete[] tmp;
        reverse(d);
        // cout << "dr=" << dr << endl;
    }
    // return d;
}

bool strlt(const char *d1, const char *d2)
{
    if (strlen(d1) < strlen(d2)) return true;
    if (strlen(d1) > strlen(d2)) return false;
    for (int i = 0; d1[i] != '\0'; i++) {
        if (d1[i] == d2[i])
            continue;
        else if (d1[i] < d2[i])
            return true;
        else
            return false;
    }
    return false;
}

bool strgt(const char *d1, const char *d2)
{
    if (strlen(d1) > strlen(d2)) return true;
    if (strlen(d1) < strlen(d2)) return false;
    for (int i = 0; d1[i] != '\0'; i++) {
        if (d1[i] == d2[i])
            continue;
        else if (d1[i] > d2[i])
            return true;
        else
            return false;
    }
    return false;
}

void cut_zeros(char *&d)
{
    unsigned int res_len = strlen(d);
    char *tmp = new char[res_len + 1];
    unsigned int i = 0;
    unsigned int j = 0;
    bool f = true;
    while (i < res_len) {
        if ((d[i] == '0') && f && (d[i + 1] != '.')) {
            i++;
        } else {
            tmp[j++] = d[i++];
            f = false;
        }
    }
    tmp[j] = '\0';
    delete[] d;
    d = new char[strlen(tmp) + 1];
    strcpy(d, tmp);
    delete[] tmp;
    if (strpbrk(d, "123456789") == nullptr) {
        delete[] d;
        d = new char[2];
        d[0] = '0';
        d[1] = '\0';
    }
}

void first_zero(char *&d)
{
    unsigned int res_len = strlen(d);
    if (d[0] == '.') {
        char *tmp = new char[res_len + 1];
        strcpy(tmp, d);
        delete[] d;
        d = new char[res_len + 1 + 1];
        reverse(tmp);
        strcpy(d, tmp);
        d[res_len] = '0';
        d[res_len + 1] = '\0';
        reverse(d);
        delete[] tmp;
    }
}

void del_point(char *&d)
{
    unsigned int res_len = strlen(d);
    if (d[res_len - 1] == '.') { // number ends with '.'
        char *tmp = new char[res_len];
        strncpy(tmp, d, res_len - 1);
        tmp[res_len - 1] = '\0';
        delete[] d;
        d = new char[res_len];
        strcpy(d, tmp);
        delete[] tmp;
    }
}

char *epsilon(unsigned int exact)
{
    char *retn;
    retn = new char[exact + 1 + 1 + 1];
    retn[0] = '0';
    retn[1] = '.';
    unsigned int j = 0;
    for (j = 2; j < exact + 1; j++) retn[j] = '0';
    retn[j++] = '1';
    retn[j] = '\0';
    return retn;
}

/*void leading_zeros(char* &d1, char* &d2)
{
    short n;
    short max;
    (strlen(d1)>strlen(d2))? max=strlen(d1) : max=strlen(d2); // which line is longer
    n=max/8+1;
    if ((strchr(d1,'.')==NULL)) // adding '.' in d1
    {
        char* d=new char[n*8+1];
        strcpy(d,d1);
        delete [] d1;
        d1=new char[n*8+1+1];
        strcpy(d1,d);
        strcat(d1,".");
        delete [] d;
        *//*char* d=new char[strlen(d1)+1];
        strcpy(d,d1);
        delete [] d1;
        d1=new char[strlen(d)+2];
        strcpy(d1,d);
        //cout << "d1=" << d1 << endl;
        d1[strlen(d1)+1]='\0';
        d1[strlen(d1)]='.';
        delete [] d;*/
/*	}
    if ((strchr(d2,'.')==NULL)) // adding '.' in d2
    {
        char* d=new char[n*8+1];
        strcpy(d,d2);
        delete [] d2;
        d2=new char[n*8+1+1];
        strcpy(d2,d);
        strcat(d2,".");
        delete [] d;
        *//*char* d=new char[strlen(d2)+1];
        strcpy(d,d2);
        delete [] d2;
        d2=new char[strlen(d)+2];
        strcpy(d2,d);
        //cout << "d2=" << d2 << endl;
        d2[strlen(d2)+1]='\0';
        d2[strlen(d2)]='.';
        delete [] d;*/
//	}

// cout << "--d1=" << d1 << endl;
// cout << "--d2=" << d2 << endl;
/*	if ((strchr(d1,'.')!=NULL) && (strchr(d2,'.')!=NULL)) // splitting d1 and d2 into integer and fractional parts
    {
        char* d1bp; // integer part d1
        char* d1ap; // fractional part d1
        char* d2bp; // integer part d2
        char* d2ap; // fractional part d2
        if (strchr(d1,'.')!=NULL)
        {
            short d1bl=strcspn(d1,".");
            //cout << d1bl << endl;
            d1bp=new char[n*8+1];
            strncpy(d1bp,d1,d1bl);
            d1bp[d1bl]='\0';
            //cout << "d1bp=" << d1bp << endl;

            short d1al=strlen(d1)-d1bl-1;
            //cout << d1al << endl;
            d1ap=new char[n*8+1];
            for (short i=0; i<strlen(d1); i++)
                d1ap[i]=d1[i+d1bl+1];
            d1ap[d1al]='\0';
            //cout << "d1ap=" << d1ap << endl;

            *//*char* d=new char[strlen(d1)+1];
            strcpy(d,d1);
            delete [] d1;
            d1=new char[strlen(d)+2];
            strcpy(d1,d);
            cout << "d1=" << d1 << endl;
            d1[strlen(d1)+1]='\0';
            d1[strlen(d1)]='.';
            delete [] d;*/
/*		}
        if (strchr(d2,'.')!=NULL)
        {
            short d2bl=strcspn(d2,".");
            //cout << d2bl << endl;
            d2bp=new char[n*8+1];
            strncpy(d2bp,d2,d2bl);
            d2bp[d2bl]='\0';
            //cout << "d2bp=" << d2bp << endl;

            short d2al=strlen(d2)-d2bl-1;
            //cout << d2al << endl;
            d2ap=new char[n*8+1];
            for (short i=0; i<strlen(d2); i++)
                d2ap[i]=d2[i+d2bl+1];
            d2ap[d2al]='\0';
            //cout << "d2ap=" << d2ap << endl;

            *//*char* d=new char[strlen(d1)+1];
            strcpy(d,d1);
            delete [] d1;
            d1=new char[strlen(d)+2];
            strcpy(d1,d);
            cout << "d1=" << d1 << endl;
            d1[strlen(d1)+1]='\0';
            d1[strlen(d1)]='.';
            delete [] d;*/
/*		}
        if (strlen(d1bp)>strlen(d2bp)) // adding zeros to d2bp
        {
            reverse(d2bp);
            char* d=new char[n*8+1];
            strcpy(d,d2bp);
            delete [] d2bp;
            d2bp=new char[n*8+1];
            strcpy(d2bp,d);
            delete [] d;
            for (short i=strlen(d2bp); i<strlen(d1bp); i++)
                d2bp[i]='0';
            d2bp[i]='\0';
            reverse(d2bp);
            //cout << "d1bp=" << d1bp << endl;
            //cout << "d2bp=" << d2bp << endl;
        }
        if (strlen(d2bp)>strlen(d1bp)) // adding zeros to d1bp
        {
            reverse(d1bp);
            char* d=new char[n*8+1];
            strcpy(d,d1bp);
            delete [] d1bp;
            d1bp=new char[n*8+1];
            strcpy(d1bp,d);
            delete [] d;
            for (short i=strlen(d1bp); i<strlen(d2bp); i++)
                d1bp[i]='0';
            d1bp[i]='\0';
            reverse(d1bp);
            //cout << "d1bp=" << d1bp << endl;
            //cout << "d2bp=" << d2bp << endl;
        }
        if (strlen(d1ap)>strlen(d2ap)) // adding zeros to d2ap
        {
            char* d=new char[n*8+1];
            strcpy(d,d2ap);
            delete [] d2ap;
            d2ap=new char[n*8+1];
            strcpy(d2ap,d);
            delete [] d;
            for (short i=strlen(d2ap); i<strlen(d1ap); i++)
                d2ap[i]='0';
            d2ap[i]='\0';
            //cout << "d1ap=" << d1ap << endl;
            //cout << "d2ap=" << d2ap << endl;
        }
        if (strlen(d2ap)>strlen(d1ap)) // adding zeros to d1ap
        {
            char* d=new char[n*8+1];
            strcpy(d,d1ap);
            delete [] d1ap;
            d1ap=new char[n*8+1];
            strcpy(d1ap,d);
            delete [] d;
            for (short i=strlen(d1ap); i<strlen(d2ap); i++)
                d1ap[i]='0';
            d1ap[i]='\0';
            //cout << "d1ap=" << d1ap << endl;
            //cout << "d2ap=" << d2ap << endl;
        }
        //cout << d1 << d2 << endl;
        delete [] d1;
        d1=new char[strlen(d1bp)+strlen(d1ap)+2];
        strcpy(d1,d1bp);
        strcat(d1,".");
        strcat(d1,d1ap);
        delete [] d2;
        d2=new char[strlen(d2bp)+strlen(d2ap)+2];
        strcpy(d2,d2bp);
        strcat(d2,".");
        strcat(d2,d2ap);

        delete [] d1bp;
        delete [] d1ap;
        delete [] d2bp;
        delete [] d2ap;

        //cout << d1 << endl;
        //cout << d2 << endl;
    }
}*/

/*Value operator*(Value d1, Value d2)
{
    Value result;

    bool minus=false;
    if (((strchr(d1.var,'-')!=NULL) || (strchr(d2.var,'-')!=NULL)) &&
    !((strchr(d1.var,'-')!=NULL) && (strchr(d2.var,'-')!=NULL)))
        minus=true;
    abs(d1.var);
    abs(d2.var);

    //cout << "d11=" << d1.var << endl;
    //cout << "d22=" << d2.var << endl;

    first_zero(d1.var);
    first_zero(d2.var);

    result.var=new char[2];
    strcpy(result.var,"0");
    if ((strpbrk(d1.var,"123456789")==NULL) || (strpbrk(d2.var,"123456789")==NULL)) // zero
        return result;

    Value one("1");
    Value counter;
    counter.var=new char[2];
    strcpy(counter.var,"0");

    *//*if (strcmp(d2.var,d1.var)==1)
    {
        char* d=new char[strlen(d1.var)+1];
        strcpy(d,d1.var);
        delete [] d1.var;
        d1.var=new char[strlen(d2.var)+1];
        strcpy(d1.var,d2.var);
        delete [] d2.var;
        d2.var=new char[strlen(d)+1];
        strcpy(d2.var,d);
        delete [] d;
        cout << "yes" << endl;
    }*/
/*	int pos1=0;
    if (strchr(d1.var,'.')!=NULL) // determine the number of characters after '.'
    {
        char* n=strchr(d1.var,'.');
        pos1=n-d1.var;
        pos1=strlen(d1.var)-pos1-1;
        if (d1.var[0]=='.')
            pos1=strlen(d1.var)-1;
        //cout << "pos1=" << pos1 << endl;

        char* d=new char[strlen(d1.var)+1];
        strcpy(d,d1.var);
        //cout << "d=" << d << endl;
        delete [] d1.var;

        d1.var=new char[strlen(d)];
        unsigned int i=0, j=0;
        while (i<strlen(d))
        {
            if (d[i]!='.')
                d1.var[j++]=d[i++];
            else
                i++;
        }
        //cout << strlen(d) << endl;
        d1.var[strlen(d)-1]='\0';
        delete [] d;
    }
    //cout << d1.var << endl;

    if (d1.var[0]=='0') // remove extra zeros
    {
        char* d=new char[strlen(d1.var)+1];
        strcpy(d,d1.var);
        delete [] d1.var;
        d1.var=new char[strlen(d)];
        unsigned int i=0, j=0;
        while (d[i]=='0')
            i++;
        while (i<strlen(d))
        {
            d1.var[j++]=d[i++];
        }
        d1.var[j]='\0';
        delete [] d;
    }

    int pos2=0;
    if (strchr(d2.var,'.')!=NULL) // determine the number of characters after '.'
    {
        char* n=strchr(d2.var,'.');
        pos2=n-d2.var;
        pos2=strlen(d2.var)-pos2-1;
        if (d2.var[0]=='.')
            pos2=strlen(d2.var)-1;
        //cout << "pos2=" << pos2 << endl;

        char* d=new char[strlen(d2.var)+1];
        strcpy(d,d2.var);
        //cout << "d=" << d << endl;
        delete [] d2.var;

        d2.var=new char[strlen(d)];
        unsigned int i=0, j=0;
        while (i<strlen(d))
        {
            if (d[i]!='.')
                d2.var[j++]=d[i++];
            else
                i++;
        }
        //cout << strlen(d) << endl;
        d2.var[strlen(d)-1]='\0';
        delete [] d;
    }
    //cout << d2.var << endl;

    if (d2.var[0]=='0') // remove extra zeros
    {
        char* d=new char[strlen(d2.var)+1];
        strcpy(d,d2.var);
        delete [] d2.var;
        d2.var=new char[strlen(d)];
        unsigned int i=0, j=0;
        while (d[i]=='0')
            i++;
        while (i<strlen(d))
        {
            d2.var[j++]=d[i++];
        }
        d2.var[j]='\0';
        delete [] d;
    }

    if ((strlen(d2.var)>strlen(d1.var)) || ((strlen(d2.var)==strlen(d1.var)) && (strcmp(d2.var,d1.var)==1))) // d2 > d1
    {
        char* d=new char[strlen(d1.var)+1];
        strcpy(d,d1.var);
        delete [] d1.var;
        d1.var=new char[strlen(d2.var)+1];
        strcpy(d1.var,d2.var);
        delete [] d2.var;
        d2.var=new char[strlen(d)+1];
        strcpy(d2.var,d);
        delete [] d;
        //cout << "yes" << endl;
    }
    //cout << "d11=" << d1.var << endl;
    //cout << "d22=" << d2.var << endl;


    while (strcmp(counter.var,d2.var)!=0) // multiplication
    {
        //cout << "strcmp=" << strcmp(counter.var,d2.var) << endl;
        result=result+d1;
        counter=counter+one;
        //cout << "result=" << result.var << " counter=" << counter.var << " d2=" << d2.var << endl;
    }
    //cout << "strcmp=" << strcmp(counter.var,d2.var) << endl;
    //cout << "otvet=" << result.var << endl;

    unsigned int pos=0;
    *//*if (strchr(d1.var,'.')!=NULL)
        pos=strchr(d1.var,'.')-d1.var;
    else
        pos=0;*/
// cout << "pos=" << pos << endl;
/*	pos=pos1+pos2;
    //cout << "pos=" << pos << endl;

    if (pos<strlen(result.var)) // define position '.' as a result
    {
        char* d=new char[strlen(result.var)+1];
        strcpy(d,result.var);
        delete [] result.var;
        result.var=new char[strlen(d)+1+1];
        unsigned int i=0, j=0;
        while (i<strlen(d))
        {
            if (j==strlen(d)-pos)
                result.var[j++]='.';
            else
                result.var[j++]=d[i++];
        }
        result.var[j]='\0';
        delete [] d;
    }
    else
    {
        *//*cout << pos << endl;
        if (int(strlen(d))-pos<0)
        {
            reverse(result.var);
            char* d=new char[100];
            strcpy(d,result.var);
        }*/
//		char* d=new char[/*strlen(result.var)+*/pos+2+1];
/*		for (unsigned int i=0; i<pos+2; i++)
            d[i]='0';
        d[pos+2]='\0';
        //d[strlen(d)-pos-1]='.';
        //cout << "d=" << d << endl;
        reverse(d);
        reverse(result.var);
        i=0;
        unsigned int j=0;
        while (i<strlen(result.var))
        {
            d[j++]=result.var[i++];
        }
        while (j<pos)
        {
            d[j++]='0';
        }
        d[j++]='.';
        d[j++]='0';
        d[j]='\0';
        reverse(d);
        //cout << "d=" << d << endl;
        delete [] result.var;
        result.var=new char[strlen(d)+1];
        strcpy(result.var,d);
        delete [] d;
    }

    *//*if (result.var[0]=='.')
    {
        char* d=new char[strlen(result.var)+1];
        strcpy(d,result.var);
        delete [] result.var;
        result.var=new char[strlen(d)+1+1];
        result.var[0]='0';
        for (int i=0; i<strlen(d); i++)
            result.var[i+1]=d[i];
        result.var[strlen(d)+1]='\0';
        delete [] d;
    }*/

/*	if (minus)
        result=-result;

    //cout << "result '*' = " << result.var << endl;
    return result;
}*/
