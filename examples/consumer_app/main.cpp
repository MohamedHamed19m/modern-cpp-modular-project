/**
 * @file main.cpp
 * @brief Example consumer application using installed MathEngine package
 *
 * This demonstrates that find_package() works correctly:
 * - Includes are resolved automatically
 * - No need to specify include paths manually
 * - All dependencies are handled transitively
 */

#include "math/calculator.hpp"

#include <iostream>

int main() {
    std::cout << "========================================\n";
    std::cout << "  Consumer App - find_package() Demo\n";
    std::cout << "========================================\n\n";

    using namespace MathEngine;

    std::cout << "Using MathEngine via find_package():\n\n";

    auto result = Calculator::power(2.0, 8);
    std::cout << "2.0^8 = " << result << "\n";

    result = Calculator::add(result, 10);
    std::cout << "+ 10 = " << result << "\n";

    result = Calculator::divide(result, 3);
    std::cout << "/ 3 = " << result << "\n\n";

    std::cout << "========================================\n";
    std::cout << "Key points:\n";
    std::cout << "========================================\n";
    std::cout << "- No CMakeLists.txt modifications needed\n";
    std::cout << "- Include paths resolved automatically\n";
    std::cout << "- Transitive dependencies handled\n";
    std::cout << "- Works with any C++20 compatible compiler\n";
    std::cout << "========================================\n";

    return 0;
}
