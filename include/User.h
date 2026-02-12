#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <ctime>

using namespace std;

/**
 * User class - Represents a registered user in the system
 */
class User {
private:
    string username;
    string password;
    string email;
    int totalSubmissions;
    int acceptedSubmissions;
    int score;
    time_t registrationDate;
    vector<string> submissionHistory;

public:
    // Constructors
    User();
    User(string uname, string pwd, string mail);

    // Getters
    string getUsername() const;
    string getPassword() const;
    string getEmail() const;
    int getTotalSubmissions() const;
    int getAcceptedSubmissions() const;
    int getScore() const;
    time_t getRegistrationDate() const;
    vector<string> getSubmissionHistory() const;

    // Setters
    void setPassword(string pwd);
    void setEmail(string mail);
    void incrementTotalSubmissions();
    void incrementAcceptedSubmissions();
    void addScore(int points);
    void addSubmission(string submissionId);

    // Utility methods
    double getAcceptanceRate() const;
    string getRegistrationDateString() const;
    void displayProfile() const;

    // Serialization for file storage
    string serialize() const;
    static User deserialize(const string& data);
};

#endif // USER_H
