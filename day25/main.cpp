#include "../utils.cpp"

using namespace std;

void first_part(fstream& file) {
    u64 res = 0;

    vector<vector<char>> keys = {};
    vector<vector<char>> locks = {};
    while (!file.eof()) {
        char_grid grid(file);
        bool is_lock = grid[0][0] == '#';
        vector<char> pattern(5,0);
        for(auto row: grid) {
            for(int j=0; j<grid.col; ++j) {
                pattern[j] += row[j] == '#';
            }
        }
        if(grid[0][0] == '#') {
            locks.push_back(pattern);
        } else {
            keys.push_back(pattern);
        }
    }
    for(auto lock: locks) {
        for(auto key: keys) {
            bool valid = true;
            for(int i=0; i<5; ++i) {
                if(key[i]+lock[i]>7) {
                    valid = false;
                    break;
                }
            }
            if(valid) ++res;
        }
    }

    cout << "Answer is: " << res << endl;
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

