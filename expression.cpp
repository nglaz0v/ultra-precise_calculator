#include "expression.h"

#include <climits>
#include <ctime>
#include <iostream>
#include <stack>
#if (defined USE_VALUE) || (defined USE_VERYLONG) || (defined USE_BIGINT)
#ifdef USE_VALUE
#include "value.h"
#endif // USE_VALUE
#ifdef USE_VERYLONG
#include "verylong.h"
#endif // USE_VERYLONG
#ifdef USE_BIGINT
#include "big_int.hpp"
#endif // USE_BIGINT
#else
#error "Choose number type: USE_VALUE | USE_VERYLONG | USE_BIGINT"
#endif // USE_VALUE || USE_VERYLONG || USE_BIGINT

// clang-format off
const Expression::States Expression::SM[16][17] =
{	//   0-9        .                   -           +*/^        (           )           !           s           i           n           c       o       t           a           q       r           eol
    { st_int_part,  st_flt_part_pnt,    st_uminus,  error,      st_braL,    error,      st_fact,    st_s,       error,      error,      st_c,   error,  st_t,       error,      error,  error,      error }, //0
    { st_int_part,  st_flt_part_pnt,    st_operat,  st_operat,  error,      st_braR,    error,      error,      error,      error,      error,  error,  error,      error,      error,  error,      OK    }, //1
    { st_flt_part,  error,              error,      error,      error,      error,      error,      error,      error,      error,      error,  error,  error,      error,      error,  error,      error }, //2
    { st_int_part,  st_flt_part_pnt,    error,      error,      st_braL,    error,      st_fact,    st_s,       error,      error,      st_c,   error,  st_t,       error,      error,  error,      error }, //3
    { st_int_part,  st_flt_part_pnt,    st_uminus,  error,      st_braL,    error,      st_fact,    st_s,       error,      error,      st_c,   error,  st_t,       error,      error,  error,      error }, //4
    { error,        error,              error,      error,      st_braL,    error,      error,      error,      error,      error,      error,  error,  error,      error,      error,  error,      error }, //5
    { error,        error,              error,      error,      error,      error,      error,      error,      st_sin_tan, error,      error,  error,  error,      error,      st_q,   error,      error }, //6
    { error,        error,              error,      error,      error,      error,      error,      error,      error,      error,      error,  st_cos, error,      error,      error,  error,      error }, //7
    { error,        error,              error,      error,      error,      error,      error,      error,      error,      error,      error,  error,  error,      st_sin_tan, error,  error,      error }, //8
    { st_int_part,  st_flt_part_pnt,    error,      error,      st_braL,    error,      st_fact,    st_s,       error,      error,      st_c,   error,  st_t,       error,      error,  error,      error }, //9
    { error,        error,              st_operat,  st_operat,  error,      st_braR,    error,      error,      error,      error,      error,  error,  error,      error,      error,  error,      OK    }, //10
    { st_flt_part,  error,              st_operat,  st_operat,  error,      st_braR,    error,      error,      error,      error,      error,  error,  error,      error,      error,  error,      OK    }, //11
    { error,        error,              error,      error,      error,      error,      error,      error,      error,      st_braL,    error,  error,  error,      error,      error,  error,      error }, //12
    { error,        error,              error,      error,      error,      error,      error,      error,      error,      error,      error,  error,  error,      error,      error,  st_sqrt,    error }, //13
    { error,        error,              error,      error,      error,      error,      error,      st_braL,    error,      error,      error,  error,  error,      error,      error,  error,      error }, //14
    { error,        error,              error,      error,      error,      error,      error,      error,      error,      error,      error,  error,  st_braL,    error,      error,  error,      error }, //15
};  //  0               1                   2           3           4           5           6           7           8           9           10      11      12          13          14      15          16
// clang-format on

signed char Expression::Token::getPrior() const
{
    signed char r = -1;
    switch (this->type) {
    case number: r = -1; break;
    case bracketL: r = 0; break;
    case bracketR: r = 1; break;
    case operate:
        switch (this->value[0]) {
        case '+':
        case '-': r = 2; break;
        case '*':
        case '/': r = 3; break;
        case '^': r = 5; break;
        }
        break;
    case function: r = (this->value[0] == '-') ? 4 : 6; break;
    }
    return r;
}

//! string to number conversion
bool str2long(const char *str, long &val)
{
    char *endptr = nullptr;
    errno = 0; // To distinguish success/failure after call
    val = strtol(str, &endptr, 10);
    // Check for various possible errors
    if (((errno == ERANGE) && ((val == LONG_MAX) || (val == LONG_MIN))) || ((errno != 0) && (val == 0))) {
        perror("strtol");
        return false;
    }
    if (endptr == str) {
        std::cerr << "No digits were found" << std::endl;
        return false;
    }
    if (*endptr != '\0') {
        std::cerr << "Further characters after number: " << endptr << std::endl;
        return false;
    }
    // If we got here, strtol() successfully parsed a number
    return true;
}

