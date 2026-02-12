#ifndef JUDGE_ENGINE_H
#define JUDGE_ENGINE_H

#include "Problem.h"
#include "Submission.h"
#include "Compiler.h"
#include <string>
#include <vector>

using namespace std;

/**
 * ExecutionResult structure - Result of code execution
 */
struct ExecutionResult {
    bool success;
    string output;
    string errorOutput;
    double executionTime;  // in milliseconds
    int memoryUsed;        // in KB
    int exitCode;

    ExecutionResult()
        : success(false), output(""), errorOutput(""), executionTime(0.0), memoryUsed(0), exitCode(-1) {}
};

/**
 * JudgeEngine class - Core judging logic
 */
class JudgeEngine {
private:
    Compiler compiler;
    string submissionsDir;
    string executablesDir;
    string logsDir;

public:
    // Constructor
    JudgeEngine(const string& subDir = "submissions/", 
                const string& exeDir = "executables/", 
                const string& logDir = "logs/");

    // Main judging method
    Submission judge(const string& username, const string& problemId, 
                    const string& sourceCode, const Problem& problem);

    // Execution methods
    ExecutionResult executeCode(const string& executablePath, 
                               const string& inputFile, 
                               const string& outputFile,
                               int timeLimit);

    // Validation methods
    bool validateOutput(const string& userOutput, const string& expectedOutput);
    bool validateOutputFile(const string& userOutputFile, const string& expectedOutputFile);

    // Utility methods
    string saveSourceCode(const string& username, const string& problemId, const string& code);
    void logExecution(const string& message);
    void logCompilationError(const string& username, const string& problemId, const string& error);

private:
    // Helper methods
    string generateSubmissionId(const string& username, const string& problemId);
    string trim(const string& str);
    vector<string> splitLines(const string& text);
    bool compareLines(const string& line1, const string& line2);
    double measureExecutionTime(const string& executablePath, const string& inputFile);
};

#endif // JUDGE_ENGINE_H
