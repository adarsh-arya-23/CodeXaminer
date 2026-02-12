#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <string>
#include <vector>
#include <ctime>

using namespace std;

/**
 * Verdict enum - Possible outcomes of a submission
 */
enum Verdict {
    PENDING,
    ACCEPTED,
    WRONG_ANSWER,
    TIME_LIMIT_EXCEEDED,
    RUNTIME_ERROR,
    COMPILATION_ERROR,
    MEMORY_LIMIT_EXCEEDED
};

/**
 * TestResult structure - Result of a single test case
 */
struct TestResult {
    int testCaseNumber;
    Verdict verdict;
    double executionTime;  // in milliseconds
    int memoryUsed;        // in KB
    string errorMessage;

    TestResult(int num = 0, Verdict v = PENDING, double time = 0.0, int mem = 0, string err = "")
        : testCaseNumber(num), verdict(v), executionTime(time), memoryUsed(mem), errorMessage(err) {}
};

/**
 * Submission class - Represents a code submission
 */
class Submission {
private:
    string submissionId;
    string username;
    string problemId;
    string sourceCode;
    string language;
    time_t submissionTime;
    Verdict finalVerdict;
    vector<TestResult> testResults;
    int score;
    double totalExecutionTime;

public:
    // Constructors
    Submission();
    Submission(string id, string user, string prob, string code, string lang);

    // Getters
    string getSubmissionId() const;
    string getUsername() const;
    string getProblemId() const;
    string getSourceCode() const;
    string getLanguage() const;
    time_t getSubmissionTime() const;
    Verdict getFinalVerdict() const;
    vector<TestResult> getTestResults() const;
    int getScore() const;
    double getTotalExecutionTime() const;

    // Setters
    void setFinalVerdict(Verdict v);
    void addTestResult(const TestResult& result);
    void setScore(int s);
    void calculateTotalExecutionTime();

    // Utility methods
    string getVerdictString() const;
    string getSubmissionTimeString() const;
    void displayResults() const;
    int getPassedTestCases() const;
    int getTotalTestCases() const;

    // Serialization
    string serialize() const;
    static Submission deserialize(const string& data);
};

// Helper function to convert verdict to string
string verdictToString(Verdict v);

#endif // SUBMISSION_H