void Expression::setup()
{
#ifdef USE_VALUE
    std::string str;
    long val = 0;
    bool input = false;
    do {
        std::cout << "\nEnter calculation accuracy - ";
        std::cin >> str;
        input = str2long(str.c_str(), val);
    } while (!input);
    Value::exactness = static_cast<unsigned int>(val);
#endif // USE_VALUE
}

bool Expression::checkFormula(const std::string &num)
{
    States CurSt = st_start;
    int col = 0;
    for (char ch : num) {
        /*cout << endl << "i=" << i << endl;
        cout << "s[i]=" << s[i] << endl;*/
        switch (ch) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': col = 0; break;
        case '.': col = 1; break;
        case '-': col = 2; break;
        case '+':
        case '*':
        case '/':
        case '^': col = 3; break;
        case '(': col = 4; break;
        case ')': col = 5; break;
        case '!': col = 6; break;
        case 's': col = 7; break;
        case 'i': col = 8; break;
        case 'n': col = 9; break;
        case 'c': col = 10; break;
        case 'o': col = 11; break;
        case 't': col = 12; break;
        case 'a': col = 13; break;
        case 'q': col = 14; break;
        case 'r': col = 15; break;
        default: CurSt = error;
        };

        if (CurSt == error) {
            // cout << "Error" << endl;
            return false;
        }
        CurSt = SM[CurSt][col];
        // cout << "CurSt-" << CurSt << endl;
    }
    // cout << "CurSt=" << CurSt << endl;
    if (CurSt == error) {
        // cout << "Error" << endl;
        return false;
    }
    CurSt = SM[CurSt][16];
    // cout << "CurSt=" << CurSt << endl;
    return (CurSt == OK);
}

void Expression::print() const
{
    std::cout << "-------------------------" << std::endl;
    for (const auto &token : tokens) {
        std::string tip;
        switch (token.type) {
        case number: tip = "number"; break;
        case operate: tip = "operation"; break;
        case bracketL: tip = "bracket("; break;
        case bracketR: tip = "bracket)"; break;
        case function: tip = "function"; break;
        }
        std::cout << token.value << " : " << tip << std::endl;
    }
    std::cout << "=========================" << std::endl;
}

void Expression::prepare(const std::string &expr)
{
    tokens.clear();

    if (!checkFormula(expr)) { // expression failed state machine check
        std::cerr << "\a\nError in expression" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < expr.length(); ++i) {
        std::string ss(expr);
        size_t j = 0;
        TokenTypes tp;
        bool unary; // "unary minus" function

        switch (expr[i]) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
            while ((expr[i] >= '0' && expr[i] <= '9') || (expr[i] == '.')) { ss[j++] = expr[i++]; }
            i--;
            ss[j] = '\0';
            tp = number;
            break;

        case '+':
        case '*':
        case '/':
        case '^':
            ss[0] = expr[i];
            ss[1] = '\0';
            tp = operate;
            break;

        case '(':
            ss[0] = expr[i];
            ss[1] = '\0';
            tp = bracketL;
            break;

        case ')':
            ss[0] = expr[i];
            ss[1] = '\0';
            tp = bracketR;
            break;

        case '-':
            unary = false;
            if (i == 0) {
                unary = true;
            } else {
                if (expr[i - 1] == '(') { unary = true; }
            }
            ss[0] = expr[i];
            ss[1] = '\0';
            if (unary) { // "unary minus" function
                tp = function;
            } else { // minus operation
                tp = operate;
            }
            break;

        case 's':
        case 'c':
        case 't':
        case '!':
            while (expr[i] != '(') { ss[j++] = expr[i++]; }
            i--;
            ss[j] = '\0';
            tp = function;
            break;

        default: // error
            std::cerr << "\a\nError in expression: " << expr[i] << std::endl;
            exit(EXIT_FAILURE);
        }
        tokens.emplace_back(Token(tp, ss.c_str())); // add token to list
    }

    std::cout << "\nString is divided into tokens" << std::endl;
    this->print();

    this->postfix();
    std::cout << "\nString is converted to postfix" << std::endl;
    this->print();
}

