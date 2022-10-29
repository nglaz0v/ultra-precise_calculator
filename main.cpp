#include "expression.h"

#include <ctime>
#include <iostream>
#include <map>

int main()
{
    Expression::setup();

    Expression expr;
#if 1
    std::string str;
    while (true) {
        std::cout << "\nEnter calculated expression: " << std::endl;
        std::cin >> str;
        if (str == "exit") break;

        expr.prepare(str);
        std::cout << "\nresult=" << expr.compute() << std::endl;

        while (std::cin.get() != '\n') continue;
    }
#else
#if 0
    expr.prepare("!(4000)");
    std::string result;
    const auto t1 = time(nullptr);
    const auto T1 = clock();
    for (int i = 0; i < 1; ++i) result = expr.compute();
    const auto t2 = time(nullptr);
    const auto T2 = clock();
    const auto t = t2 - t1;
    const auto T = T2 - T1;
    std::cout << "Time=" << t << "\t" << T << "\t" << (float(T) / CLOCKS_PER_SEC) << std::endl;
    std::cout << "\nresult=" << result << std::endl;
#else
    constexpr size_t NDIGITS = 30;
    const std::map<std::string, std::pair<std::string, size_t>> examples{
        {"!(1000)", {"402387260077093773543702433923", 2568}},
        {"!(2000)", {"331627509245063324117539338057", 5736}},
        {"!(3000)", {"414935960343785408555686709308", 9131}},
        {"!(4000)", {"182880195151406501331474317557", 12674}},
    };
    for (const auto &example : examples) {
        const std::string a = example.first;
        const std::string b = example.second.first;
        const auto c = example.second.second;
        expr.prepare(a);
        const std::string x = expr.compute();
        std::cout << x << std::endl;
        std::cout << x.substr(0, NDIGITS) << "\t" << x.length() << std::endl;
        std::cout << b << "\t" << c << std::endl;
        std::cout << (x.substr(0, NDIGITS) == b) << "\t" << (x.length() == c) << std::endl;
    }
#endif
#endif
    std::cout << "Press 'Enter'" << std::endl;
    std::cin.get();
    return 0;
}
