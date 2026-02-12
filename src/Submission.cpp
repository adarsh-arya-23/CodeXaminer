#include "../include/Submission.h"
#include "../include/Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

// ==================== Helper Function ====================

string verdictToString(Verdict v) {
    switch (v) {
        case PENDING: return "Pending";
        case ACCEPTED: return "Accepted";
        case WRONG_ANSWER: return "Wrong Answer";
        case TIME_LIMIT_EXCEEDED: return "Time Limit Exceeded";
        case RUNTIME_ERROR: return "Runtime Error";
        case COMPILATION_ERROR: return "Compilation Error";
        case MEMORY_LIMIT_EXCEEDED: return "Memory Limit Exceeded";
        default: return "Unknown";
    }
}

// ==================== Constructors ====================

Submission::Submission()
    : submissionId(""), username(""), problemId(""), sourceCode(""),
      language("C++"), submissionTime(time(nullptr)), finalVerdict(PENDING),
      score(0), totalExecutionTime(0.0) {}

Submission::Submission(string id, string user, string prob, string code, string lang)
    : submissionId(id), username(user), problemId(prob), sourceCode(code),
      language(lang), submissionTime(time(nullptr)), finalVerdict(PENDING),
      score(0), totalExecutionTime(0.0) {}

// ==================== Getters ====================

string Submission::getSubmissionId() const {
    return submissionId;
}

string Submission::getUsername() const {
    return username;
}

string Submission::getProblemId() const {
    return problemId;
}

string Submission::getSourceCode() const {
    return sourceCode;
}

string Submission::getLanguage() const {
    return language;
}

time_t Submission::getSubmissionTime() const {
    return submissionTime;
}

Verdict Submission::getFinalVerdict() const {
    return finalVerdict;
}

vector<TestResult> Submission::getTestResults() const {
    return testResults;
}

int Submission::getScore() const {
    return score;
}

double Submission::getTotalExecutionTime() const {
    return totalExecutionTime;
}

// ==================== Setters ====================

void Submission::setFinalVerdict(Verdict v) {
    finalVerdict = v;
}

void Submission::addTestResult(const TestResult& result) {
    testResults.push_back(result);
}

void Submission::setScore(int s) {
    score = s;
}

void Submission::calculateTotalExecutionTime() {
    totalExecutionTime = 0.0;
    for (const auto& result : testResults) {
        totalExecutionTime += result.executionTime;
    }
}

// ==================== Utility Methods ====================

string Submission::getVerdictString() const {
    return verdictToString(finalVerdict);
}

string Submission::getSubmissionTimeString() const {
    return Utils::formatTime(submissionTime);
}

void Submission::displayResults() const {
    cout << "\n";
    Utils::printSeparator('=', 70);
    cout << "  SUBMISSION RESULTS" << endl;
    Utils::printSeparator('=', 70);
    cout << "  Submission ID:  " << submissionId << endl;
    cout << "  Username:       " << username << endl;
    cout << "  Problem ID:     " << problemId << endl;
    cout << "  Language:       " << language << endl;
    cout << "  Submitted:      " << getSubmissionTimeString() << endl;
    cout << "  Final Verdict:  ";
    
    // Color-coded verdict display
    if (finalVerdict == ACCEPTED) {
        cout << "\033[1;32m" << getVerdictString() << "\033[0m" << endl;
    } else if (finalVerdict == COMPILATION_ERROR) {
        cout << "\033[1;31m" << getVerdictString() << "\033[0m" << endl;
    } else {
        cout << "\033[1;33m" << getVerdictString() << "\033[0m" << endl;
    }
    
    cout << "  Score:          " << score << " points" << endl;
    cout << "  Total Time:     " << Utils::formatDuration(totalExecutionTime) << endl;
    
    Utils::printSeparator('-', 70);
    cout << "  Test Cases: " << getPassedTestCases() << "/" << getTotalTestCases() << " passed" << endl;
    Utils::printSeparator('-', 70);
    
    // Display individual test case results
    for (const auto& result : testResults) {
        cout << "  Test #" << result.testCaseNumber << ": ";
        
        if (result.verdict == ACCEPTED) {
            cout << "\033[1;32m✓ PASS\033[0m";
        } else {
            cout << "\033[1;31m✗ " << verdictToString(result.verdict) << "\033[0m";
        }
        
        cout << " (" << Utils::formatDuration(result.executionTime) << ")";
        
        if (!result.errorMessage.empty()) {
            cout << "\n      Error: " << result.errorMessage;
        }
        cout << endl;
    }
    
    Utils::printSeparator('=', 70);
    cout << endl;
}

int Submission::getPassedTestCases() const {
    int passed = 0;
    for (const auto& result : testResults) {
        if (result.verdict == ACCEPTED) {
            passed++;
        }
    }
    return passed;
}

int Submission::getTotalTestCases() const {
    return testResults.size();
}

// ==================== Serialization ====================

string Submission::serialize() const {
    stringstream ss;
    ss << submissionId << "|"
       << username << "|"
       << problemId << "|"
       << language << "|"
       << submissionTime << "|"
       << static_cast<int>(finalVerdict) << "|"
       << score << "|"
       << totalExecutionTime << "|"
       << testResults.size();
    
    // Serialize test results
    for (const auto& result : testResults) {
        ss << "|" << result.testCaseNumber
           << "," << static_cast<int>(result.verdict)
           << "," << result.executionTime
           << "," << result.memoryUsed
           << "," << result.errorMessage;
    }
    
    return ss.str();
}

Submission Submission::deserialize(const string& data) {
    vector<string> parts = Utils::split(data, '|');
    
    if (parts.size() < 9) {
        return Submission(); // Return empty submission if data is invalid
    }
    
    Submission sub;
    sub.submissionId = parts[0];
    sub.username = parts[1];
    sub.problemId = parts[2];
    sub.language = parts[3];
    sub.submissionTime = stoll(parts[4]);
    sub.finalVerdict = static_cast<Verdict>(stoi(parts[5]));
    sub.score = stoi(parts[6]);
    sub.totalExecutionTime = stod(parts[7]);
    
    int numResults = stoi(parts[8]);
    
    // Deserialize test results
    for (int i = 0; i < numResults && (9 + i) < parts.size(); i++) {
        vector<string> resultParts = Utils::split(parts[9 + i], ',');
        if (resultParts.size() >= 4) {
            TestResult result;
            result.testCaseNumber = stoi(resultParts[0]);
            result.verdict = static_cast<Verdict>(stoi(resultParts[1]));
            result.executionTime = stod(resultParts[2]);
            result.memoryUsed = stoi(resultParts[3]);
            if (resultParts.size() > 4) {
                result.errorMessage = resultParts[4];
            }
            sub.testResults.push_back(result);
        }
    }
    
    return sub;
}
