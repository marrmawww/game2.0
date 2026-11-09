#include "Logger.h"
#include <fstream>
#include <iostream>
#include <sstream>

static std::ofstream logFile;
static bool consoleEnabled = true;
static int logCount = 0;

void openLog(const std::string& path) {
    logFile.open(path.c_str());
}

void closeLog() {
    if (logFile.is_open()) logFile.close();
}

void setConsoleLog(bool enabled) {
    consoleEnabled = enabled;
}

int getLogCount() {
    return logCount;
}

void logMessage(const std::string& tag, const std::string& message) {
    logCount = logCount + 1;
    std::string line = "[" + std::to_string(logCount) + "] [" + tag + "] " + message;
    if (consoleEnabled) std::cout << "  " << line << "\n";
    if (logFile.is_open()) logFile << line << "\n";
}

std::string toText(double value) {
    std::ostringstream out;
    out.precision(3);
    out << value;
    return out.str();
}
