#include "../include/Compiler.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// ==================== Constructor ====================

Compiler::Compiler(string path, string flags, int timeout)
    : compilerPath(path), compilerFlags(flags), timeoutSeconds(timeout) {}

// ==================== Compilation Methods ====================

CompilationResult Compiler::compile(const string& sourceFile, const string& outputFile) {
    return compileWithFlags(sourceFile, outputFile, compilerFlags);
}

CompilationResult Compiler::compileWithFlags(const string& sourceFile, 
                                             const string& outputFile, 
                                             const string& customFlags) {
    CompilationResult result;
    
    // Check if source file exists
    if (!fileExists(sourceFile)) {
        result.success = false;
        result.errorMessage = "Source file not found: " + sourceFile;
        Utils::logError(result.errorMessage);
        return result;
    }
    
    // Build compilation command
    stringstream cmd;
    cmd << compilerPath << " " << customFlags << " "
        << "\"" << sourceFile << "\" -o \"" << outputFile << "\" 2>&1";
    
    Utils::logInfo("Compiling: " + cmd.str());
    
    // Execute compilation
    string output = executeCommand(cmd.str());
    
    // Check if executable was created
    if (fileExists(outputFile)) {
        result.success = true;
        result.executablePath = outputFile;
        result.warnings = extractWarnings(output);
        
        if (!result.warnings.empty()) {
            Utils::logWarning("Compilation warnings: " + to_string(result.warnings.size()));
        }
        
        Utils::logInfo("Compilation successful: " + outputFile);
    } else {
        result.success = false;
        result.errorMessage = extractErrors(output);
        
        if (result.errorMessage.empty()) {
            result.errorMessage = "Compilation failed (unknown error)";
        }
        
        Utils::logError("Compilation failed: " + result.errorMessage);
    }
    
    return result;
}

// ==================== Utility Methods ====================

bool Compiler::checkCompilerAvailable() {
    return Utils::isCommandAvailable(compilerPath);
}

string Compiler::getCompilerVersion() {
    string cmd = compilerPath + " --version";
    return executeCommand(cmd);
}

void Compiler::setCompilerFlags(const string& flags) {
    compilerFlags = flags;
}

void Compiler::setTimeout(int seconds) {
    timeoutSeconds = seconds;
}

// ==================== Cleanup ====================

bool Compiler::removeExecutable(const string& executablePath) {
    return Utils::deleteFile(executablePath);
}

// ==================== Private Helper Methods ====================

string Compiler::executeCommand(const string& command) {
    return Utils::executeSystemCommand(command);
}

bool Compiler::fileExists(const string& path) {
    return Utils::fileExists(path);
}

string Compiler::extractErrors(const string& compilerOutput) {
    // Extract error messages from compiler output
    stringstream ss(compilerOutput);
    string line;
    string errors;
    
    while (getline(ss, line)) {
        // Look for common error indicators
        if (line.find("error:") != string::npos ||
            line.find("fatal error:") != string::npos ||
            line.find("undefined reference") != string::npos) {
            errors += line + "\n";
        }
    }
    
    // If no specific errors found, return first few lines of output
    if (errors.empty() && !compilerOutput.empty()) {
        stringstream ss2(compilerOutput);
        int lineCount = 0;
        while (getline(ss2, line) && lineCount < 10) {
            errors += line + "\n";
            lineCount++;
        }
    }
    
    return errors;
}

vector<string> Compiler::extractWarnings(const string& compilerOutput) {
    vector<string> warnings;
    stringstream ss(compilerOutput);
    string line;
    
    while (getline(ss, line)) {
        if (line.find("warning:") != string::npos) {
            warnings.push_back(line);
        }
    }
    
    return warnings;
}
