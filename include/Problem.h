#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include <vector>
#include <map>

using namespace std;

/**
 * TestCase structure - Represents a single test case for a problem
 */
struct TestCase {
    string inputFile;
    string outputFile;
    int timeLimit;  // in milliseconds
    int memoryLimit; // in MB

    TestCase(string in = "", string out = "", int time = 1000, int mem = 256)
        : inputFile(in), outputFile(out), timeLimit(time), memoryLimit(mem) {}
};

/**
 * Problem class - Represents a coding problem
 */
class Problem {
private:
    string problemId;
    string title;
    string statement;
    string difficulty;  // Easy, Medium, Hard
    int points;
    vector<TestCase> testCases;
    int totalSubmissions;
    int acceptedSubmissions;

public:
    // Constructors
    Problem();
    Problem(string id, string title, string diff, int pts);

    // Getters
    string getProblemId() const;
    string getTitle() const;
    string getStatement() const;
    string getDifficulty() const;
    int getPoints() const;
    vector<TestCase> getTestCases() const;
    int getTotalSubmissions() const;
    int getAcceptedSubmissions() const;

    // Setters
    void setStatement(string stmt);
    void addTestCase(const TestCase& tc);
    void incrementTotalSubmissions();
    void incrementAcceptedSubmissions();

    // Utility methods
    double getAcceptanceRate() const;
    void displayProblem() const;
    bool loadFromDirectory(const string& dirPath);

    // Serialization
    string serialize() const;
    static Problem deserialize(const string& data);
};

#endif // PROBLEM_H
