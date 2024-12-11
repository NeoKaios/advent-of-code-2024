#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

using u64 = unsigned long long;
typedef pair<int, int> coord;

void get_u64_list(fstream& file, char separator, vector<u64>& vec) {
    char ch;
    u64 a = 0;
    while (file.get(ch)) {
        if(ch == '\n') {
            vec.push_back(a);
            return;
        } else if(ch == separator) {
            vec.push_back(a);
            a = 0;
        } else if(ch>='0' && ch<='9'){
            a = 10*a + (ch - 48);
        } else {
            cout << "Unknown char " << ch << endl;
        }
    }
}
void get_int_list(fstream& file, char separator, vector<int>& vec) {
    char ch;
    int a = 0;
    while (file.get(ch)) {
        if(ch == '\n') {
            vec.push_back(a);
            return;
        } else if(ch == separator) {
            vec.push_back(a);
            a = 0;
        } else if(ch>='0' && ch<='9'){
            a = 10*a + (ch - 48);
        } else {
            cout << "Unknown char " << ch << endl;
        }
    }
}

class int_vec : public vector<char> {
    friend std::ostream& operator<<(std::ostream& os, const int_vec& vec)
    {
        for (const int i : vec) os << i;
        return os;
    }
};

class int_grid : public vector<int_vec> {
    public:
    int row;
    int col;

    friend std::ostream & operator<<(std::ostream &os, const int_grid& grid) {
        for (const int_vec v : grid)
            os << v << endl;
        return os;
    }

    int_grid(fstream& file) {
        char ch;
        push_back({});
        while (file.get(ch)) {
            if(ch == '\n') push_back({});
            else back().push_back(ch-48);
        }
        if(back().size() == 0) pop_back();
        col = this[0].size();
        row = size();
    }
};

class char_vec : public vector<char> {
    friend std::ostream& operator<<(std::ostream& os, const char_vec& vec)
    {
        for (const char i : vec) os << i;
        return os;
    }
};

class char_grid : public vector<char_vec> {
    public:
    int row;
    int col;

    friend std::ostream & operator<<(std::ostream &os, const char_grid& grid) {
        for (const char_vec v : grid)
            os << v << endl;
        return os;
    }

    char_grid(fstream& file) {
        char ch;
        push_back({});
        while (file.get(ch)) {
            if(ch == '\n') push_back({});
            else back().push_back(ch);
        }
        if(back().size() == 0) pop_back();
        col = this[0].size();
        row = size();
    }
};

std::ostream& operator<<(std::ostream& os, const vector<int>& vec)
{
    for (const int i : vec) os << i << ' ';
    return os;
}

std::ostream& operator<<(std::ostream& os, const vector<u64>& vec)
{
    for (const u64 i : vec) os << i << ' ';
    return os;
}
