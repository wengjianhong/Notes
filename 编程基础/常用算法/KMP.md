

[Leetcode-28](https://leetcode-cn.com/problems/implement-strstr/)

```cpp
class Solution {
public:
    void initNext(const string& str, vector<int>& next) {
        next[0] = 0;
        int pre = 0;
        for (int i = 1; i < str.size(); i++) {
            while (pre > 0 && str[i] != str[pre]) {
                pre = next[pre - 1];    // 后退一步
            }
            if (str[i] == str[pre]){
                ++pre;                  // 前进一步
            }
            next[i] = pre;
        }
    }

    int KMP(const string& str, const string& substr, vector<int>& next) {
        int i, j;
        for (i = 0, j = 0; i < str.size() && j < substr.size(); i++) {
            while (j > 0 && str[i] != substr[j]) {
                j = next[j - 1];      // 后退一步
            }
            if (str[i] == substr[j]) {
                j++;                  // 前进一步
            }
        }
        if (j >= substr.size())
            return i - j;

        return -1;
    }

    int strStr(string haystack, string needle) {
        if (needle == "") return 0;

        vector<int> next(needle.size());
        initNext(needle, next);
        return KMP(haystack, needle, next);
    }
};
```

