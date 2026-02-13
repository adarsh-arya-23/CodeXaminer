#include "../include/Utils.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <random>
#include <climits>
#include <sys/stat.h>

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #define mkdir(path, mode) _mkdir(path)
#else
    #include <unistd.h>
    #include <dirent.h>
#endif

using namespace std;

namespace Utils {

// ==================== String Manipulation ====================

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string toUpper(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

string join(const vector<string>& vec, const string& delimiter) {
    if (vec.empty()) return "";
    string result = vec[0];
    for (size_t i = 1; i < vec.size(); i++) {
        result += delimiter + vec[i];
    }
    return result;
}

bool startsWith(const string& str, const string& prefix) {
    return str.size() >= prefix.size() && 
           str.compare(0, prefix.size(), prefix) == 0;
}

bool endsWith(const string& str, const string& suffix) {
    return str.size() >= suffix.size() && 
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// ==================== File Operations ====================

bool fileExists(const string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISREG(buffer.st_mode));
}

bool directoryExists(const string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

bool createDirectory(const string& path) {
    return mkdir(path.c_str(), 0755) == 0 || directoryExists(path);
}

string readFile(const string& path) {
    ifstream file(path);
    if (!file.is_open()) return "";
    
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

bool writeFile(const string& path, const string& content) {
    ofstream file(path);
    if (!file.is_open()) return false;
    
    file << content;
    file.close();
    return true;
}

bool deleteFile(const string& path) {
    return remove(path.c_str()) == 0;
}

vector<string> listFiles(const string& directory) {
    vector<string> files;
    
#ifdef _WIN32
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((directory + "/*").c_str(), &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                files.push_back(findData.cFileName);
            }
        } while (FindNextFile(hFind, &findData));
        FindClose(hFind);
    }
#else
    DIR* dir = opendir(directory.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) {
                files.push_back(entry->d_name);
            }
        }
        closedir(dir);
    }
#endif
    
    return files;
}

long long getFileSize(const string& path) {
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0) return -1;
    return buffer.st_size;
}

// ==================== Time Utilities ====================

string getCurrentTimestamp() {
    time_t now = time(nullptr);
    return formatTime(now);
}

string formatTime(time_t time) {
    char buffer[80];
    struct tm* timeinfo = localtime(&time);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}

string formatDuration(double milliseconds) {
    if (milliseconds < 1000) {
        return to_string((int)milliseconds) + " ms";
    } else {
        return to_string(milliseconds / 1000.0) + " s";
    }
}

time_t parseTimestamp(const string& timestamp) {
    struct tm tm = {};
    istringstream ss(timestamp);
    ss >> get_time(&tm, "%Y-%m-%d %H:%M:%S");
    return mktime(&tm);
}

// ==================== Validation ====================

bool isValidUsername(const string& username) {
    if (username.length() < 3 || username.length() > 20) return false;
    
    for (char c : username) {
        if (!isalnum(c) && c != '_') return false;
    }
    return true;
}

bool isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find_last_of('.');
    
    return atPos != string::npos && 
           dotPos != string::npos && 
           atPos < dotPos && 
           dotPos < email.length() - 1;
}

bool isValidPassword(const string& password) {
    return password.length() >= 6;
}

bool isValidProblemId(const string& problemId) {
    if (problemId.empty()) return false;
    
    for (char c : problemId) {
        if (!isalnum(c) && c != '_' && c != '-') return false;
    }
    return true;
}

// ==================== Logging ====================

void log(const string& message, const string& logFile) {
    ofstream file(logFile, ios::app);
    if (file.is_open()) {
        file << "[" << getCurrentTimestamp() << "] " << message << endl;
        file.close();
    }
}

void logError(const string& message, const string& logFile) {
    log("ERROR: " + message, logFile);
}

void logInfo(const string& message) {
    log("INFO: " + message, "logs/system.log");
}

void logWarning(const string& message) {
    log("WARNING: " + message, "logs/system.log");
}

// ==================== Console Utilities ====================

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printHeader(const string& title) {
    printSeparator('=', 60);
    cout << "  " << title << endl;
    printSeparator('=', 60);
}

void printSeparator(char ch, int length) {
    cout << string(length, ch) << endl;
}

void printSuccess(const string& message) {
    cout << "[SUCCESS] " << message << endl;
}

void printError(const string& message) {
    cout << "[ERROR] " << message << endl;
}

void printWarning(const string& message) {
    cout << "[WARNING] " << message << endl;
}

void printInfo(const string& message) {
    cout << "[INFO] " << message << endl;
}

string getInput(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin, input);
    return trim(input);
}

string getPassword(const string& prompt) {
    cout << prompt;
    string password;
    
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
    getline(cin, password);
    SetConsoleMode(hStdin, mode);
#else
    // For Unix-like systems, use termios
    getline(cin, password);
#endif
    
    cout << endl;
    return password;
}

int getIntInput(const string& prompt, int min, int max) {
    while (true) {
        string input = getInput(prompt);
        try {
            int value = stoi(input);
            if (value >= min && value <= max) {
                return value;
            }
            printError("Value must be between " + to_string(min) + " and " + to_string(max));
        } catch (...) {
            printError("Invalid number. Please try again.");
        }
    }
}

// ==================== Hashing ====================

string hashPassword(const string& password) {
    // Simple hash for demonstration (use proper hashing in production)
    hash<string> hasher;
    return to_string(hasher(password + "SALT_KEY_12345"));
}

bool verifyPassword(const string& password, const string& hash) {
    return hashPassword(password) == hash;
}

// ==================== System Commands ====================

string executeSystemCommand(const string& command) {
    string tempFile = "temp_output_" + generateRandomId(8) + ".txt";
    string fullCommand = command + " > " + tempFile + " 2>&1";
    
    system(fullCommand.c_str());
    
    string result = readFile(tempFile);
    deleteFile(tempFile);
    
    return result;
}

bool isCommandAvailable(const string& command) {
    string checkCmd = command + " --version 2>&1";
    string output = executeSystemCommand(checkCmd);
    return !output.empty();
}

// ==================== Random Generation ====================

string generateRandomId(int length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);
    
    string id;
    for (int i = 0; i < length; i++) {
        id += alphanum[dis(gen)];
    }
    return id;
}

int generateRandomNumber(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

} // namespace Utils