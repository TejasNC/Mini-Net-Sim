#include "../include/logger.hpp"
#include <iomanip>
#include <iostream>

namespace NetSim {

Logger::Level Logger::currentLevel_ = Logger::Level::INFO;

void Logger::log(Level level, double simTime, const std::string &message) { log(level, simTime, GENERAL, "", message); }

void Logger::log(Level level, double simTime, Component component, const std::string &componentId,
                 const std::string &message) {
    if (!shouldLog(level)) {
        return;
    }
    // Format time with fixed width (8 characters, 3 decimal places)
    std::cout << "[" << std::setw(5) << std::fixed << std::setprecision(3) << simTime << "] ";

    // Format level with fixed width (5 characters, left-aligned)
    std::cout << std::setw(5) << std::left << levelToString(level) << ": ";

    // Format component with fixed width (8 characters, left-aligned)
    if (component != GENERAL) {
        std::string componentStr = componentToString(component);
        if (!componentId.empty()) {
            componentStr += "(" + componentId + ")";
        }
        std::cout << std::setw(20) << std::left << componentStr;
    } else {
        std::cout << std::setw(20) << " "; // Empty space for GENERAL component
    }

    std::cout << message << std::endl;
}

void Logger::setLevel(Level level) { currentLevel_ = level; }

std::string Logger::levelToString(Level level) {
    switch (level) {
    case DEBUG:
        return "DEBUG";
    case INFO:
        return "INFO";
    case WARNING:
        return "WARN";
    case ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

std::string Logger::componentToString(Component component) {
    switch (component) {
    case SIMULATOR:
        return "SIM";
    case HOST:
        return "HOST";
    case ROUTER:
        return "ROUTER";
    case LINK:
        return "LINK";
    case INTERFACE:
        return "IF";
    case PACKET:
        return "PACKET";
    case GENERAL:
        return "";
    default:
        return "UNKNOWN";
    }
}

bool Logger::shouldLog(Level level) { return level >= currentLevel_; }

} // namespace NetSim
