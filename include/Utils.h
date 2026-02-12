#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <ctime>
#include <climits>

using namespace std;

/**
 * Utility functions for the Online Judge System
 */
namespace Utils {
    // String manipulation
    string trim(const string& str);
    string toLower(const string& str);
    string toUpper(const string& str);
    vector<string> split(const string& str, char delimiter);
    string join(const vector<string>& vec, const string& delimiter);
    bool startsWith(const string& str, const string& prefix);
    bool endsWith(const string& str, const string& suffix);

    // File operations
    bool fileExists(const string& path);
    bool directoryExists(const string& path);
    bool createDirectory(const string& path);
    string readFile(const string& path);
    bool writeFile(const string& path, const string& content);
    bool deleteFile(const string& path);
    vector<string> listFiles(const string& directory);
    long long getFileSize(const string& path);

    // Time utilities
    string getCurrentTimestamp();
    string formatTime(time_t time);
    string formatDuration(double milliseconds);
    time_t parseTimestamp(const string& timestamp);

    // Validation
    bool isValidUsername(const string& username);
    bool isValidEmail(const string& email);
    bool isValidPassword(const string& password);
    bool isValidProblemId(const string& problemId);

    // Logging
    void log(const string& message, const string& logFile = "logs/system.log");
    void logError(const string& message, const string& logFile = "logs/errors.log");
    void logInfo(const string& message);
    void logWarning(const string& message);

    // Console utilities
    void clearScreen();
    void printHeader(const string& title);
    void printSeparator(char ch = '=', int length = 50);
    void printSuccess(const string& message);
    void printError(const string& message);
    void printWarning(const string& message);
    void printInfo(const string& message);
    string getInput(const string& prompt);
    string getPassword(const string& prompt);
    int getIntInput(const string& prompt, int min = INT_MIN, int max = INT_MAX);

    // Hashing (for password storage)
    string hashPassword(const string& password);
    bool verifyPassword(const string& password, const string& hash);

    // System commands
    string executeSystemCommand(const string& command);
    bool isCommandAvailable(const string& command);

    // Random generation
    string generateRandomId(int length = 16);
    int generateRandomNumber(int min, int max);
}

#endif // UTILS_H
