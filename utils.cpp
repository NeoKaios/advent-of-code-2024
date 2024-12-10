#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

using u64 = unsigned long long;
typedef pair<int, int> coord;

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
