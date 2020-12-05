#include "expression.h"
#include <ctime>
#include <iostream>
#include <stack>
#define USE_VERYLONG
#ifndef USE_VERYLONG
#include "value.h"
#else // USE_VERYLONG
#include "verylong.h"
typedef Verylong Value;
#endif // !USE_VERYLONG

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

// возвращает приоритет элемента
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

// проверить число на корректность с помощью конечного автомата
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

// распечатать список
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

// разбить строку на лексемы и преобразовать в список
void Expression::prepare(const std::string &expr)
{
    tokens.clear();

    if (!checkFormula(expr)) { // выражение не прошло проверку конечным автоматом
        std::cerr << "\a\nError in expression" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < expr.length(); ++i) {
        std::string ss(expr);
        size_t j = 0;
        TokenTypes tp;
        bool unary; // функция "унарный минус"

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
            if (unary) { // функция "унарный минус"
                tp = function;
            } else { // операция "минус"
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

        default: // ошибка в выражении
            std::cerr << "\a\nError in expression: " << expr[i] << std::endl;
            exit(EXIT_FAILURE);
        }
        tokens.emplace_back(Token(tp, ss)); // добавить лексему в список
    }

    std::cout << "\nString is divided into tokens" << std::endl;
    this->print();

    this->postfix();
    std::cout << "\nString is converted to postfix" << std::endl;
    this->print();
}

// преобразует список в польскую инверсную запись
void Expression::postfix()
{
    if (tokens.empty()) { // пустая строка
        std::cerr << "\a\nEmpty string" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::list<Token> ths;
    std::stack<Token> tmp_stack;

    for (const auto &token : tokens) {
        Token tmp;
        switch (token.type) {
        case number:              //число
            ths.push_back(token); // помещаем число в выходную строку
            break;

        case bracketL:             //открывающая скобка
            tmp_stack.push(token); // кладём открывающую скобку в стек
            break;

        case bracketR: // закрывающая скобка
            do { // извлекаем из стека все операции до отрывающей скобки
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
        case function: // операция или функция
            // если стек не пуст, то извлекаем из стека в выходную строку все операции/функции
            // с большим или равным приоритетом
            while (!tmp_stack.empty() && tmp_stack.top().getPrior() >= token.getPrior()) {
                ths.push_back(tmp_stack.top());
                tmp_stack.pop();
            }
            tmp_stack.push(token); // помещаем операцию/функцию в стек
            break;
        }
    }
    // входная строка кончилась
    while (!tmp_stack.empty()) {
        const Token tmp = tmp_stack.top();
        tmp_stack.pop();
        if (tmp.type == bracketL) {
            std::cerr << "\a\nUnpaired bracket '('" << std::endl;
            exit(EXIT_FAILURE);
        } else {
            ths.push_back(tmp); // переносим оставшиеся операции/функции в выходную строку
        }
    }
    this->tokens.clear();
    for (const auto &tkn : ths) { this->tokens.push_back(tkn); }
}

// вычислить значение выражения
std::string Expression::compute()
{
    if (tokens.empty()) { // входная строка пуста
        std::cerr << "\a\nEmpty string" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::stack<Token> tmp_stack;
    Value sv; // выражение-результат

#ifdef USE_VERYLONG
    Verylong result;
    Verylong i;
#endif // USE_VERYLONG

    for (const auto &token : tokens) {
        time_t t1;
        time_t t2;
        std::string d1; // операнд 1
        std::string d2; // операнд 2
        std::string d0; // операнд
        switch (token.type) {
        case number: {             // число
            tmp_stack.push(token); // помещаем число в стек
            break;
        }
        case operate: { // операция
            if (!tmp_stack.empty()) {
                d2 = tmp_stack.top().value;
                tmp_stack.pop();
                // cout << d2 << endl;
            } else { // не хватает операндов
                std::cerr << "\a\nNot enough operands" << std::endl;
                exit(EXIT_FAILURE);
            }
            if (!tmp_stack.empty()) {
                d1 = tmp_stack.top().value;
                tmp_stack.pop();
                // cout << d1 << endl;
            } else { // не хватает операндов
                std::cerr << "\a\nNot enough operands" << std::endl;
                exit(EXIT_FAILURE);
            }
            switch (token.value[0]) {
            case '+': sv = Value(d1.c_str()) + Value(d2.c_str()); break;
            case '-': sv = Value(d1.c_str()) - Value(d2.c_str()); break;
            case '*': sv = Value(d1.c_str()) * Value(d2.c_str()); break;
            case '/': sv = Value(d1.c_str()) / Value(d2.c_str()); break;
#ifndef USE_VERYLONG
            case '^': sv = Value(d1.c_str()) ^ Value(d2.c_str()); break;
#else  // USE_VERYLONG
            case '^': sv = pow(Value(d1.c_str()), Value(d2.c_str())); break;
#endif // !USE_VERYLONG
            }
            tmp_stack.push(Token(number, std::string(sv)));
            break;
        }
        case function: {
            if (!tmp_stack.empty()) {
                d0 = tmp_stack.top().value;
                tmp_stack.pop();
                // cout << d0 << endl;
            } else { // не хватает операндов
                std::cerr << "\a\nNot enough operands" << std::endl;
                exit(EXIT_FAILURE);
            }
            switch (token.value[0]) {
            case '-':
                sv = Value(d0.c_str());
                sv = -sv;
                break;
            case '!': sv = Value(d0.c_str()); t1 = time(nullptr);
#ifndef USE_VERYLONG
                sv.factorial();
#else  // USE_VERYLONG
                result = Verylong("1");
                for (i = Verylong("2"); i <= sv; ++i) { result *= i; }
                sv = result;
#endif // !USE_VERYLONG
                t2 = time(nullptr);
                std::cout << "Time=" << (t2 - t1) << std::endl;
                break;
            case 's':
                sv = Value(d0.c_str());
                if (token.value[1] == 'q') {
#ifndef USE_VERYLONG
                    sv.sqrt();
#else  // USE_VERYLONG
                    sv = sqrt(sv);
#endif // !USE_VERYLONG
                } else {
#ifndef USE_VERYLONG
                    sv.sin();
#else  // USE_VERYLONG
                    sv = sin(sv);
#endif // !USE_VERYLONG
                }
                break;
            case 'c': sv = Value(d0.c_str());
#ifndef USE_VERYLONG
                sv.cos();
#else  // USE_VERYLONG
                sv = cos(sv);
#endif // !USE_VERYLONG
                break;
            case 't': sv = Value(d0.c_str());
#ifndef USE_VERYLONG
                sv.tan();
#else  // USE_VERYLONG
                sv = tan(sv);
#endif // !USE_VERYLONG
                break;
            }
            tmp_stack.push(Token(number, std::string(sv)));
            break;
        }
        case bracketL:
        case bracketR: break;
        }
    }
    // const Token result = tmp_stack.top();
    // tmp_stack.pop();
    return tmp_stack.top().value;
}
