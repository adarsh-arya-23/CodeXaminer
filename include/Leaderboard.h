#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "User.h"
#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace std;

/**
 * LeaderboardEntry structure - Represents a user's ranking entry
 */
struct LeaderboardEntry {
    string username;
    int score;
    int acceptedSubmissions;
    int totalSubmissions;
    double averageExecutionTime;
    
    LeaderboardEntry(string user = "", int sc = 0, int acc = 0, int tot = 0, double avgTime = 0.0)
        : username(user), score(sc), acceptedSubmissions(acc), 
          totalSubmissions(tot), averageExecutionTime(avgTime) {}

    // Comparison operator for priority queue (max heap based on score)
    bool operator<(const LeaderboardEntry& other) const {
        if (score != other.score)
            return score < other.score;  // Higher score is better
        if (acceptedSubmissions != other.acceptedSubmissions)
            return acceptedSubmissions < other.acceptedSubmissions;
        return averageExecutionTime > other.averageExecutionTime;  // Lower time is better
    }
};

/**
 * Leaderboard class - Manages user rankings
 */
class Leaderboard {
private:
    map<string, LeaderboardEntry> userEntries;
    string dataFile;

public:
    // Constructor
    Leaderboard(const string& file = "data/leaderboard.txt");

    // Update methods
    void updateUserScore(const string& username, int scoreChange);
    void updateUserStats(const string& username, int acceptedChange, int totalChange, double executionTime);
    void addUser(const User& user);

    // Query methods
    vector<LeaderboardEntry> getTopN(int n);
    vector<LeaderboardEntry> getAllRankings();
    LeaderboardEntry getUserEntry(const string& username);
    int getUserRank(const string& username);

    // Display methods
    void displayLeaderboard(int topN = 10);
    void displayUserRanking(const string& username);

    // Persistence
    bool saveToFile();
    bool loadFromFile();
    void clearLeaderboard();

private:
    // Helper methods
    priority_queue<LeaderboardEntry> buildPriorityQueue();
    string formatRankDisplay(int rank, const LeaderboardEntry& entry);
};

#endif // LEADERBOARD_H
