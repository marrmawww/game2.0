#pragma once
#include <string>

void openLog(const std::string& path);
void closeLog();
void setConsoleLog(bool enabled);
void logMessage(const std::string& tag, const std::string& message);
int getLogCount();


std::string toText(double value);
