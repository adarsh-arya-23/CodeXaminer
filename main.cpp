#include "include/User.h"
#include "include/Problem.h"
#include "include/Submission.h"
#include "include/JudgeEngine.h"
#include "include/Compiler.h"
#include "include/Leaderboard.h"
#include "include/Utils.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

// Global data structures
map<string, User> users;
map<string, Problem> problems;
Leaderboard leaderboard("data/leaderboard.txt");
JudgeEngine judgeEngine;

// Current logged-in user
string currentUser = "";

// Function declarations
void loadUsers();
void saveUsers();
void loadProblems();
void registerUser();
void loginUser();
void logoutUser();
void submitCode();
void viewSubmissionHistory();
void viewLeaderboard();
void viewProblems();
void viewProblemDetail();
void viewProfile();
void mainMenu();
void userMenu();

// ==================== Main Function ====================

int main() {
    // Check if g++ is available
    if (!Utils::isCommandAvailable("g++")) {
        Utils::printError("g++ compiler not found!");
        Utils::printError("Please install g++ (MinGW on Windows, GCC on Linux/Mac)");
        return 1;
    }
    
    Utils::logInfo("=== Mini Online Judge System Started ===");
    
    // Load data
    loadUsers();
    loadProblems();
    
    // Start main menu
    mainMenu();
    
    Utils::logInfo("=== Mini Online Judge System Stopped ===");
    return 0;
}

// ==================== Data Loading/Saving ====================

void loadUsers() {
    string userFile = "data/users.txt";
    
    if (!Utils::fileExists(userFile)) {
        Utils::logInfo("User file not found, creating new");
        return;
    }
    
    ifstream file(userFile);
    if (!file.is_open()) {
        Utils::logError("Failed to load users");
        return;
    }
    
    string line;
    while (getline(file, line)) {
        User user = User::deserialize(line);
        if (!user.getUsername().empty()) {
            users[user.getUsername()] = user;
        }
    }
    
    file.close();
    Utils::logInfo("Loaded " + to_string(users.size()) + " users");
}

void saveUsers() {
    string userFile = "data/users.txt";
    ofstream file(userFile);
    
    if (!file.is_open()) {
        Utils::logError("Failed to save users");
        return;
    }
    
    for (const auto& pair : users) {
        file << pair.second.serialize() << endl;
    }
    
    file.close();
}

void loadProblems() {
    // Load problems from directories
    vector<string> problemDirs = {"problem1", "problem2", "problem3"};
    
    for (const string& dir : problemDirs) {
        string problemPath = "problems/" + dir;
        
        if (Utils::directoryExists(problemPath)) {
            Problem prob;
            
            // Extract problem info from directory name
            if (dir == "problem1") {
                prob = Problem("P001", "Two Sum", "Easy", 100);
            } else if (dir == "problem2") {
                prob = Problem("P002", "Fibonacci Number", "Easy", 100);
            } else if (dir == "problem3") {
                prob = Problem("P003", "Array Sorting", "Medium", 200);
            }
            
            if (prob.loadFromDirectory(problemPath)) {
                problems[prob.getProblemId()] = prob;
            }
        }
    }
    
    Utils::logInfo("Loaded " + to_string(problems.size()) + " problems");
}

// ==================== User Management ====================

void registerUser() {
    Utils::clearScreen();
    Utils::printHeader("USER REGISTRATION");
    
    string username = Utils::getInput("Enter username (3-20 chars, alphanumeric): ");
    
    if (!Utils::isValidUsername(username)) {
        Utils::printError("Invalid username format!");
        Utils::getInput("Press Enter to continue...");
        return;
    }
    
    if (users.find(username) != users.end()) {
        Utils::printError("Username already exists!");
        Utils::getInput("Press Enter to continue...");
        return;
    }
    
    string password = Utils::getPassword("Enter password (min 6 chars): ");
    
    if (!Utils::isValidPassword(password)) {
        Utils::printError("Password must be at least 6 characters!");
        Utils::getInput("Press Enter to continue...");
        return;
    }
    
    string email = Utils::getInput("Enter email: ");
    
    if (!Utils::isValidEmail(email)) {
        Utils::printError("Invalid email format!");
        Utils::getInput("Press Enter to continue...");
        return;
    }
    
    // Create user with hashed password
    User newUser(username, Utils::hashPassword(password), email);
    users[username] = newUser;
    leaderboard.addUser(newUser);
    saveUsers();
    
    Utils::printSuccess("Registration successful! You can now login.");
    Utils::logInfo("New user registered: " + username);
    Utils::getInput("Press Enter to continue...");
}

void loginUser() {
    Utils::clearScreen();
    Utils::printHeader("USER LOGIN");
    
    string username = Utils::getInput("Username: ");
    string password = Utils::getPassword("Password: ");
    
    if (users.find(username) == users.end()) {
        Utils::printError("User not found!");
        Utils::getInput("Press Enter to continue...");
        return;
    }
    
    if (!Utils::verifyPassword(password, users[username].getPassword())) {
        Utils::printError("Incorrect password!");
        Utils::getInput("Press Enter to continue...");
        return;
    }
    
    currentUser = username;
    Utils::printSuccess("Login successful! Welcome, " + username + "!");
    Utils::logInfo("User logged in: " + username);
    Utils::getInput("Press Enter to continue...");
}

void logoutUser() {
    Utils::logInfo("User logged out: " + currentUser);
    currentUser = "";
    Utils::printSuccess("Logged out successfully!");
    Utils::getInput("Press Enter to continue...");
}

// ==================== Problem Management ====================

