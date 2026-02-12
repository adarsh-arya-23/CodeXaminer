#include "../include/Leaderboard.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// ==================== Constructor ====================

Leaderboard::Leaderboard(const string& file) : dataFile(file) {
    loadFromFile();
}

// ==================== Update Methods ====================

void Leaderboard::updateUserScore(const string& username, int scoreChange) {
    if (userEntries.find(username) != userEntries.end()) {
        userEntries[username].score += scoreChange;
    } else {
        LeaderboardEntry entry(username, scoreChange, 0, 0, 0.0);
        userEntries[username] = entry;
    }
    saveToFile();
}

void Leaderboard::updateUserStats(const string& username, int acceptedChange, 
                                  int totalChange, double executionTime) {
    if (userEntries.find(username) != userEntries.end()) {
        LeaderboardEntry& entry = userEntries[username];
        entry.acceptedSubmissions += acceptedChange;
        entry.totalSubmissions += totalChange;
        
        // Update average execution time
        if (entry.totalSubmissions > 0) {
            entry.averageExecutionTime = 
                ((entry.averageExecutionTime * (entry.totalSubmissions - totalChange)) + executionTime) 
                / entry.totalSubmissions;
        }
    } else {
        LeaderboardEntry entry(username, 0, acceptedChange, totalChange, executionTime);
        userEntries[username] = entry;
    }
    saveToFile();
}

void Leaderboard::addUser(const User& user) {
    LeaderboardEntry entry(user.getUsername(), user.getScore(), 
                          user.getAcceptedSubmissions(), 
                          user.getTotalSubmissions(), 0.0);
    userEntries[user.getUsername()] = entry;
    saveToFile();
}

// ==================== Query Methods ====================

vector<LeaderboardEntry> Leaderboard::getTopN(int n) {
    priority_queue<LeaderboardEntry> pq = buildPriorityQueue();
    vector<LeaderboardEntry> topEntries;
    
    int count = 0;
    while (!pq.empty() && count < n) {
        topEntries.push_back(pq.top());
        pq.pop();
        count++;
    }
    
    return topEntries;
}

vector<LeaderboardEntry> Leaderboard::getAllRankings() {
    priority_queue<LeaderboardEntry> pq = buildPriorityQueue();
    vector<LeaderboardEntry> allEntries;
    
    while (!pq.empty()) {
        allEntries.push_back(pq.top());
        pq.pop();
    }
    
    return allEntries;
}

LeaderboardEntry Leaderboard::getUserEntry(const string& username) {
    if (userEntries.find(username) != userEntries.end()) {
        return userEntries[username];
    }
    return LeaderboardEntry(); // Return empty entry if not found
}

int Leaderboard::getUserRank(const string& username) {
    vector<LeaderboardEntry> rankings = getAllRankings();
    
    for (size_t i = 0; i < rankings.size(); i++) {
        if (rankings[i].username == username) {
            return i + 1;
        }
    }
    
    return -1; // User not found
}

// ==================== Display Methods ====================

void Leaderboard::displayLeaderboard(int topN) {
    vector<LeaderboardEntry> topEntries = getTopN(topN);
    
    cout << "\n";
    Utils::printSeparator('=', 90);
    cout << "  LEADERBOARD - TOP " << topN << endl;
    Utils::printSeparator('=', 90);
    
    // Header
    cout << left << setw(6) << "Rank"
         << setw(20) << "Username"
         << setw(12) << "Score"
         << setw(12) << "Accepted"
         << setw(12) << "Total"
         << setw(15) << "Acceptance %"
         << setw(13) << "Avg Time" << endl;
    Utils::printSeparator('-', 90);
    
    // Display entries
    for (size_t i = 0; i < topEntries.size(); i++) {
        cout << formatRankDisplay(i + 1, topEntries[i]) << endl;
    }
    
    Utils::printSeparator('=', 90);
    cout << endl;
}

