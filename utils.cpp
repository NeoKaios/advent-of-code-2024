#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>
#include "print.cpp"

using namespace std;

coord operator+(coord a, coord b) {
    return {a.first + b.first, a.second + b.second};
}

void operator+=(coord& a, coord b) {
    a.first += b.first;
    a.second += b.second;
}

coord operator-(coord a) {
    return {-a.first, -a.second};
}


coord operator-(coord a, coord b) {
    return {a.first - b.first, a.second - b.second};
}

bool operator<=(coord a, coord b) {
    return a.first <= b.first && a.second <= b.second;
}

bool operator>=(coord a, coord b) {
    return a.first >= b.first && a.second >= b.second;
}

bool operator==(coord a, coord b) {
    return a.first == b.first && a.second == b.second;
}

dir char_to_dir(char dir) {
    switch(dir) {
        case '<':
            return Left;
        case 'v':
            return Down;
        case '>':
            return Right;
        case '^':
            return Up;
    }
    cout << "ERROR when converting char_to_dir " << dir << endl;
    return Right;
}

coord inline dir_to_coord_offset(dir dir) {
    switch(dir) {
        case Left:
            return {0,-1};
        case Down:
            return {1,0};
        case Up:
            return {-1,0};
        case Right:
        default:
            return {0,1};
    }
}

coord operator+(coord a, dir b) {
    return a + dir_to_coord_offset(b);
}

dir coord_offset_to_dir(coord offset) {
    if(offset == (coord){0,-1}) return Left;
    if(offset == (coord){0,1}) return Right;
    if(offset == (coord){1,0}) return Down;
    if(offset == (coord){-1,0}) return Up;
    cout << "ERROR, given value is not a coord_offset: " << offset << endl;
    return Right;
}


coord char_to_coord_offset(char dir) {
    return dir_to_coord_offset(char_to_dir(dir));
}

typedef pair<coord,dir> coord_dir;

template <typename T>

void get_num_list(fstream& file, string separator, vector<T>& vec) {
    char ch;
    T a = 0;
    int i = 0;
    bool in_separation=false;
    while (file.get(ch)) {
        if(in_separation && i < separator.size() && ch == separator[i]) {
            ++i;
            continue;
        } else {
            in_separation=false;
            i=0;
        }
        if(ch == '\n') {
            vec.push_back(a);
            return;
        } else if(ch == separator[0]) {
            vec.push_back(a);
            i++;
            in_separation = true;
            a = 0;
        } else if(ch>='0' && ch<='9'){
            a = 10*a + (ch - 48);
        } else {
            cout << "Unknown char " << ch << endl;
        }
    }
}

void get_string_list(fstream& file, string separator, vector<string>& vec) {
    char ch;
    string a = "";
    int i = 0;
    bool in_separation=false;
    while (file.get(ch)) {
        if(in_separation && i < separator.size() && ch == separator[i]) {
            ++i;
            continue;
        } else {
            in_separation=false;
            i=0;
        }
        if(ch == '\n') {
            vec.push_back(a);
            return;
        } else if(ch == separator[0]) {
            vec.push_back(a);
            i++;
            in_separation = true;
            a = "";
        } else {
            a += ch;
        }
    }
}

int parse_str(fstream& file, string str) {
    char ch;
    int i = 0;
    while(file.get(ch)) {
        if(str[i] == ch) ++i;
        else {
            cout << "Error: Unexpected char in parse_str (" << ch << ") expecting " << str[i] << endl;
            return 1;
        }
        if(i == str.size()) return 0;
    }
    return 1;
}


int parse_u64_list(fstream& file, string _str, vector<u64>& out) {
    char ch;
    bool parsing_int = false;
    int i = 0;
    u64 a = 0;
    string str = _str + '\n';
    while(file.get(ch)) {
        if(str[i] == '%') {
            if(ch>='0' && ch<='9') {
                a = 10*a + (ch - 48);
                continue;
            }
            else {
                out.push_back(a);
                a=0;
                ++i;
            }
        }
        if(str[i] == ch) ++i;
        else {
            cout << "Error: Unexpected char in line_parsing (" << ch << ") expecting " << str[i] << endl;
            return 1;
        }
        if(i == str.size()) return 0;
    }
    return 1;
}