void viewProblems() {
    Utils::clearScreen();
    Utils::printHeader("AVAILABLE PROBLEMS");
    
    if (problems.empty()) {
        Utils::printWarning("No problems available yet!");
        Utils::getInput("Press Enter to continue...");
        return;
    }
    
    cout << left << setw(10) << "ID"
         << setw(30) << "Title"
         << setw(12) << "Difficulty"
         << setw(10) << "Points"
         << setw(12) << "Acceptance" << endl;
    Utils::printSeparator('-', 74);
    
    for (const auto& pair : problems) {
        const Problem& prob = pair.second;
        cout << left << setw(10) << prob.getProblemId()
             << setw(30) << prob.getTitle()
             << setw(12) << prob.getDifficulty()
             << setw(10) << prob.getPoints()
             << setw(12) << (to_string((int)prob.getAcceptanceRate()) + "%") << endl;
    }
    
    Utils::printSeparator('=', 74);
    Utils::getInput("\nPress Enter to continue...");
}

void viewProblemDetail() {
    Utils::clearScreen();
    Utils::printHeader("PROBLEM DETAILS");
    
    string problemId = Utils::getInput("Enter Problem ID: ");
    problemId = Utils::toUpper(problemId);
    
    if (problems.find(problemId) == problems.end()) {
        Utils::printError("Problem not found!");
        Utils::getInput("Press Enter to continue...");
        return;
    }
    
    problems[problemId].displayProblem();
    Utils::getInput("Press Enter to continue...");
}

// ==================== Code Submission ====================

void submitCode() {
    Utils::clearScreen();
    Utils::printHeader("SUBMIT CODE");
    
    // Show available problems
    cout << "Available Problems:" << endl;
    for (const auto& pair : problems) {
        cout << "  " << pair.second.getProblemId() << " - " 
             << pair.second.getTitle() << endl;
    }
    cout << endl;
    
    string problemId = Utils::getInput("Enter Problem ID: ");
    problemId = Utils::toUpper(problemId);
    
    if (problems.find(problemId) == problems.end()) {
        Utils::printError("Problem not found!");
        Utils::getInput("Press Enter to continue...");
        return;
    }
    
    cout << "\nEnter your C++ code (type 'END' on a new line to finish):" << endl;
    Utils::printSeparator('-', 50);
    
    string code;
    string line;
    while (true) {
        getline(cin, line);
        if (line == "END") break;
        code += line + "\n";
    }
    
    if (code.empty()) {
        Utils::printError("No code provided!");
        Utils::getInput("Press Enter to continue...");
        return;
    }
    
    Utils::printInfo("Submitting code for judgment...");
    
    // Judge the submission
    Submission submission = judgeEngine.judge(currentUser, problemId, code, problems[problemId]);
    
    // Update problem statistics
    problems[problemId].incrementTotalSubmissions();
    if (submission.getFinalVerdict() == ACCEPTED) {
        problems[problemId].incrementAcceptedSubmissions();
    }
    
    // Update user statistics
    users[currentUser].incrementTotalSubmissions();
    users[currentUser].addSubmission(submission.getSubmissionId());
    
    if (submission.getFinalVerdict() == ACCEPTED) {
        users[currentUser].incrementAcceptedSubmissions();
        users[currentUser].addScore(submission.getScore());
    }
    
    // Update leaderboard
    leaderboard.updateUserScore(currentUser, submission.getScore());
    leaderboard.updateUserStats(currentUser, 
                               submission.getFinalVerdict() == ACCEPTED ? 1 : 0,
                               1, submission.getTotalExecutionTime());
    
    saveUsers();
    
    // Display results
    submission.displayResults();
    Utils::getInput("Press Enter to continue...");
}

// ==================== View Functions ====================

void viewProfile() {
    Utils::clearScreen();
    users[currentUser].displayProfile();
    leaderboard.displayUserRanking(currentUser);
    Utils::getInput("Press Enter to continue...");
}

void viewLeaderboard() {
    Utils::clearScreen();
    leaderboard.displayLeaderboard(10);
    Utils::getInput("Press Enter to continue...");
}

// ==================== Menu Functions ====================

void mainMenu() {
    while (true) {
        Utils::clearScreen();
        Utils::printHeader("MINI ONLINE JUDGE SYSTEM");
        
        cout << "\n  1. Register" << endl;
        cout << "  2. Login" << endl;
        cout << "  3. View Problems" << endl;
        cout << "  4. View Leaderboard" << endl;
        cout << "  5. Exit" << endl;
        
        int choice = Utils::getIntInput("\nEnter your choice: ", 1, 5);
        
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                if (!currentUser.empty()) {
                    userMenu();
                }
                break;
            case 3:
                viewProblems();
                break;
            case 4:
                viewLeaderboard();
                break;
            case 5:
                Utils::printInfo("Thank you for using Mini Online Judge!");
                return;
        }
    }
}

void userMenu() {
    while (!currentUser.empty()) {
        Utils::clearScreen();
        Utils::printHeader("WELCOME, " + Utils::toUpper(currentUser) + "!");
        
        cout << "\n  1. View Problems" << endl;
        cout << "  2. View Problem Details" << endl;
        cout << "  3. Submit Code" << endl;
        cout << "  4. View My Profile" << endl;
        cout << "  5. View Leaderboard" << endl;
        cout << "  6. Logout" << endl;
        
        int choice = Utils::getIntInput("\nEnter your choice: ", 1, 6);
        
        switch (choice) {
            case 1:
                viewProblems();
                break;
            case 2:
                viewProblemDetail();
                break;
            case 3:
                submitCode();
                break;
            case 4:
                viewProfile();
                break;
            case 5:
                viewLeaderboard();
                break;
            case 6:
                logoutUser();
                return;
        }
    }
}
