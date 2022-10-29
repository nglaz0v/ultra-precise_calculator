#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <list>
#include <string>
#include <utility>

//! mathematical expression class
class Expression final {
public:
    Expression() = default;
    ~Expression() = default;

    Expression(const Expression &) = delete;
    Expression &operator=(const Expression &) = delete;
    Expression(Expression &&) noexcept = delete;
    Expression &operator=(Expression &&) noexcept = delete;

    //! print the list
    void print() const;

    //! split the string into tokens and convert to list
    void prepare(const std::string &expr);

    //! calculate the value of the expression
    std::string compute();

private:
    //! check the number for correctness using a state machine
    static bool checkFormula(const std::string &num);

    //! convert the list to inverse polish notation
    void postfix();

private:
    enum TokenTypes { number, operate, bracketL, bracketR, function };

    struct Token {
        TokenTypes type{number};
        std::string value;
        Token() = default;
        Token(TokenTypes _type, std::string _value)
            : type(_type)
            , value(std::move(_value))
        {
        }
        //! return the priority of the token
        signed char getPrior() const;
    };

    //! array values for state machine
    enum States {
        st_start = 0,    //!< initial state
        st_int_part,     //!< integer part set (there were numbers)
        st_flt_part_pnt, //!< fractional set (there were no numbers, there was a point)
        st_uminus,       //!< unary minus, integer part set (there was a sign, there were no numbers)
        st_braL,         //!< "("
        st_fact,         //!< "!"
        st_s,            //!< "s"
        st_c,            //!< "c"
        st_t,            //!< "t"
        st_operat,       //!< operation
        st_braR,         //!< ")"
        st_flt_part,     //!< fractional set (there were numbers)
        st_sin_tan,      //!< "sin", "tan"
        st_q,            //!< "q"
        st_cos,          //!< "cos"
        st_sqrt,         //!< "sqrt"
        error,           //!< fault
        OK               //!< everything is fine
    };

    static const States SM[16][17]; //!< array for state machine

    std::list<Token> tokens; //!< list of tokens
};

#endif // EXPRESSION_H
