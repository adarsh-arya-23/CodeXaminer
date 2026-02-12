#include "../include/User.h"
#include "../include/Utils.h"
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

// ==================== Constructors ====================

User::User() 
    : username(""), password(""), email(""), 
      totalSubmissions(0), acceptedSubmissions(0), score(0),
      registrationDate(time(nullptr)) {}

User::User(string uname, string pwd, string mail)
    : username(uname), password(pwd), email(mail),
      totalSubmissions(0), acceptedSubmissions(0), score(0),
      registrationDate(time(nullptr)) {}

// ==================== Getters ====================

string User::getUsername() const {
    return username;
}

string User::getPassword() const {
    return password;
}

string User::getEmail() const {
    return email;
}

int User::getTotalSubmissions() const {
    return totalSubmissions;
}

int User::getAcceptedSubmissions() const {
    return acceptedSubmissions;
}

int User::getScore() const {
    return score;
}

time_t User::getRegistrationDate() const {
    return registrationDate;
}

vector<string> User::getSubmissionHistory() const {
    return submissionHistory;
}

// ==================== Setters ====================

void User::setPassword(string pwd) {
    password = pwd;
}

void User::setEmail(string mail) {
    email = mail;
}

void User::incrementTotalSubmissions() {
    totalSubmissions++;
}

void User::incrementAcceptedSubmissions() {
    acceptedSubmissions++;
}

void User::addScore(int points) {
    score += points;
}

void User::addSubmission(string submissionId) {
    submissionHistory.push_back(submissionId);
}

// ==================== Utility Methods ====================

double User::getAcceptanceRate() const {
    if (totalSubmissions == 0) return 0.0;
    return (double)acceptedSubmissions / totalSubmissions * 100.0;
}

string User::getRegistrationDateString() const {
    return Utils::formatTime(registrationDate);
}

void User::displayProfile() const {
    cout << "\n";
    Utils::printSeparator('=', 50);
    cout << "  USER PROFILE" << endl;
    Utils::printSeparator('=', 50);
    cout << "  Username:            " << username << endl;
    cout << "  Email:               " << email << endl;
    cout << "  Registration Date:   " << getRegistrationDateString() << endl;
    cout << "  Total Submissions:   " << totalSubmissions << endl;
    cout << "  Accepted:            " << acceptedSubmissions << endl;
    cout << "  Acceptance Rate:     " << fixed << setprecision(2) 
         << getAcceptanceRate() << "%" << endl;
    cout << "  Total Score:         " << score << endl;
    Utils::printSeparator('=', 50);
    cout << endl;
}

// ==================== Serialization ====================

string User::serialize() const {
    stringstream ss;
    ss << username << "|"
       << password << "|"
       << email << "|"
       << totalSubmissions << "|"
       << acceptedSubmissions << "|"
       << score << "|"
       << registrationDate << "|"
       << Utils::join(submissionHistory, ",");
    return ss.str();
}

User User::deserialize(const string& data) {
    vector<string> parts = Utils::split(data, '|');
    
    if (parts.size() < 7) {
        return User(); // Return empty user if data is invalid
    }
    
    User user;
    user.username = parts[0];
    user.password = parts[1];
    user.email = parts[2];
    user.totalSubmissions = stoi(parts[3]);
    user.acceptedSubmissions = stoi(parts[4]);
    user.score = stoi(parts[5]);
    user.registrationDate = stoll(parts[6]);
    
    if (parts.size() > 7 && !parts[7].empty()) {
        user.submissionHistory = Utils::split(parts[7], ',');
    }
    
    return user;
}
