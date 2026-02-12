# 📝 Sample Solutions for Testing

## Problem 1: Two Sum (P001)

### ✅ Correct Solution

```cpp
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }
    
    int target;
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

**Expected Output for Test Case 1:**
```
0 1
```

---

## Problem 2: Fibonacci Number (P002)

### ✅ Correct Solution

```cpp
#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    if (n == 0) {
        cout << 0 << endl;
        return 0;
    }
    if (n == 1) {
        cout << 1 << endl;
        return 0;
    }
    
    long long a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        long long temp = a + b;
        a = b;
        b = temp;
    }
    
    cout << b << endl;
    return 0;
}
```

**Expected Outputs:**
- Test 1 (n=6): `8`
- Test 2 (n=10): `55`
- Test 3 (n=0): `0`

---

## Problem 3: Array Sorting (P003)

### ✅ Correct Solution

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    sort(arr.begin(), arr.end());
    
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << " ";
        cout << arr[i];
    }
    cout << endl;
    
    return 0;
}
```

**Expected Outputs:**
- Test 1: `1 2 5 8 9`
- Test 2: `-5 -3 0 2 7 10`
- Test 3: `42`

---

## 🧪 How to Test

1. **Run the application:**
   ```bash
   .\judge.exe
   ```

2. **Register/Login** with your account

3. **Submit Code:**
   - Select "Submit Code"
   - Enter Problem ID (e.g., `P001`)
   - Paste the solution code
   - Type `END` on a new line

4. **Check Results:**
   - You should see "Accepted" (AC) verdict
   - Score: 100 points (for Easy problems)
   - All test cases should pass ✅

---

## 🐛 Common Issues Fixed

### Issue 1: Trailing Newlines
**Problem:** Output files had trailing newlines causing validation failures  
**Solution:** ✅ Fixed - All output files now have no trailing newlines

### Issue 2: Output Format
**Problem:** Make sure your output matches exactly  
**Solution:** Use `endl` or `\n` after output, the judge will handle it

### Issue 3: Input Reading
**Problem:** Not reading all input correctly  
**Solution:** Follow the exact input format in problem statement

---

## ✅ What Was Fixed

1. **Problem 1 Output Files:**
   - output1.txt: `0 1` (no trailing newline)
   - output2.txt: `1 2` (no trailing newline)
   - output3.txt: `0 2` (no trailing newline)

2. **Problem 2 Output Files:**
   - output1.txt: `8` (no trailing newline)
   - output2.txt: `55` (no trailing newline)
   - output3.txt: `0` (no trailing newline)

3. **Problem 3 Output Files:**
   - output1.txt: `1 2 5 8 9` (no trailing newline)
   - output2.txt: `-5 -3 0 2 7 10` (no trailing newline)
   - output3.txt: `42` (no trailing newline)

---

## 🎯 Test Now!

The test case validation issue has been **FIXED**! ✅

Try submitting the sample solutions above and you should get:
- ✅ **Verdict:** Accepted (AC)
- ✅ **Score:** 100 points
- ✅ **All test cases passed:** 3/3

Happy coding! 🚀
