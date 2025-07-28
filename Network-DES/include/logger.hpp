#pragma once
#include <string>
// #include <iomanip>
// #include <format>

/*
Simple logger for network simulation.
Provides basic logging at different levels (DEBUG, INFO, WARNING, ERROR) with optional component identification.
*/

namespace NetSim {
class Logger {
  public:
    enum Level { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3 };
    enum Component { SIMULATOR, HOST, ROUTER, LINK, INTERFACE, PACKET, GENERAL };

  private:
    static Level currentLevel_;

  public:
    Logger() = delete;

    // Main logging methods
    static void log(Level level, double simTime, const std::string &message);
    static void log(Level level, double simTime, Component component, const std::string &componentId,
                    const std::string &message);

    // Configuration
    static void setLevel(Level level);

  private:
    static std::string levelToString(Level level);
    static std::string componentToString(Component component);
    static bool shouldLog(Level level);
};
} // namespace NetSim
