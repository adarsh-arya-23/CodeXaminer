# 🚀 Quick Start Guide - Mini Online Judge System

## ⚡ Get Started in 3 Steps

### Step 1: Build the Project

```powershell
# Windows
.\build.ps1

# Linux/Mac
make
```

### Step 2: Run the Application

```bash
.\judge.exe    # Windows
./judge        # Linux/Mac
```

### Step 3: Start Using

1. **Register** a new account
2. **Login** with your credentials
3. **View Problems** and choose one
4. **Submit Code** and get instant feedback!

---

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

**How to Submit:**
1. Login to your account
2. Select "Submit Code" from menu
3. Enter problem ID: `P001`
4. Paste your code
5. Type `END` on a new line
6. View your results!

---

## 🎯 Menu Navigation

### Main Menu
```
1. Register    - Create new account
2. Login       - Access your account
3. View Problems - Browse all problems
4. View Leaderboard - See top users
5. Exit        - Quit application
```

### User Menu (After Login)
```
1. View Problems - Browse problems
2. View Problem Details - See full problem statement
3. Submit Code - Submit your solution
4. View My Profile - Check your stats
5. View Leaderboard - See rankings
6. Logout - Return to main menu
```

---

## 📊 Understanding Verdicts

| Verdict | Meaning | What to Do |
|---------|---------|------------|
| **AC** ✅ | Accepted | Perfect! All test cases passed |
| **WA** ❌ | Wrong Answer | Check your logic |
| **TLE** ⏱️ | Time Limit Exceeded | Optimize your code |
| **RE** 💥 | Runtime Error | Fix crashes/errors |
| **CE** 🔧 | Compilation Error | Fix syntax errors |

---

## 🏆 Scoring System

- **Easy Problems:** 100 points
- **Medium Problems:** 200 points
- **Hard Problems:** 300 points (future)
- **Partial Credit:** Based on passed test cases

---

## 💡 Tips for Success

1. **Read Problem Carefully** - Understand input/output format
2. **Test Locally First** - Verify your solution works
3. **Handle Edge Cases** - Consider boundary conditions
4. **Optimize** - Watch for time complexity
5. **Practice Regularly** - Improve your ranking!

---

## 🔧 Troubleshooting

### g++ not found?
- **Windows:** Install MinGW and add to PATH
- **Linux:** `sudo apt install g++`
- **Mac:** Install Xcode Command Line Tools

### Build fails?
- Ensure C++17 support: `g++ --version`
- Check all files are present
- Try manual compilation (see README.md)

### Permission denied?
```bash
chmod +x judge    # Linux/Mac
./judge
```

---

## 📚 Available Problems

### P001: Two Sum (Easy - 100 pts)
Find two numbers in array that add up to target.

### P002: Fibonacci Number (Easy - 100 pts)
Calculate the nth Fibonacci number.

### P003: Array Sorting (Medium - 200 pts)
Sort an array in ascending order.

---

## 🎓 Learning Path

1. **Start with Easy Problems** (P001, P002)
2. **Master Basic Algorithms** (sorting, searching)
3. **Move to Medium Problems** (P003)
4. **Optimize Solutions** (improve execution time)
5. **Climb the Leaderboard!** 🚀

---

## 📞 Need Help?

- Check **README.md** for detailed documentation
- Review **PROJECT_SUMMARY.md** for project overview
- See **WORK_TRACKER.md** for development details

---

**Happy Coding! 🎉**

*Start your competitive programming journey today!*