void Leaderboard::displayUserRanking(const string& username) {
    int rank = getUserRank(username);
    
    if (rank == -1) {
        Utils::printError("User not found in leaderboard");
        return;
    }
    
    LeaderboardEntry entry = getUserEntry(username);
    
    cout << "\n";
    Utils::printSeparator('=', 60);
    cout << "  YOUR RANKING" << endl;
    Utils::printSeparator('=', 60);
    cout << "  Rank:              #" << rank << endl;
    cout << "  Username:          " << entry.username << endl;
    cout << "  Score:             " << entry.score << " points" << endl;
    cout << "  Accepted:          " << entry.acceptedSubmissions << endl;
    cout << "  Total Submissions: " << entry.totalSubmissions << endl;
    
    if (entry.totalSubmissions > 0) {
        double acceptanceRate = (double)entry.acceptedSubmissions / entry.totalSubmissions * 100.0;
        cout << "  Acceptance Rate:   " << fixed << setprecision(2) 
             << acceptanceRate << "%" << endl;
    }
    
    if (entry.averageExecutionTime > 0) {
        cout << "  Avg Exec Time:     " 
             << Utils::formatDuration(entry.averageExecutionTime) << endl;
    }
    
    Utils::printSeparator('=', 60);
    cout << endl;
}

// ==================== Persistence ====================

bool Leaderboard::saveToFile() {
    ofstream file(dataFile);
    if (!file.is_open()) {
        Utils::logError("Failed to save leaderboard to: " + dataFile);
        return false;
    }
    
    for (const auto& pair : userEntries) {
        const LeaderboardEntry& entry = pair.second;
        file << entry.username << "|"
             << entry.score << "|"
             << entry.acceptedSubmissions << "|"
             << entry.totalSubmissions << "|"
             << entry.averageExecutionTime << endl;
    }
    
    file.close();
    return true;
}

bool Leaderboard::loadFromFile() {
    if (!Utils::fileExists(dataFile)) {
        Utils::logInfo("Leaderboard file not found, creating new: " + dataFile);
        return true;
    }
    
    ifstream file(dataFile);
    if (!file.is_open()) {
        Utils::logError("Failed to load leaderboard from: " + dataFile);
        return false;
    }
    
    userEntries.clear();
    string line;
    
    while (getline(file, line)) {
        vector<string> parts = Utils::split(line, '|');
        if (parts.size() >= 5) {
            LeaderboardEntry entry;
            entry.username = parts[0];
            entry.score = stoi(parts[1]);
            entry.acceptedSubmissions = stoi(parts[2]);
            entry.totalSubmissions = stoi(parts[3]);
            entry.averageExecutionTime = stod(parts[4]);
            
            userEntries[entry.username] = entry;
        }
    }
    
    file.close();
    Utils::logInfo("Loaded " + to_string(userEntries.size()) + " entries from leaderboard");
    return true;
}

void Leaderboard::clearLeaderboard() {
    userEntries.clear();
    saveToFile();
}

// ==================== Private Helper Methods ====================

priority_queue<LeaderboardEntry> Leaderboard::buildPriorityQueue() {
    priority_queue<LeaderboardEntry> pq;
    
    for (const auto& pair : userEntries) {
        pq.push(pair.second);
    }
    
    return pq;
}

string Leaderboard::formatRankDisplay(int rank, const LeaderboardEntry& entry) {
    stringstream ss;
    
    // Rank with medal emoji for top 3
    ss << left << setw(6);
    if (rank == 1) {
        ss << "🥇 1";
    } else if (rank == 2) {
        ss << "🥈 2";
    } else if (rank == 3) {
        ss << "🥉 3";
    } else {
        ss << rank;
    }
    
    ss << setw(20) << entry.username
       << setw(12) << entry.score
       << setw(12) << entry.acceptedSubmissions
       << setw(12) << entry.totalSubmissions;
    
    // Acceptance rate
    if (entry.totalSubmissions > 0) {
        double acceptanceRate = (double)entry.acceptedSubmissions / entry.totalSubmissions * 100.0;
        ss << setw(15) << (to_string((int)acceptanceRate) + "%");
    } else {
        ss << setw(15) << "N/A";
    }
    
    // Average time
    if (entry.averageExecutionTime > 0) {
        ss << setw(13) << Utils::formatDuration(entry.averageExecutionTime);
    } else {
        ss << setw(13) << "N/A";
    }
    
    return ss.str();
}
