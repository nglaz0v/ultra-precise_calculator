#include <iostream>
#include <string>
#include <ctime>
#include "expression.h"
#include "value.h"


//! преобразование строки в число
bool str2long(const char *str, long& val)
{
    char *endptr = nullptr;
    errno = 0;  /* To distinguish success/failure after call */
    val = strtol(str, &endptr, 10);
    /* Check for various possible errors */
    if ( ((errno == ERANGE) && ((val == LONG_MAX) || (val == LONG_MIN))) || ((errno != 0) && (val == 0)) ) {
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
    /* If we got here, strtol() successfully parsed a number */
    return true;
}

int main()
{
    Expression expr;
    std::string str;
    long val = 0;
    bool input = false;

    do {
        std::cout << "\nEnter calculation accuracy - ";
        std::cin >> str;
        input = str2long(str.c_str(), val);
    } while (!input);
    Value::exactness = static_cast<unsigned int>(val);
#if 1
    while (true) {
        std::cout << "\nEnter calculated expression: " << std::endl;
        std::cin >> str;
        if (str == "exit") break;

        expr.prepare(str);
        std::cout << "\nresult=" << expr.compute() << std::endl;

        while (std::cin.get() != '\n') continue;
	}
#else
    expr.prepare("!(4000)");
    const time_t t1 = time(nullptr);
    for (int i = 0; i < 1; ++i) expr.compute();
    const time_t t2 = time(nullptr);
    std::cout << "Time=" << (t2 - t1) << std::endl;
#endif
    std::cout << "Press 'Enter'" << std::endl;
    std::cin.get();
	return 0;
}
