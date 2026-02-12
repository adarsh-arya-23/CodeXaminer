#include "../include/JudgeEngine.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/time.h>
    #include <sys/resource.h>
    #include <unistd.h>
#endif

using namespace std;
using namespace chrono;

// ==================== Constructor ====================

JudgeEngine::JudgeEngine(const string& subDir, const string& exeDir, const string& logDir)
    : submissionsDir(subDir), executablesDir(exeDir), logsDir(logDir) {
    
    // Ensure directories exist
    Utils::createDirectory(submissionsDir);
    Utils::createDirectory(executablesDir);
    Utils::createDirectory(logsDir);
}

// ==================== Main Judging Method ====================

Submission JudgeEngine::judge(const string& username, const string& problemId,
                              const string& sourceCode, const Problem& problem) {
    
    // Generate submission ID
    string submissionId = generateSubmissionId(username, problemId);
    
    // Create submission object
    Submission submission(submissionId, username, problemId, sourceCode, "C++");
    
    Utils::logInfo("Starting judgment for submission: " + submissionId);
    
    // Step 1: Save source code
    string sourceFile = saveSourceCode(username, problemId, sourceCode);
    if (sourceFile.empty()) {
        submission.setFinalVerdict(COMPILATION_ERROR);
        TestResult result(0, COMPILATION_ERROR, 0.0, 0, "Failed to save source code");
        submission.addTestResult(result);
        return submission;
    }
    
    // Step 2: Compile the code
    string executableFile = executablesDir + username + "_" + problemId + ".exe";
    CompilationResult compResult = compiler.compile(sourceFile, executableFile);
    
    if (!compResult.success) {
        submission.setFinalVerdict(COMPILATION_ERROR);
        TestResult result(0, COMPILATION_ERROR, 0.0, 0, compResult.errorMessage);
        submission.addTestResult(result);
        logCompilationError(username, problemId, compResult.errorMessage);
        return submission;
    }
    
    Utils::logInfo("Compilation successful for: " + submissionId);
    
    // Step 3: Run test cases
    vector<TestCase> testCases = problem.getTestCases();
    int passedTests = 0;
    bool allPassed = true;
    
    for (size_t i = 0; i < testCases.size(); i++) {
        const TestCase& tc = testCases[i];
        
        Utils::logInfo("Running test case " + to_string(i + 1) + "/" + to_string(testCases.size()));
        
        // Create output file path
        string userOutputFile = executablesDir + username + "_" + problemId + 
                               "_output" + to_string(i + 1) + ".txt";
        
        // Execute the code
        ExecutionResult execResult = executeCode(executableFile, tc.inputFile, 
                                                userOutputFile, tc.timeLimit);
        
        TestResult testResult;
        testResult.testCaseNumber = i + 1;
        testResult.executionTime = execResult.executionTime;
        testResult.memoryUsed = execResult.memoryUsed;
        
        // Check for runtime errors
        if (!execResult.success || execResult.exitCode != 0) {
            testResult.verdict = RUNTIME_ERROR;
            testResult.errorMessage = execResult.errorOutput;
            allPassed = false;
        }
        // Check for time limit exceeded
        else if (execResult.executionTime > tc.timeLimit) {
            testResult.verdict = TIME_LIMIT_EXCEEDED;
            testResult.errorMessage = "Execution time: " + 
                                     Utils::formatDuration(execResult.executionTime) +
                                     " (Limit: " + to_string(tc.timeLimit) + " ms)";
            allPassed = false;
        }
        // Validate output
        else if (!validateOutputFile(userOutputFile, tc.outputFile)) {
            testResult.verdict = WRONG_ANSWER;
            testResult.errorMessage = "Output does not match expected output";
            allPassed = false;
        }
        // Test passed
        else {
            testResult.verdict = ACCEPTED;
            passedTests++;
        }
        
        submission.addTestResult(testResult);
        
        // Clean up output file
        Utils::deleteFile(userOutputFile);
    }
    
    // Step 4: Determine final verdict and score
    if (allPassed) {
        submission.setFinalVerdict(ACCEPTED);
        submission.setScore(problem.getPoints());
    } else if (passedTests > 0) {
        submission.setFinalVerdict(WRONG_ANSWER);
        // Partial scoring
        submission.setScore((problem.getPoints() * passedTests) / testCases.size());
    } else {
        submission.setFinalVerdict(WRONG_ANSWER);
        submission.setScore(0);
    }
    
    submission.calculateTotalExecutionTime();
    
    // Clean up executable
    compiler.removeExecutable(executableFile);
    
    Utils::logInfo("Judgment complete for: " + submissionId + 
                   " | Verdict: " + submission.getVerdictString() +
                   " | Score: " + to_string(submission.getScore()));
    
    return submission;
}