int parse_int_list(fstream& file, string _str, vector<int>& out) {
    char ch;
    bool parsing_int = false;
    int i = 0;
    int a = 0;
    string str = _str + '\n';
    bool neg_flag = false;
    while(file.get(ch)) {
        if(str[i] == '%') {
            if(ch>='0' && ch<='9') {
                a = 10*a + (ch - 48);
                continue;
            } else if (a == 0 && ch == '-') {
                neg_flag = true;
                continue;
            }
            else {
                out.push_back(neg_flag ? -a : a);
                neg_flag = false;
                a=0;
                ++i;
            }
        }
        if(str[i] == ch) ++i;
        else {
            cout << "Error: Unexpected char in line_parsing (" << ch << ") expecting " << str[i] << endl;
            return 1;
        }
        if(i == str.size()) return 0;
    }
    return 1;
}

class grid : public vector<vector<u64>> {
    public:
    friend std::ostream & operator<<(std::ostream &os, const grid& grid) {
        for (const auto v : grid)
            os << v << endl;
        return os;
    }

    u64& operator()(const coord coord) {
        return at(coord.first).at(coord.second);
    }

};

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
            if(ch == '\n') {
                if(back().size() == 0) break;
                push_back({});
            } else back().push_back(ch-48);
        }
        if(back().size() == 0) pop_back();
        col = this[0].size();
        row = size();
    }
};

class u64_grid: public vector<vector<u64>> {
    public:
    int row;
    int col;

    friend std::ostream & operator<<(std::ostream &os, const u64_grid& grid) {
        for (const vector<u64> v : grid)
            os << v << endl;
        return os;
    }

    void update_row_col() {
        col = at(0).size();
        row = size();
    }

    u64_grid(u64 init, int row, int col) {
        for(int i=0; i<row; ++i) {
            push_back({});
            for(int j=0; j<col; ++j) {
                back().push_back(init);
            }
        }
        update_row_col();
    }

    u64& operator()(const coord coord) {
        return at(coord.first).at(coord.second);
    }

};

class char_vec : public vector<char> {
    friend std::ostream& operator<<(std::ostream& os, const char_vec& vec)
    {
        for (const char i : vec) os << i;
        return os;
    }
    public:
    char_vec() {}

    char_vec(fstream& file, bool stop_at_newline) {
        char ch;
        while(file.get(ch)) {
            if(ch == '\n') {
                if(stop_at_newline) break;
                else continue;
            }
            push_back(ch);
        }
    }
};

class char_grid : public vector<char_vec> {
    char out_of_range = '\x00';
    public:
    int row;
    int col;

    friend std::ostream & operator<<(std::ostream &os, const char_grid& grid) {
        os << "Grid of size " << grid.row << 'x' << grid.col << ':' << endl;
        for (const char_vec v : grid)
            os << v << endl;
        return os;
    }

    char& operator()(const coord coord) {
        return at(coord.first).at(coord.second);
    }

    char& safe_get(const coord coo) {
        if(coo >= (coord){0,0} && coo <= (coord){row-1, col-1})
            return at(coo.first).at(coo.second);
        else return out_of_range;
    }


    char_vec& operator[](const size_t i) {
        return at(i);
    }

    void update_row_col() {
        col = at(0).size();
        row = size();
    }

    coord find(char ch) {
        for(int i=0;i<row;++i) {
            for(int j=0;j<col;++j) {
                if(at(i).at(j) == ch)
                    return (coord){i,j};
            }
        }
        return {};
    }

    char_grid() {}

    char_grid(fstream& file) {
        char ch;
        push_back({});
        while (file.get(ch)) {
            if(ch == '\n') {
                if(back().size() == 0) break;
                push_back({});
            } else back().push_back(ch);
        }
        if(back().size() == 0) pop_back();
        update_row_col();
    }

    char_grid(char init, int row, int col) {
        for(int i=0; i<row; ++i) {
            push_back({});
            for(int j=0; j<col; ++j) {
                back().push_back(init);
            }
        }
        update_row_col();
    }
};

std::ostream& operator<<(std::ostream& os, const dir& dir) {
    switch(dir) {
        case Left:
            os << "Left";
            break;
        case Down:
            os << "Down";
            break;
        case Right:
            os << "Right";
            break;
        case Up:
        default:
            os << "Up";
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const coord_dir& cd) {
    return os << cd.first << ' ' << cd.second << endl;
}
