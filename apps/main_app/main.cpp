#include "math/calculator.hpp"

#include <iostream>
#include <iomanip>

/**
 * @brief Main application demonstrating the MathEngine library
 *
 * Key CMake concepts demonstrated:
 * - We only include math/calculator.hpp
 * - We didn't need to include logger/logger.hpp explicitly
 * - The include paths are handled automatically via target properties
 * - Linking to MathEngine::math_engine gives us logger automatically
 */
int main() {
    using namespace MathEngine;

    std::cout << "========================================\n";
    std::cout << "  Modern CMake Mastery Demo\n";
    std::cout << "========================================\n\n";

    std::cout << std::fixed << std::setprecision(2);

    // Demonstrate basic operations
    std::cout << "--- Basic Operations ---\n";
    auto sum = Calculator::add(10.0, 5.0);
    std::cout << "10.0 + 5.0 = " << sum << "\n";

    auto diff = Calculator::subtract(10.0, 3.0);
    std::cout << "10.0 - 3.0 = " << diff << "\n";

    auto product = Calculator::multiply(4.0, 7.0);
    std::cout << "4.0 * 7.0 = " << product << "\n";

    auto quotient = Calculator::divide(20.0, 4.0);
    std::cout << "20.0 / 4.0 = " << quotient << "\n\n";

    // Demonstrate power operation
    std::cout << "--- Power Operation ---\n";
    auto power = Calculator::power(2.0, 10);
    std::cout << "2.0^10 = " << power << "\n\n";

    // Demonstrate getLastResult
    std::cout << "--- Last Result ---\n";
    std::cout << "Last result: " << Calculator::getLastResult() << "\n\n";

    // Demonstrate error handling
    std::cout << "--- Error Handling ---\n";
    try {
        Calculator::divide(10.0, 0.0);
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    std::cout << "\n========================================\n";
    std::cout << "  CMake Concepts Demonstrated:\n";
    std::cout << "========================================\n";
    std::cout << "- Transitive dependencies (PUBLIC linking)\n";
    std::cout << "- Target-based include directories\n";
    std::cout << "- Alias targets (MathEngine::math_engine)\n";
    std::cout << "- Header-only + compiled library mix\n";
    std::cout << "========================================\n";

    return 0;
}
