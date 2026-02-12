#include "math/calculator.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

using namespace MathEngine;
using Catch::Matchers::WithinAbsMatcher;
using Catch::Matchers::ContainsSubstring;

// ============================================================================
// Test Suite: Basic Arithmetic Operations
// ============================================================================

TEST_CASE("Calculator::add - Basic addition", "[math][add]") {
    SECTION("Positive numbers") {
        REQUIRE(Calculator::add(2.0, 3.0) == 5.0);
        REQUIRE(Calculator::add(0.0, 0.0) == 0.0);
    }

    SECTION("Negative numbers") {
        REQUIRE(Calculator::add(-2.0, -3.0) == -5.0);
        REQUIRE(Calculator::add(10.0, -5.0) == 5.0);
    }

    SECTION("Floating point precision") {
        REQUIRE_THAT(Calculator::add(0.1, 0.2), WithinAbsMatcher(0.3, 1e-10));
    }
}

TEST_CASE("Calculator::subtract - Basic subtraction", "[math][subtract]") {
    SECTION("Positive numbers") {
        REQUIRE(Calculator::subtract(5.0, 3.0) == 2.0);
    }

    SECTION("Negative result") {
        REQUIRE(Calculator::subtract(3.0, 5.0) == -2.0);
    }

    SECTION("From zero") {
        REQUIRE(Calculator::subtract(0.0, 5.0) == -5.0);
    }
}

TEST_CASE("Calculator::multiply - Basic multiplication", "[math][multiply]") {
    SECTION("Positive numbers") {
        REQUIRE(Calculator::multiply(3.0, 4.0) == 12.0);
    }

    SECTION("With zero") {
        REQUIRE(Calculator::multiply(5.0, 0.0) == 0.0);
    }

    SECTION("Negative numbers") {
        REQUIRE(Calculator::multiply(-2.0, 3.0) == -6.0);
        REQUIRE(Calculator::multiply(-2.0, -3.0) == 6.0);
    }
}

TEST_CASE("Calculator::divide - Basic division", "[math][divide]") {
    SECTION("Normal division") {
        REQUIRE(Calculator::divide(10.0, 2.0) == 5.0);
        REQUIRE(Calculator::divide(7.0, 2.0) == 3.5);
    }

    SECTION("Fractional result") {
        REQUIRE_THAT(Calculator::divide(1.0, 3.0),
            WithinAbsMatcher(0.3333333333, 1e-9));
    }

    SECTION("Division by zero throws exception") {
        REQUIRE_THROWS_AS(Calculator::divide(5.0, 0.0), std::invalid_argument);
        REQUIRE_THROWS_WITH(Calculator::divide(5.0, 0.0),
            ContainsSubstring("Cannot divide by zero"));
    }
}

TEST_CASE("Calculator::power - Power operation", "[math][power]") {
    SECTION("Positive exponent") {
        REQUIRE(Calculator::power(2.0, 0) == 1.0);
        REQUIRE(Calculator::power(2.0, 1) == 2.0);
        REQUIRE(Calculator::power(2.0, 10) == 1024.0);
    }

    SECTION("Negative base") {
        REQUIRE(Calculator::power(-2.0, 2) == 4.0);
        REQUIRE(Calculator::power(-2.0, 3) == -8.0);
    }

    SECTION("Fractional base") {
        REQUIRE_THAT(Calculator::power(1.5, 2),
            WithinAbsMatcher(2.25, 1e-10));
    }
}

// ============================================================================
// Test Suite: State Management
// ============================================================================

TEST_CASE("Calculator::getLastResult - State tracking", "[math][state]") {
    // After an operation, should return the last result
    Calculator::add(10.0, 5.0);
    REQUIRE(Calculator::getLastResult() == 15.0);

    Calculator::multiply(3.0, 7.0);
    REQUIRE(Calculator::getLastResult() == 21.0);
}

// ============================================================================
// Test Suite: Integration Tests
// ============================================================================

TEST_CASE("Calculator - Chained operations", "[math][integration]") {
    // This tests that the calculator maintains state correctly
    // across multiple operations
    auto a = Calculator::add(5.0, 3.0);      // 8
    auto b = Calculator::multiply(a, 2.0);  // 16
    auto c = Calculator::divide(b, 4.0);    // 4
    auto d = Calculator::subtract(c, 1.0);  // 3

    REQUIRE(d == 3.0);
    REQUIRE(Calculator::getLastResult() == 3.0);
}
