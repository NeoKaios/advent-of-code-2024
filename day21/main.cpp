#include "../utils.cpp"
#include <map>

using namespace std;
vector<vector<vector<string>>> numpad_possible {};
vector<vector<vector<string>>> arrowpad_possible {};
vector<vector<string>> arrowpad_shortest {};
vector<vector<string>> numpad_shortest {};
char possible[] = { '^', ':', '<', '>', 'v'};
map<char, size_t> arrow_map = { {'^', 0}, {':', 1}, {'<', 2}, {'>',3}, {'v',4}};

size_t to_idx(char ch) {
    return arrow_map[ch];
}

void fill_cache_compute() {
    fstream file;
    file.open("num_keypad.txt", ios::in);
    char_grid keypad(file);
    for(int i=0; i<11; ++i) {
        numpad_possible.push_back({});
        for(int j=0; j<11; ++j) {
            numpad_possible.back().push_back({});
            coord start = keypad.find(i+48);
            coord end = keypad.find(j+48);
            string lr="";
            string ud="";
            for(int k=0; k<abs(start.first-end.first); ++k) {
                ud+= (start.first-end.first > 0) ? '^' : 'v';
            }
            for(int k=0; k<abs(start.second-end.second); ++k) {
                lr+= (start.second-end.second > 0) ? '<' : '>';
            }
            if(ud == "" || lr == "")
                numpad_possible[i][j].push_back(ud+lr+':');
            else {
                if(start.second != 0 || end.first != 3)
                    numpad_possible[i][j].push_back(ud+lr+':');
                if(start.first != 3 || end.second != 0)
                    numpad_possible[i][j].push_back(lr+ud+':');
            }
        }
    }
    file.close();
    file.open("arrow_keypad.txt", ios::in);
    char_grid arrowpad(file);
    for(auto i: possible) {
        arrowpad_possible.push_back({});
        arrowpad_shortest.push_back({});
        for(auto j: possible) {
            arrowpad_shortest.back().push_back("");
            arrowpad_possible.back().push_back({});
            size_t idx_i = to_idx(i);
            size_t idx_j = to_idx(j);
            coord start = arrowpad.find(i);
            coord end = arrowpad.find(j);
            string lr="";
            string ud="";
            for(int k=0; k<abs(start.first-end.first); ++k) {
                ud+= (start.first-end.first > 0) ? '^' : 'v';
            }
            for(int k=0; k<abs(start.second-end.second); ++k) {
                lr+= (start.second-end.second > 0) ? '<' : '>';
            }
            if(ud == "" || lr == "")
                arrowpad_possible[idx_i][idx_j].push_back(ud+lr+':');
            else {
                if(start.second != 0 || end.first != 0)
                    arrowpad_possible[idx_i][idx_j].push_back(ud+lr+':');
                if(start.first != 0 || end.second != 0)
                    arrowpad_possible[idx_i][idx_j].push_back(lr+ud+':');
            }
        }
    }
}

string first_arrow_code(const string& code) {
    string res = "";
    char prev = ':';
    for(char ch: code) {
        res += numpad_shortest[prev-48][ch-48];
        prev = ch;
    }
    return res;
}

void possible_next_code(const string& code, vector<string>& out) {
    vector<string> temp = {};
    string sub_code = code.substr(1, code.size()-1);
    char prev = ':';
    out.push_back("");
    for(char ch: code) {
        temp = out;
        out.clear();
        for(auto res: arrowpad_possible[to_idx(prev)][to_idx(ch)]){
            for(string cur: temp) {
                out.push_back(cur+res);
            }
        }
        prev = ch;
    }
}

string find_longterm_best(string& code0, string& code1) {
    vector<vector<vector<string>>> history = {{{code0}},{{code1}}};
    while(1) {
        vector<u64> mins = {0, 0};
        for(int i=0; i<2; ++i) {
            mins[i] = -1;
            auto hist_back = history[i].back();
            history[i].push_back({});
            for(auto c: hist_back) {
                vector<string> out = {};
                possible_next_code(c, out);
                for(auto c: out) {
                    history[i].back().push_back(c);
                    if(c.size()<mins[i]) mins[i] = c.size();
                }
            }
        }
        if(mins[1] < mins[0]) return code1;
        if(mins[0] < mins[1]) return code0;
    }
}

void build_minimum_pushes(vector<vector<vector<u64>>>& hist) {
    for(auto c1: possible) {
        hist.push_back({});
        for(auto c2: possible) {
            hist.back().push_back({arrowpad_shortest[to_idx(c1)][to_idx(c2)].size()});
        }
    }
    for(int idx=0; idx<27; ++idx) {
        for(auto i: arrow_map) {
            for(auto j: arrow_map) {
                string next = arrowpad_shortest[i.second][j.second];
                char prev = ':';
                u64 res = 0;
                for(auto ch: next) {
                    res += hist[to_idx(prev)][to_idx(ch)][idx];
                    prev = ch;
                }
                hist[i.second][j.second].push_back(res);
            }
        }
    }
}

void first_part(fstream& file) {
    char ch;
    u64 res1 = 0;
    u64 res2 = 0;
    fill_cache_compute();
    for(auto i: arrow_map) {
        for(auto j: arrow_map) {
            vector<string>& possible = arrowpad_possible[i.second][j.second];
            if(possible.size() > 1)
                possible = {find_longterm_best(possible[0], possible[1])};
            arrowpad_shortest[i.second][j.second] = possible[0];
        }
    }
    for(int i=0; i<11; ++i) {
        numpad_shortest.push_back({});
        for(int j=0; j<11; ++j) {
            vector<string>& possible = numpad_possible[i][j];
            if(possible.size() > 1)
                possible = {find_longterm_best(possible[0], possible[1])};
            numpad_shortest[i].push_back(possible[0]);
            cout << (char)(i+48) << (char)(j+48) << ' ' << possible << endl;
        }
    }

    vector<string> codes = {};
    string code = "";
    while (file.get(ch)) {
        if(ch == '\n') {
            codes.push_back(code);
            code="";
        } else if(ch == 'A') code += ':';
        else {
            code += ch;
        }
    }
    vector<vector<vector<u64>>> hist = {};
    build_minimum_pushes(hist);
    for(auto code: codes) {
        u64 numpart = 0;
        for(char ch: code) {
            if(ch == ':') break;
            numpart = numpart*10 + ch-48;
        }
        vector<string> keypad = {};
        string first_code = first_arrow_code(code);
        char prev = ':';
        u64 min1 = 0;
        u64 min2 = 0;
        for(auto ch: first_code) {
            min1 += hist[to_idx(prev)][to_idx(ch)][1];
            min2 += hist[to_idx(prev)][to_idx(ch)][24];
            prev = ch;
        }
        res1 += numpart*min1;
        res2 += numpart*min2;
    }
    cout << "Answer is: " << res1 << endl;
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

