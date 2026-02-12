#include "math/calculator.hpp"
#include "logger/logger.hpp"

#include <stdexcept>
#include <cmath>
#include <sstream>

namespace MathEngine {

Calculator::ResultType Calculator::add(ResultType a, ResultType b) {
    Logger::info("Calculating: " + std::to_string(a) + " + " + std::to_string(b));
    lastResult_ = a + b;
    Logger::debug("Result: " + std::to_string(lastResult_));
    return lastResult_;
}

Calculator::ResultType Calculator::subtract(ResultType a, ResultType b) {
    Logger::info("Calculating: " + std::to_string(a) + " - " + std::to_string(b));
    lastResult_ = a - b;
    Logger::debug("Result: " + std::to_string(lastResult_));
    return lastResult_;
}

Calculator::ResultType Calculator::multiply(ResultType a, ResultType b) {
    Logger::info("Calculating: " + std::to_string(a) + " * " + std::to_string(b));
    lastResult_ = a * b;
    Logger::debug("Result: " + std::to_string(lastResult_));
    return lastResult_;
}

Calculator::ResultType Calculator::divide(ResultType a, ResultType b) {
    std::ostringstream oss;
    oss << "Calculating: " << a << " / " << b;
    Logger::info(oss.str());

    if (std::abs(b) < 1e-10) {
        Logger::error("Division by zero attempted!");
        throw std::invalid_argument("Cannot divide by zero");
    }

    lastResult_ = a / b;
    Logger::debug("Result: " + std::to_string(lastResult_));
    return lastResult_;
}

Calculator::ResultType Calculator::power(ResultType base, std::int32_t exp) {
    std::ostringstream oss;
    oss << "Calculating: " << base << "^" << exp;
    Logger::info(oss.str());

    if (exp < 0) {
        Logger::warning("Negative exponent - may lose precision");
    }

    lastResult_ = std::pow(base, static_cast<double>(exp));
    Logger::debug("Result: " + std::to_string(lastResult_));
    return lastResult_;
}

Calculator::ResultType Calculator::getLastResult() {
    Logger::debug("Retrieving last result");
    return lastResult_;
}

} // namespace MathEngine