// ==================== Execution Methods ====================

ExecutionResult JudgeEngine::executeCode(const string& executablePath,
                                        const string& inputFile,
                                        const string& outputFile,
                                        int timeLimit) {
    ExecutionResult result;
    
    // Build execution command with I/O redirection
    stringstream cmd;
    
#ifdef _WIN32
    // Windows: Use cmd /c to properly handle I/O redirection
    cmd << "cmd /c \"\"" << executablePath << "\" < \"" << inputFile 
        << "\" > \"" << outputFile << "\" 2>&1\"";
#else
    cmd << "./" << executablePath << " < " << inputFile 
        << " > " << outputFile << " 2>&1";
#endif
    
    // Measure execution time
    auto start = high_resolution_clock::now();
    
    // Execute the command
    int exitCode = system(cmd.str().c_str());
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    
    result.executionTime = duration.count();
    result.exitCode = exitCode;
    
    // Check if output file was created
    if (Utils::fileExists(outputFile)) {
        result.success = true;
        result.output = Utils::readFile(outputFile);
    } else {
        result.success = false;
        result.errorOutput = "No output generated";
    }
    
    return result;
}

// ==================== Validation Methods ====================

bool JudgeEngine::validateOutput(const string& userOutput, const string& expectedOutput) {
    vector<string> userLines = splitLines(userOutput);
    vector<string> expectedLines = splitLines(expectedOutput);
    
    if (userLines.size() != expectedLines.size()) {
        return false;
    }
    
    for (size_t i = 0; i < userLines.size(); i++) {
        if (!compareLines(userLines[i], expectedLines[i])) {
            return false;
        }
    }
    
    return true;
}

bool JudgeEngine::validateOutputFile(const string& userOutputFile, 
                                     const string& expectedOutputFile) {
    string userOutput = Utils::readFile(userOutputFile);
    string expectedOutput = Utils::readFile(expectedOutputFile);
    
    return validateOutput(userOutput, expectedOutput);
}

// ==================== Utility Methods ====================

string JudgeEngine::saveSourceCode(const string& username, const string& problemId, 
                                   const string& code) {
    string filename = submissionsDir + username + "_" + problemId + ".cpp";
    
    if (Utils::writeFile(filename, code)) {
        Utils::logInfo("Source code saved: " + filename);
        return filename;
    }
    
    Utils::logError("Failed to save source code: " + filename);
    return "";
}

void JudgeEngine::logExecution(const string& message) {
    Utils::log(message, logsDir + "execution_logs.txt");
}

void JudgeEngine::logCompilationError(const string& username, const string& problemId, 
                                     const string& error) {
    string message = "User: " + username + " | Problem: " + problemId + "\n" + error;
    Utils::log(message, logsDir + "compilation_errors.txt");
}

// ==================== Private Helper Methods ====================

string JudgeEngine::generateSubmissionId(const string& username, const string& problemId) {
    return username + "_" + problemId + "_" + Utils::generateRandomId(8);
}

string JudgeEngine::trim(const string& str) {
    return Utils::trim(str);
}

vector<string> JudgeEngine::splitLines(const string& text) {
    vector<string> lines;
    stringstream ss(text);
    string line;
    
    while (getline(ss, line)) {
        line = trim(line);
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    
    return lines;
}

bool JudgeEngine::compareLines(const string& line1, const string& line2) {
    string trimmed1 = trim(line1);
    string trimmed2 = trim(line2);
    
    return trimmed1 == trimmed2;
}

double JudgeEngine::measureExecutionTime(const string& executablePath, const string& inputFile) {
    auto start = high_resolution_clock::now();
    
    string cmd = executablePath + " < " + inputFile + " > /dev/null 2>&1";
    system(cmd.c_str());
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    
    return duration.count();
}