void Expression::postfix()
{
    if (tokens.empty()) {
        std::cerr << "\a\nEmpty string" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::list<Token> ths;
    std::stack<Token> tmp_stack;

    for (const auto &token : tokens) {
        Token tmp;
        switch (token.type) {
        case number:
            ths.push_back(token); // put a number in the output string
            break;

        case bracketL:
            tmp_stack.push(token); // put the opening brace on the stack
            break;

        case bracketR:
            do { // pop all operations up to the opening parenthesis from the stack
                if (tmp_stack.empty()) {
                    std::cerr << "\a\nUnpaired bracket ')'" << std::endl;
                    exit(EXIT_FAILURE);
                } else {
                    tmp = tmp_stack.top();
                    tmp_stack.pop();
                    if (tmp.type != bracketL) { ths.push_back(tmp); }
                }
            } while (tmp.type != bracketL);
            break;

        case operate:
        case function:
            // if the stack is not empty, then we extract from the stack to the output string all operations/functions
            // with greater or equal priority
            while (!tmp_stack.empty() && tmp_stack.top().getPrior() >= token.getPrior()) {
                ths.push_back(tmp_stack.top());
                tmp_stack.pop();
            }
            tmp_stack.push(token); // push the operation/function onto the stack
            break;
        }
    }
    // input line ended
    while (!tmp_stack.empty()) {
        const Token tmp = tmp_stack.top();
        tmp_stack.pop();
        if (tmp.type == bracketL) {
            std::cerr << "\a\nUnpaired bracket '('" << std::endl;
            exit(EXIT_FAILURE);
        } else {
            ths.push_back(tmp); // transfer the remaining operations/functions to the output string
        }
    }
    this->tokens.clear();
    for (const auto &tkn : ths) { this->tokens.push_back(tkn); }
}

std::string Expression::compute()
{
#ifdef USE_VALUE
    using value_t = Value;
#endif // USE_VALUE
#ifdef USE_VERYLONG
    using value_t = Verylong;
#endif // USE_VERYLONG
#ifdef USE_BIGINT
    using value_t = BigInt<1024>;
#endif // USE_BIGINT

    if (tokens.empty()) {
        std::cerr << "\a\nEmpty string" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::stack<Token> tmp_stack;
    value_t sv; // result-expression

#ifndef USE_VALUE
    value_t result;
    value_t i;
#endif // !USE_VALUE

    for (const auto &token : tokens) {
        time_t t1;
        time_t t2;
        std::string d1; // operand 1
        std::string d2; // operand 2
        std::string d0; // operand
        switch (token.type) {
        case number: {
            tmp_stack.push(token); // push a number onto the stack
            break;
        }
        case operate: {
            if (!tmp_stack.empty()) {
                d2 = tmp_stack.top().value;
                tmp_stack.pop();
                // cout << d2 << endl;
            } else {
                std::cerr << "\a\nNot enough operands" << std::endl;
                exit(EXIT_FAILURE);
            }
            if (!tmp_stack.empty()) {
                d1 = tmp_stack.top().value;
                tmp_stack.pop();
                // cout << d1 << endl;
            } else {
                std::cerr << "\a\nNot enough operands" << std::endl;
                exit(EXIT_FAILURE);
            }
            switch (token.value[0]) {
            case '+': sv = value_t(d1) + value_t(d2); break;
            case '-': sv = value_t(d1) - value_t(d2); break;
            case '*': sv = value_t(d1) * value_t(d2); break;
            case '/': sv = value_t(d1) / value_t(d2); break;
#ifdef USE_VALUE
            case '^': sv = value_t(d1) ^ value_t(d2); break;
#endif // USE_VALUE
#ifdef USE_VERYLONG
            case '^': sv = pow(value_t(d1), value_t(d2)); break;
#endif // USE_VERYLONG
            }
            tmp_stack.push(Token(number, std::string(sv).c_str()));
            break;
        }
        case function: {
            if (!tmp_stack.empty()) {
                d0 = tmp_stack.top().value;
                tmp_stack.pop();
                // cout << d0 << endl;
            } else {
                std::cerr << "\a\nNot enough operands" << std::endl;
                exit(EXIT_FAILURE);
            }
            switch (token.value[0]) {
            case '-':
                sv = value_t(d0);
                sv = -sv;
                break;
            case '!': sv = value_t(d0); t1 = time(nullptr);
#ifdef USE_VALUE
                sv.factorial();
#else  // !USE_VALUE
                result = value_t("1");
                for (i = value_t("2"); i <= sv; ++i) { result *= i; }
                sv = result;
#endif // USE_VALUE
                t2 = time(nullptr);
                std::cout << "Time=" << (t2 - t1) << std::endl;
                break;
            case 's':
                sv = value_t(d0);
                if (token.value[1] == 'q') {
#ifdef USE_VALUE
                    sv.sqrt();
#endif // USE_VALUE
#ifdef USE_VERYLONG
                    sv = sqrt(sv);
#endif // USE_VERYLONG
                } else {
#ifdef USE_VALUE
                    sv.sin();
#endif // USE_VALUE
#ifdef USE_VERYLONG
                    sv = sin(sv);
#endif // USE_VERYLONG
                }
                break;
            case 'c': sv = value_t(d0);
#ifdef USE_VALUE
                sv.cos();
#endif // USE_VALUE
#ifdef USE_VERYLONG
                sv = cos(sv);
#endif // USE_VERYLONG
                break;
            case 't': sv = value_t(d0);
#ifdef USE_VALUE
                sv.tan();
#endif // USE_VALUE
#ifdef USE_VERYLONG
                sv = tan(sv);
#endif // USE_VERYLONG
                break;
            }
            tmp_stack.push(Token(number, std::string(sv).c_str()));
            break;
        }
        case bracketL:
        case bracketR: break;
        }
    }
    // const Token result = tmp_stack.top();
    // tmp_stack.pop();
    return (!tmp_stack.empty()) ? tmp_stack.top().value : std::string{"???"};
}
