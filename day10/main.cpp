#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

using u64 = unsigned long long;
typedef pair<int, int> coord;

int total_trails(int i, int j, vector<vector<char>>& grid, int wanted) {
    if(grid[i][j] != wanted) return 0;
    if(grid[i][j] == 9) return 1;
    int col = grid[0].size();
    int row = grid.size();
    int res = 0;
    int val = grid[i][j] + 1;
    if(i>0) res += total_trails(i-1, j, grid, val);
    if(j>0) res += total_trails(i, j-1, grid, val);
    if(j<col-1) res += total_trails(i, j+1, grid, val);
    if(i<row-1) res += total_trails(i+1, j, grid, val);
    return res;
}


void trail_summits(int i, int j, vector<vector<char>>& grid, int wanted, set<coord>& summits) {
    if(grid[i][j] != wanted) return;
    if(grid[i][j] == 9) {
        summits.insert({i,j});
        return;
    }
    int col = grid[0].size();
    int row = grid.size();
    int res = 0;
    int val = grid[i][j] + 1;
    if(i>0) trail_summits(i-1, j, grid, val, summits);
    if(j>0) trail_summits(i, j-1, grid, val, summits);
    if(j<col-1) trail_summits(i, j+1, grid, val, summits);
    if(i<row-1) trail_summits(i+1, j, grid, val, summits);
}


void first_part(fstream& file) {
    char ch;
    vector<vector<char>> grid = {{}};

    while (file.get(ch)) {
        if(ch == '\n') {
            grid.push_back({});
        }
        else {
            grid.back().push_back(ch-48);
        }
    }
    if(grid.back().size() == 0) {
        grid.pop_back();
    }

    int res = 0;
    int res2 = 0;
    int col = grid[0].size();
    int row = grid.size();
    set<coord> trailhead_summits = {};

    for(int i=0; i<row; ++i) {
        for(int j=0; j<col; ++j) {
            trail_summits(i, j, grid, 0, trailhead_summits);
            res += trailhead_summits.size();
            trailhead_summits.clear();
            res2 += total_trails(i, j, grid, 0);
        }
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

