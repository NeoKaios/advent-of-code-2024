#include "../utils.cpp"
#include <array>
#include <cstring>
#include <map>

using namespace std;

bool starts_with(const char* str, string& pattern) {
    for(auto ch : pattern) {
        if(ch != *str) return false;
        str++;
    }
    return true;
}

bool ends_with(const char* str, string& pattern, int len) {
    int i = len-pattern.size();
    for(auto ch : pattern) {
        if(ch != str[i]) return false;
        i++;
    }
    return true;
}


bool match_aux(const char* str, map<char, vector<string>>& patterns, vector<signed char>& history) {
    char first = str[0];
    auto len = strlen(str);
    if(history[len] >= 0) return history[len];
    history[len] = 0;
    for(string pat : patterns[first]) {
        if(starts_with(str, pat)) {
            if(match_aux(&str[pat.size()], patterns, history)) {
                history[len] = 1;
                break;
            }
        }
    }
    return history[len];
}

bool match(const char* str, map<char, vector<string>>& patterns) {
    char first = str[0];
    size_t len = strlen(str);
    vector<signed char> history(len+1, (signed char) -1);
    history[0] = 1;
    return match_aux(str, patterns, history);
}

i64 match_aux2(const char* str, map<char, vector<string>>& patterns, vector<i64>& history) {
    char first = str[0];
    auto len = strlen(str);
    if(history[len] >= 0) return history[len];
    history[len] = 0;
    for(string pat : patterns[first]) {
        if(starts_with(str, pat))
             history[len] += match_aux2(&str[pat.size()], patterns, history);
    }
    return history[len];
}

i64 match2(const char* str, map<char, vector<string>>& patterns) {
    char first = str[0];
    size_t len = strlen(str);
    vector<i64> history(len+1, -1);
    history[0] = 1;
    return match_aux2(str, patterns, history);
}


void first_part(fstream& file) {
    char ch;
    u64 res = 0;
    u64 res2 = 0;
    vector<string> patterns_vec = {};
    get_string_list(file, ", ", patterns_vec);
    file.get(ch);

    map<char, vector<string>> patterns = {};
    for(auto s : patterns_vec) {
        if(patterns.count(s[0])>0) {
            patterns[s[0]].push_back(s);
        } else {
            patterns[s[0]] = {s};
        }
    }
    for(array<char, 100> s; file.getline(&s[0], 100, '\n');) {
        int len = 0;
        for(auto c : s) {
            if(c=='\x00') break;
            ++len;
        }
        bool possible = false;
        string str(s.data());
        for(auto pat : patterns_vec) {
            if(ends_with(&s[0], pat, len)) {
                possible = true;
                break;
            }
        }
        if(!possible) continue;
        if(match(&s[0], patterns)) ++res;
        res2 += match2(&s[0], patterns);
    }
    cout << "Answer is: " << res << endl;
    cout << "Answer is: " << res2 << endl;
}

void second_part(fstream& file) {
}

int main (int argc, char *argv[])
{
    fstream file;
    if(argc > 1 && !strcmp(argv[1], "-e"))
        file.open("example.txt", ios::in);
    else if(argc > 1 && !strcmp(argv[1], "-e2"))
        file.open("example2.txt", ios::in);
    else
        file.open("input.txt", ios::in);
    if (!file) {
        cout << "No such file";
        return 1;
    }
    first_part(file);
    file.clear();
    file.seekg(0, ios::beg);
    second_part(file);
    file.close();
    return 0;
}

