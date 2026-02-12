#ifndef MATH_CALCULATOR_HPP
#define MATH_CALCULATOR_HPP

#include <cstdint>
#include <limits>

namespace MathEngine {

/**
 * @brief Simple calculator class for demonstrating CMake concepts
 *
 * This class demonstrates:
 * - Header declarations in include/
 * - Implementation in src/ (separate from public headers)
 * - Use of logger library in implementation
 */
class Calculator {
public:
    using ResultType = double;

    /**
     * @brief Add two numbers
     * @param a First operand
     * @param b Second operand
     * @return Sum of a and b
     */
    static ResultType add(ResultType a, ResultType b);

    /**
     * @brief Subtract two numbers
     * @param a First operand
     * @param b Second operand
     * @return Difference of a and b
     */
    static ResultType subtract(ResultType a, ResultType b);

    /**
     * @brief Multiply two numbers
     * @param a First operand
     * @param b Second operand
     * @return Product of a and b
     */
    static ResultType multiply(ResultType a, ResultType b);

    /**
     * @brief Divide two numbers
     * @param a Numerator
     * @param b Denominator
     * @return Quotient of a and b
     * @throws std::invalid_argument if b is zero
     */
    static ResultType divide(ResultType a, ResultType b);

    /**
     * @brief Calculate power (base^exponent)
     * @param base Base number
     * @param exp Exponent (non-negative integer)
     * @return base raised to the power of exp
     */
    static ResultType power(ResultType base, std::int32_t exp);

    /**
     * @brief Get the last calculated value
     * @return Last result or NaN if no calculation performed
     */
    static ResultType getLastResult();

private:
    static inline ResultType lastResult_ = std::numeric_limits<ResultType>::quiet_NaN();
};

} // namespace MathEngine

#endif // MATH_CALCULATOR_HPP
