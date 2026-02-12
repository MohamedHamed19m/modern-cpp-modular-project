#ifndef LOGGER_LOGGER_HPP
#define LOGGER_LOGGER_HPP

#include <iostream>
#include <string_view>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace MathEngine {

/**
 * @brief Simple thread-safe logging class (header-only implementation)
 *
 * This demonstrates a header-only library pattern:
 * - All implementation in the header
 * - No separate .cpp file needed
 * - Can be included anywhere without linking
 */
class Logger {
public:
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    /**
     * @brief Log a message with the specified level
     * @param level The severity level
     * @param message The message to log
     */
    static void log(Level level, std::string_view message) {
        // Get current time
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ) % 1000;

        // Format timestamp
        std::tm tm;
#ifdef _WIN32
        localtime_s(&tm, &time_t);
#else
        localtime_r(&time_t, &tm);
#endif

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
            << '.' << std::setfill('0') << std::setw(3) << ms.count()
            << " [" << levelToString(level) << "] "
            << message;

        // Colorize output (platform-specific)
        const char* color = getColor(level);
        if (color) {
            std::cerr << color << oss.str() << "\033[0m" << std::endl;
        } else {
            std::cerr << oss.str() << std::endl;
        }
    }

    /**
     * @brief Convenience methods for different log levels
     */
    static void debug(std::string_view message) {
        log(Level::DEBUG, message);
    }

    static void info(std::string_view message) {
        log(Level::INFO, message);
    }

    static void warning(std::string_view message) {
        log(Level::WARNING, message);
    }

    static void error(std::string_view message) {
        log(Level::ERROR, message);
    }

private:
    static const char* levelToString(Level level) {
        switch (level) {
            case Level::DEBUG:   return "DEBUG";
            case Level::INFO:    return "INFO";
            case Level::WARNING: return "WARN";
            case Level::ERROR:   return "ERROR";
            default:             return "UNKNOWN";
        }
    }

    static const char* getColor(Level level) {
#if defined(_WIN32)
        // Windows terminal color support is limited, return nullptr for simplicity
        (void)level;  // Suppress unused parameter warning
        return nullptr;
#else
        switch (level) {
            case Level::DEBUG:   return "\033[36m";  // Cyan
            case Level::INFO:    return "\033[32m";  // Green
            case Level::WARNING: return "\033[33m";  // Yellow
            case Level::ERROR:   return "\033[31m";  // Red
            default:             return "\033[0m";   // Reset
        }
#endif
    }
};

} // namespace MathEngine

#endif // LOGGER_LOGGER_HPP
