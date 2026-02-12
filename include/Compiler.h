#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <vector>

using namespace std;

/**
 * CompilationResult structure - Result of compilation attempt
 */
struct CompilationResult {
    bool success;
    string executablePath;
    string errorMessage;
    vector<string> warnings;

    CompilationResult(bool s = false, string exe = "", string err = "")
        : success(s), executablePath(exe), errorMessage(err) {}
};

/**
 * Compiler class - Handles code compilation
 */
class Compiler {
private:
    string compilerPath;
    string compilerFlags;
    int timeoutSeconds;

public:
    // Constructor
    Compiler(string path = "g++", string flags = "-std=c++17 -O2", int timeout = 10);

    // Compilation methods
    CompilationResult compile(const string& sourceFile, const string& outputFile);
    CompilationResult compileWithFlags(const string& sourceFile, const string& outputFile, const string& customFlags);

    // Utility methods
    bool checkCompilerAvailable();
    string getCompilerVersion();
    void setCompilerFlags(const string& flags);
    void setTimeout(int seconds);

    // Cleanup
    bool removeExecutable(const string& executablePath);

private:
    // Helper methods
    string executeCommand(const string& command);
    bool fileExists(const string& path);
    string extractErrors(const string& compilerOutput);
    vector<string> extractWarnings(const string& compilerOutput);
};

#endif // COMPILER_H
