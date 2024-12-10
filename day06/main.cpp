#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

std::ostream& operator<<(std::ostream& ostr, const std::vector<char>& vec)
{
    for (const char i : vec)
        ostr << i;
    return ostr;
}

std::ostream& operator<<(std::ostream& ostr, const std::vector<vector<char>>& vec)
{
    for (const vector<char> v : vec)
        ostr << v << endl;
    return ostr;
}

void build_grid(vector<vector<char>>& grid, fstream& file) {
    char ch;
    while (file.get(ch)) {
        if(ch == '\n') {
            grid.push_back({});
        } else if (ch == '.' || ch == '#' || ch == '^') {
            grid.back().push_back(ch);
        } else {
            cout << "Unknown char " << ch << endl;
        }
    }
    if(grid.back().size() == 0) grid.pop_back();
}

void get_start(vector<vector<char>>& grid, char* start_i, char* start_j) {
    int row = grid.size();
    int col = grid[0].size();
    for(int _i = 0; _i < row; ++_i) {
        for(int _j = 0; _j < col; ++_j) {
            if(grid[_i][_j] == '^') {
                *start_i = _i;
                *start_j = _j;
            }
        }
    }
}

int travel_n_count(vector<vector<char>>& grid, char start_i, char start_j, bool* loop) {
    int row = grid.size();
    int col = grid[0].size();
    char dir = '^';
    int i = start_i;
    int j = start_j;
    grid[i][j] = dir;
    *loop = false;
    int res = 1;
    while(1) {
        if(dir == '^' && i == 0 ||
            dir == '>' && j == col-1 ||
            dir == '<' && j == 0 ||
            dir == 'v' && i == row-1) break;
        if(i<0 || i>=row || j<0 || j>=col) break;
        // up
        if(dir == '^')
            grid[i-1][j] == '#' ? dir = '>' : i--;

        // down
        if(dir == 'v')
            grid[i+1][j] == '#' ? dir = '<' : i++;

        // left
        if(dir == '<')
            grid[i][j-1] == '#' ? dir = '^' : j--;

        // right
        if(dir == '>')
            grid[i][j+1] == '#' ? dir = 'v' : j++;

        if(grid[i][j] == dir) {
            *loop = true;
            break;
        }
        if(grid[i][j] == '.') {
            grid[i][j] = dir;
            res++;
        }
    }
    return res;
}

void clear_grid(vector<vector<char>>& grid) {
    int row = grid.size();
    int col = grid[0].size();
    for(int i = 0; i < row; ++i) {
        for(int j = 0; j < col; ++j) {
            if(grid[i][j] != '#')
                grid[i][j] = '.';
        }
    }
}

void first_part(fstream& file) {
    vector<vector<char>> grid = {{}};
    build_grid(grid, file);

    char i = 0;
    char j = 0;
    get_start(grid, &i, &j);

    bool l;
    int res = travel_n_count(grid, i, j, &l);
    cout << "Answer is: " << res << endl;
}

void second_part(fstream& file) {
    char ch;
    int res = 0;
    vector<vector<char>> grid = {{}};
    vector<vector<char>> grid2 = {{}};
    build_grid(grid, file);
    file.clear();
    file.seekg(0, ios::beg);
    build_grid(grid2, file);

    int row = grid.size();
    int col = grid[0].size();
    char start_i = 0;
    char start_j = 0;
    char dir = '^';
    bool loop = false;
    get_start(grid, &start_i, &start_j);
    travel_n_count(grid, start_i, start_j, &loop);

    for(int i = 0; i < row; ++i) {
        for(int j = 0; j < col; ++j) {
            if(grid[i][j] == '.' || grid[i][j] == '#') continue;
            if(i == start_i && j == start_j) continue;
            grid2[i][j] = '#';
            travel_n_count(grid2, start_i, start_j, &loop);

            if(loop) {
                ++res;
                // grid2[i][j] = 'O';
                // cout << grid2 << endl;
            }
            clear_grid(grid2);
            grid2[i][j] = '.';

        }
    }
    cout << "Answer is: " << res << endl;
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

