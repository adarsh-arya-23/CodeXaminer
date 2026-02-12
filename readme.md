# 🚀 Mini Online Judge System

A fully backend-driven **Online Judge Simulation System** built in C++, designed to replicate the core working of platforms like **LeetCode**, **CodeChef**, and **HackerRank**.

## 📌 Features

✅ **Code Submission** - Submit C++ code for problems  
✅ **Automated Compilation** - Uses g++ compiler with error detection  
✅ **Multi-Test Case Execution** - Runs code against multiple test cases  
✅ **Output Validation** - Compares output with expected results  
✅ **Performance Tracking** - Measures execution time  
✅ **Leaderboard System** - Priority queue-based ranking  
✅ **Persistent Storage** - File-based data persistence  
✅ **User Management** - Registration, login, and profiles  

## 🛠️ Technologies Used

- **Language**: C++ (C++17)
- **Compiler**: g++ (MinGW on Windows, GCC on Linux/Mac)
- **STL Components**: map, vector, queue, priority_queue, fstream, chrono

## 📊 Data Structures

| Feature | Data Structure | Justification |
|---------|---------------|---------------|
| User Database | `map<string, User>` | Fast O(log n) lookup |
| Test Cases | `queue<TestCase>` | FIFO execution |
| Leaderboard | `priority_queue` | Efficient ranking |
| Submissions | `vector<Submission>` | Dynamic storage |

## 🏗️ System Architecture

```
User (CLI)
   ↓
Code Submission Module
   ↓
Compilation Module (g++)
   ↓
Execution Engine
   ↓
Test Case Validator
   ↓
Performance Analyzer
   ↓
Submission Storage
   ↓
Leaderboard System
```

## 📁 Project Structure

```
Mini-Online-Judge/
│
├── README.md
├── main.cpp                 # Main application
├── Makefile                 # Build configuration
├── build.ps1                # Windows build script
│
├── include/                 # Header files
│   ├── User.h
│   ├── Problem.h
│   ├── Submission.h
│   ├── JudgeEngine.h
│   ├── Compiler.h
│   ├── Leaderboard.h
│   └── Utils.h
│
├── src/                     # Implementation files
│   ├── User.cpp
│   ├── Problem.cpp
│   ├── Submission.cpp
│   ├── JudgeEngine.cpp
│   ├── Compiler.cpp
│   ├── Leaderboard.cpp
│   └── Utils.cpp
│
├── problems/                # Problem definitions
│   ├── problem1/
│   │   ├── statement.txt
│   │   ├── input1.txt
│   │   ├── output1.txt
│   │   ├── input2.txt
│   │   └── output2.txt
│   ├── problem2/
│   └── problem3/
│
├── submissions/             # User code submissions
├── executables/             # Compiled binaries
├── logs/                    # System logs
└── data/                    # Persistent data
    ├── users.txt
    └── leaderboard.txt
```

## 🚀 Installation & Setup

### Prerequisites

- **C++ Compiler** (g++)
  - **Windows**: Install [MinGW-w64](https://www.mingw-w64.org/)
  - **Linux**: `sudo apt install g++`
  - **Mac**: `brew install gcc`

### Verify Installation

```bash
g++ --version
```

### Build the Project

#### Option 1: Using PowerShell Script (Windows)

```powershell
.\build.ps1
```

#### Option 2: Using Makefile (Linux/Mac/Windows with Make)

```bash
make
```

#### Option 3: Manual Compilation

```bash
# Create directories
mkdir obj data logs submissions executables

# Compile all source files
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/Utils.cpp -o obj/Utils.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/User.cpp -o obj/User.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/Problem.cpp -o obj/Problem.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/Submission.cpp -o obj/Submission.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/Compiler.cpp -o obj/Compiler.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/JudgeEngine.cpp -o obj/JudgeEngine.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/Leaderboard.cpp -o obj/Leaderboard.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c main.cpp -o obj/main.o

# Link
g++ -std=c++17 -O2 obj/*.o -o judge
```

## 🎮 Usage

### Run the Program

```bash
# Windows
.\judge.exe

# Linux/Mac
./judge
```

### Main Menu

```
===== MINI ONLINE JUDGE SYSTEM =====

1. Register
2. Login
3. View Problems
4. View Leaderboard
5. Exit

Enter your choice:
```

### User Workflow

1. **Register** - Create a new account
2. **Login** - Access your account
3. **View Problems** - Browse available problems
4. **Submit Code** - Submit your solution
5. **View Results** - See test case results and verdict
6. **Check Leaderboard** - View your ranking

## 📝 Sample Code Submission

### Problem: Two Sum

```cpp
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int main() {
    int n, target;
    cin >> n;
    
    vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }
    cin >> target;
    
    unordered_map<int, int> map;
    for (int i = 0; i < n; i++) {
        int complement = target - nums[i];
        if (map.find(complement) != map.end()) {
            cout << map[complement] << " " << i << endl;
            return 0;
        }
        map[nums[i]] = i;
    }
    
    return 0;
}
```

## 🏆 Verdict Types

- **AC** (Accepted) - All test cases passed ✅
- **WA** (Wrong Answer) - Output doesn't match expected ❌
- **TLE** (Time Limit Exceeded) - Execution too slow ⏱️
- **RE** (Runtime Error) - Program crashed 💥
- **CE** (Compilation Error) - Code doesn't compile 🔧

## 📈 Scoring System

- **Easy Problems**: 100 points
- **Medium Problems**: 200 points
- **Hard Problems**: 300 points (future)
- **Partial Credit**: Based on passed test cases

## 🎯 Available Problems

### Problem 1: Two Sum (Easy - 100 points)
Find two numbers in an array that add up to a target sum.

### Problem 2: Fibonacci Number (Easy - 100 points)
Calculate the nth Fibonacci number.

### Problem 3: Array Sorting (Medium - 200 points)
Sort an array of integers in ascending order.

## 🔧 Troubleshooting

### g++ not found
- **Windows**: Add MinGW bin directory to PATH
- **Linux**: `sudo apt install build-essential`
- **Mac**: Install Xcode Command Line Tools

### Compilation Errors
- Ensure C++17 support: `g++ -std=c++17`
- Check all header files are present
- Verify file paths are correct

### Permission Denied (Linux/Mac)
```bash
chmod +x judge
./judge
```

## 📚 Learning Outcomes

- ✅ Advanced C++ programming
- ✅ STL data structures (map, vector, queue, priority_queue)
- ✅ File I/O operations
- ✅ System programming (process execution)
- ✅ Performance measurement
- ✅ Modular design patterns
- ✅ Cross-platform development

## 🚀 Future Enhancements

- [ ] Support for multiple languages (Python, Java)
- [ ] Memory limit enforcement
- [ ] Plagiarism detection
- [ ] Contest mode
- [ ] Web interface
- [ ] Database integration (SQLite)
- [ ] Sandboxed execution
- [ ] Real-time leaderboard updates

## 📄 License

This project is created for educational purposes.

## 👨‍💻 Author

**PEP Training Project**  
Mini Online Judge System  
Built with ❤️ using C++

---

**Happy Coding! 🚀**