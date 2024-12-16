#include "../utils.cpp"
#include <set>

using namespace std;
typedef pair<u64,u64> pairu64;
typedef tuple<int,int,dir> int_int_dir;

void operator+=(pairu64& a, pairu64 b) {
    a.first += b.first;
    a.second += b.second;
}

pair<u64,u64> visit_region(int i, int j, char_grid& grid, set<coord>& history) {
    if(history.count({i,j})>0) return {0,0};
    history.insert({i,j});
    pairu64 res = {1,0};
    if(i>0 && grid[i][j] == grid[i-1][j])
        res += visit_region(i-1,j,grid,history);
    else res += (pairu64){0,1};
    if(j>0 && grid[i][j] == grid[i][j-1])
        res += visit_region(i,j-1,grid,history);
    else res += (pairu64){0,1};
    if(i<grid.row-1 && grid[i][j] == grid[i+1][j])
        res += visit_region(i+1,j,grid,history);
    else res += (pairu64){0,1};
    if(j<grid.col-1 && grid[i][j] == grid[i][j+1])
        res += visit_region(i,j+1,grid,history);
    else res += (pairu64){0,1};
    return res;
}

u64 sides(int i, int j, char_grid& grid, dir dir, set<int_int_dir>& history) {
    if(history.count({i,j,dir})>0) return 0;
    history.insert({i,j,dir});
    switch(dir) {
        case Right:
            if(j<grid.col-1 && grid[i][j] == grid[i][j+1]) {
                if(i>0 && grid[i][j] == grid[i-1][j+1])
                    return 1+sides(i-1,j+1,grid,Up,history);
                else return sides(i,j+1,grid,dir,history);
            } else return 1+sides(i,j,grid,Down,history);
        break;
        case Down:
            if(i<grid.row-1 && grid[i][j] == grid[i+1][j]) {
                if(j<grid.col-1 && grid[i][j] == grid[i+1][j+1])
                    return 1+sides(i+1,j+1,grid,Right,history);
                return sides(i+1,j,grid,dir,history);
            } else return 1+sides(i,j,grid,Left,history);
        break;
        case Left:
            if(j>0 && grid[i][j] == grid[i][j-1]) {
                if(i<grid.row-1 && grid[i][j] == grid[i+1][j-1])
                    return 1+sides(i+1,j-1,grid,Down,history);
                return sides(i,j-1,grid,dir,history);
            } else return 1+sides(i,j,grid,Up,history);
        break;
        case Up:
            if(i>0 && grid[i][j] == grid[i-1][j]) {
                if(j>0 && grid[i][j] == grid[i-1][j-1])
                    return 1+sides(i-1,j-1,grid,Left,history);
                return sides(i-1,j,grid,dir,history);
            } else return 1+sides(i,j,grid,Right,history);
        break;
    }
    cout << "Error here" << endl;
    return 0;
}

u64 interior_sides(int i, int j, char_grid& grid, set<int_int_dir>& sides_history, set<coord>& area_history) {
    if(area_history.count({i,j})>0) return 0;
    area_history.insert({i,j});
    u64 res = 0;
    if(i>0) {
        if(grid[i][j] == grid[i-1][j])
            res+=interior_sides(i-1,j,grid,sides_history,area_history);
        else if(sides_history.count({i,j,Right})==0)
            res+=sides(i,j,grid,Right,sides_history);
    }
    if(j>0) {
        if(grid[i][j] == grid[i][j-1])
            res+=interior_sides(i,j-1,grid,sides_history,area_history);
        else if(sides_history.count({i,j,Up})==0)
            res+=sides(i,j,grid,Up,sides_history);
    }
    if(i<grid.row-1) {
        if(grid[i][j] == grid[i+1][j])
            res+=interior_sides(i+1,j,grid,sides_history,area_history);
        else if(sides_history.count({i,j,Left})==0)
            res+=sides(i,j,grid,Left,sides_history);
    }
    if(j<grid.col-1) {
        if(grid[i][j] == grid[i][j+1])
            res+=interior_sides(i,j+1,grid,sides_history,area_history);
        else if(sides_history.count({i,j,Down})==0)
            res+=sides(i,j,grid,Down,sides_history);
    }
    return res;
}

u64 region_sides(int i, int j, char_grid& grid) {
    set<int_int_dir> sides_history = {};
    set<coord> area_history = {};
    u64 res = sides(i,j,grid,Right,sides_history);
    return res+interior_sides(i,j,grid,sides_history,area_history);
}

void first_part(fstream& file) {
    char ch;
    u64 res = 0;
    u64 res2 = 0;
    char_grid grid(file);
    set<coord> global_history = {};
    for(int i=0; i<grid.row; ++i) {
        for(int j=0; j<grid.col; ++j) {
            if(global_history.count({i,j})>0) continue;
            auto r = visit_region(i,j,grid,global_history);
            res += r.first*r.second;
            u64 sides = region_sides(i,j,grid);
            res2 += r.first*sides;
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

