#include "../utils.cpp"

using namespace std;

bool move_boxes(coord loc, coord offset, char_grid& grid) {
    loc += offset;
    if(grid(loc) == '.') {
        return true;
    } else if(grid(loc) == '[' || grid(loc) == ']' || grid(loc) == 'O') {
        auto res = move_boxes(loc,offset,grid);
        if(res) grid(loc+offset) = grid(loc);
        return res;
    } else /*if(grid[i][j] == '#')*/
        return false;
}

void move_robot(coord& robot_loc, char dir, char_grid& grid) {
    auto offset = char_to_coord_offset(dir);
    bool move_successful = move_boxes(robot_loc,offset,grid);
    if(move_successful) {
        robot_loc += offset;
        grid(robot_loc) = '.';
    }
}

u64 compute_gps(char_grid& grid) {
    u64 res = 0;
    for(u64 i=0;i<grid.row;++i) {
        for(u64 j=0;j<grid.col;++j) {
            if(grid[i][j] == 'O' || grid[i][j] == '[')
                res += 100*i+j;
        }
    }
    return res;
}

void first_part(fstream& file) {
    char_grid grid(file);
    char_vec directions(file, false);

    int i, j;
    for(i=0;i<grid.row;++i) {
        for(j=0;j<grid.col;++j) {
            if(grid[i][j] == '@') goto bot_detected;
        }
    }
    bot_detected:
    coord robot = {i,j};
    grid(robot) = '.';

    for(char dir : directions) {
        move_robot(robot, dir, grid);
    }
    grid(robot) = '@';

    cout << "Answer is: " << compute_gps(grid) << endl;
}

bool check_wide_boxes_can_move_vertically(coord loc, coord offset, char_grid& grid) {
    loc += offset;
    if(grid(loc) == '.') {
        return true;
    } else if(grid(loc) == '[' || grid(loc) == ']') {
        coord other_loc = grid(loc) == '['
                            ? loc+(coord){0,1}
                            : loc+(coord){0,-1};
        auto lhs = check_wide_boxes_can_move_vertically(loc,offset,grid);
        auto rhs = check_wide_boxes_can_move_vertically(other_loc,offset,grid);
        return lhs && rhs;
    } else /*if(grid[i][j] == '#')*/
        return false;
}

void move_wide_boxes_vertically(coord loc, coord offset, char_grid& grid) {
    loc += offset;
    if(grid(loc) == '[' || grid(loc) == ']') {
        coord other_loc = grid(loc) == '['
                            ? loc+(coord){0,1}
                            : loc+(coord){0,-1};
        move_wide_boxes_vertically(loc,offset,grid);
        move_wide_boxes_vertically(other_loc,offset,grid);
        grid(loc+offset) = grid(loc);
        grid(other_loc+offset) = grid(other_loc);
        grid(loc) = '.';
        grid(other_loc) = '.';
    }
}

void move_robot_2(coord& robot_loc, char ch_dir, char_grid& grid) {
    auto offset = char_to_coord_offset(ch_dir);
    auto dir = char_to_dir(ch_dir);
    bool move_successful;
    if(dir == Left || dir == Right) {
        move_successful = move_boxes(robot_loc,offset,grid);
    } else {
        move_successful =
            check_wide_boxes_can_move_vertically(robot_loc,offset,grid);
        if(move_successful)
            move_wide_boxes_vertically(robot_loc,offset,grid);
    }
    if(move_successful) {
        robot_loc += offset;
        grid(robot_loc) = '.';
    }
}

void second_part(fstream& file) {
    char_grid grid_p1(file);
    char_vec directions(file, false);
    char_grid double_grid = {};

    for(auto v : grid_p1) {
        double_grid.push_back({});
        for(auto c : v) {
            if(c == '.') {
                double_grid.back().push_back('.');
                double_grid.back().push_back('.');
            } else if(c == '#') {
                double_grid.back().push_back('#');
                double_grid.back().push_back('#');
            } else if(c == 'O') {
                double_grid.back().push_back('[');
                double_grid.back().push_back(']');
            } else if(c == '@') {
                double_grid.back().push_back('@');
                double_grid.back().push_back('.');
            }
        }
    }
    double_grid.update_row_col();
    int i,j;
    for(i=0;i<double_grid.row;++i) {
        for(j=0;j<double_grid.col;++j) {
            if(double_grid[i][j] == '@') goto bot_detected;
        }
    }
    bot_detected:
    coord robot = {i,j};
    cout << double_grid << endl;
    double_grid(robot) = '.';
    for(char dir : directions) {
        move_robot_2(robot, dir, double_grid);
    }
    double_grid(robot) = '@';
    cout << "Answer is: " << compute_gps(double_grid) << endl;
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

