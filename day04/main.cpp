#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

std::ostream& operator<<(std::ostream& ostr, const std::vector<char>& vec)
{
    for (const char i : vec)
        ostr << ' ' << i;
    return ostr;
}

std::ostream& operator<<(std::ostream& ostr, const std::vector<vector<char>>& vec)
{
    for (const vector<char> v : vec)
        ostr << v << endl;
    return ostr;
}

int check_xmas_at(int i, int j, vector<vector<char>> grid) {
    int col = grid[0].size();
    int row = grid.size();
    int res = 0;
    if(grid[i][j] != 'X') return 0;
    // horizontal
    if(j<=col-4 &&
        grid[i][j+1] == 'M' &&
        grid[i][j+2] == 'A' &&
        grid[i][j+3] == 'S')
        res++;
    if(j>=3 &&
        grid[i][j-1] == 'M' &&
        grid[i][j-2] == 'A' &&
        grid[i][j-3] == 'S')
        res++;
    // vertical
    if(i>=3 &&
        grid[i-1][j] == 'M' &&
        grid[i-2][j] == 'A' &&
        grid[i-3][j] == 'S')
        res++;
    if(i<=row-4 &&
        grid[i+1][j] == 'M' &&
        grid[i+2][j] == 'A' &&
        grid[i+3][j] == 'S')
        res++;
    // diagonal
    if(i<=row-4 && j<=col-4 &&
        grid[i+1][j+1] == 'M' &&
        grid[i+2][j+2] == 'A' &&
        grid[i+3][j+3] == 'S')
        res++;
    if(i<=row-4 && j>=3 &&
        grid[i+1][j-1] == 'M' &&
        grid[i+2][j-2] == 'A' &&
        grid[i+3][j-3] == 'S')
        res++;
    if(i>=3 && j>=3 &&
        grid[i-1][j-1] == 'M' &&
        grid[i-2][j-2] == 'A' &&
        grid[i-3][j-3] == 'S')
        res++;
    if(i>=3 && j<=col-4 &&
        grid[i-1][j+1] == 'M' &&
        grid[i-2][j+2] == 'A' &&
        grid[i-3][j+3] == 'S')
        res++;
    return res;
}

int check_x_mas_at(int i, int j, vector<vector<char>> grid) {
    int col = grid[0].size();
    int row = grid.size();
    int res = 0;
    if(grid[i][j] != 'A' || i == 0 || j == 0 || j == col-1 || i == row-1)
        return 0;
    if(grid[i+1][j+1] == 'M' &&
        grid[i-1][j-1] == 'S' ||
        grid[i-1][j-1] == 'M' &&
        grid[i+1][j+1] == 'S')
        res++;
    if(grid[i+1][j-1] == 'M' &&
        grid[i-1][j+1] == 'S' ||
        grid[i-1][j+1] == 'M' &&
        grid[i+1][j-1] == 'S')
        res++;

    return res>1;
}

void first_part(fstream& file) {
    char ch;
    vector<vector<char>> grid = {{}};
    int res = 0;

    while (file.get(ch)) {
        if(ch == '\n') {
            grid.push_back({});
        }
        else {
            grid.back().push_back(ch);
        }
    }
    if(grid.back().size() == 0) {
        grid.pop_back();
    }

    int col = grid[0].size();
    int row = grid.size();

    for(int i=0; i<row; ++i) {
        for(int j=0; j<col; ++j) {
            res += check_xmas_at(i, j, grid);
        }
    }
    cout << "Answer is: " << res << endl;
    res = 0;
    for(int i=0; i<row; ++i) {
        for(int j=0; j<col; ++j) {
            res += check_x_mas_at(i, j, grid);
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

