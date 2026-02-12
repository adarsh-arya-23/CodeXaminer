#include "../include/Problem.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// ==================== Constructors ====================

Problem::Problem()
    : problemId(""), title(""), statement(""), difficulty("Medium"),
      points(100), totalSubmissions(0), acceptedSubmissions(0) {}

Problem::Problem(string id, string ttl, string diff, int pts)
    : problemId(id), title(ttl), statement(""), difficulty(diff),
      points(pts), totalSubmissions(0), acceptedSubmissions(0) {}

// ==================== Getters ====================

string Problem::getProblemId() const {
    return problemId;
}

string Problem::getTitle() const {
    return title;
}

string Problem::getStatement() const {
    return statement;
}

string Problem::getDifficulty() const {
    return difficulty;
}

int Problem::getPoints() const {
    return points;
}

vector<TestCase> Problem::getTestCases() const {
    return testCases;
}

int Problem::getTotalSubmissions() const {
    return totalSubmissions;
}

int Problem::getAcceptedSubmissions() const {
    return acceptedSubmissions;
}

// ==================== Setters ====================

void Problem::setStatement(string stmt) {
    statement = stmt;
}

void Problem::addTestCase(const TestCase& tc) {
    testCases.push_back(tc);
}

void Problem::incrementTotalSubmissions() {
    totalSubmissions++;
}

void Problem::incrementAcceptedSubmissions() {
    acceptedSubmissions++;
}

// ==================== Utility Methods ====================

double Problem::getAcceptanceRate() const {
    if (totalSubmissions == 0) return 0.0;
    return (double)acceptedSubmissions / totalSubmissions * 100.0;
}

void Problem::displayProblem() const {
    cout << "\n";
    Utils::printSeparator('=', 70);
    cout << "  PROBLEM: " << title << endl;
    Utils::printSeparator('=', 70);
    cout << "  Problem ID:    " << problemId << endl;
    cout << "  Difficulty:    ";
    
    // Color-coded difficulty
    if (difficulty == "Easy") {
        cout << "\033[1;32m" << difficulty << "\033[0m" << endl;
    } else if (difficulty == "Medium") {
        cout << "\033[1;33m" << difficulty << "\033[0m" << endl;
    } else {
        cout << "\033[1;31m" << difficulty << "\033[0m" << endl;
    }
    
    cout << "  Points:        " << points << endl;
    cout << "  Test Cases:    " << testCases.size() << endl;
    cout << "  Submissions:   " << totalSubmissions << endl;
    cout << "  Acceptance:    " << fixed << setprecision(2) 
         << getAcceptanceRate() << "%" << endl;
    
    Utils::printSeparator('-', 70);
    cout << "\n  PROBLEM STATEMENT:\n" << endl;
    cout << statement << endl;
    Utils::printSeparator('=', 70);
    cout << endl;
}

bool Problem::loadFromDirectory(const string& dirPath) {
    // Load problem statement
    string statementPath = dirPath + "/statement.txt";
    if (Utils::fileExists(statementPath)) {
        statement = Utils::readFile(statementPath);
    } else {
        Utils::logError("Statement file not found: " + statementPath);
        return false;
    }
    
    // Load test cases
    int testNum = 1;
    while (true) {
        string inputFile = dirPath + "/input" + to_string(testNum) + ".txt";
        string outputFile = dirPath + "/output" + to_string(testNum) + ".txt";
        
        if (!Utils::fileExists(inputFile) || !Utils::fileExists(outputFile)) {
            break;
        }
        
        TestCase tc(inputFile, outputFile);
        testCases.push_back(tc);
        testNum++;
    }
    
    if (testCases.empty()) {
        Utils::logError("No test cases found in: " + dirPath);
        return false;
    }
    
    Utils::logInfo("Loaded problem " + problemId + " with " + 
                   to_string(testCases.size()) + " test cases");
    return true;
}

// ==================== Serialization ====================

string Problem::serialize() const {
    stringstream ss;
    ss << problemId << "|"
       << title << "|"
       << difficulty << "|"
       << points << "|"
       << totalSubmissions << "|"
       << acceptedSubmissions << "|"
       << testCases.size();
    
    // Serialize test cases
    for (const auto& tc : testCases) {
        ss << "|" << tc.inputFile
           << "," << tc.outputFile
           << "," << tc.timeLimit
           << "," << tc.memoryLimit;
    }
    
    return ss.str();
}

Problem Problem::deserialize(const string& data) {
    vector<string> parts = Utils::split(data, '|');
    
    if (parts.size() < 7) {
        return Problem(); // Return empty problem if data is invalid
    }
    
    Problem prob;
    prob.problemId = parts[0];
    prob.title = parts[1];
    prob.difficulty = parts[2];
    prob.points = stoi(parts[3]);
    prob.totalSubmissions = stoi(parts[4]);
    prob.acceptedSubmissions = stoi(parts[5]);
    
    int numTestCases = stoi(parts[6]);
    
    // Deserialize test cases
    for (int i = 0; i < numTestCases && (7 + i) < parts.size(); i++) {
        vector<string> tcParts = Utils::split(parts[7 + i], ',');
        if (tcParts.size() >= 4) {
            TestCase tc;
            tc.inputFile = tcParts[0];
            tc.outputFile = tcParts[1];
            tc.timeLimit = stoi(tcParts[2]);
            tc.memoryLimit = stoi(tcParts[3]);
            prob.testCases.push_back(tc);
        }
    }
    
    return prob;
}
