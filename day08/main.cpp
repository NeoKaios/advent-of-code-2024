#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include "../utils.cpp"

using namespace std;

using u64 = unsigned long long;
typedef pair<int, int> coord;

struct antenna_group {
    char freq;
    vector<coord> coords;
};

coord operator+(coord a, coord b) {
    return {a.first + b.first, a.second + b.second};
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

void collect_antennas(char_grid& grid, vector<antenna_group>& antennas) {
    for(int i=0; i<grid.row; ++i) {
        for(int j=0; j<grid.col; ++j) {
            if(grid[i][j] == '.') continue;
            bool found = false;
            for(auto& ag : antennas) {
                if(ag.freq != grid[i][j]) continue;
                ag.coords.push_back({i,j});
                found = true;
                break;
            }
            if(!found) antennas.push_back({ grid[i][j], { {i,j} } });
        }
    }
}

int compute_antinodes(int row, int col, vector<antenna_group>& antennas) {
    set<coord> antinodes = {};
    coord lower_bound = {0,0};
    coord upper_bound = {row-1,col-1};

    for(auto ag : antennas) {
        for(auto c1 : ag.coords) {
            for(auto c2 : ag.coords) {
                if(c1 == c2) continue;
                coord antinode = c1+c1-c2;
                if(lower_bound <= antinode && antinode <= upper_bound)
                    antinodes.insert(antinode);
            }
        }
    }
    return antinodes.size();
}

int compute_harmonic_antinodes(int row, int col, vector<antenna_group>& antennas) {
    set<coord> antinodes = {};
    coord lower_bound = {0,0};
    coord upper_bound = {row-1,col-1};

    for(auto ag : antennas) {
        for(auto c1 : ag.coords) {
            for(auto c2 : ag.coords) {
                if(c1 == c2) continue;
                coord diff = c1-c2;
                coord antinode = c2 + diff;
                while(lower_bound <= antinode && antinode <= upper_bound) {
                    antinodes.insert(antinode);
                    antinode = antinode + diff;
                }

            }
        }
    }
    return antinodes.size();
}
void first_part(fstream& file) {
    char ch;
    char_grid grid(file);

    int res = 0;
    int res2 = 0;

    vector<antenna_group> antennas = {};
    collect_antennas(grid, antennas);
    res = compute_antinodes(grid.row, grid.col, antennas);
    res2 = compute_harmonic_antinodes(grid.row, grid.col, antennas);

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

