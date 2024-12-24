#include "../utils.cpp"

using namespace std;

u64 cheat_gain_thresh = 100;
int cheat_len = 20;

u64 valid_cheat(char_grid& grid, u64_grid& tfs, coord start) {
    u64 res = 0;
    for(int i=-cheat_len; i<=cheat_len; ++i) {
        for(int j=-(cheat_len-abs(i)); j <=cheat_len-abs(i); ++j) {
            coord end = start+(coord){i,j};
            if(grid.safe_get(end) == '.' &&
                tfs(end) >= tfs(start) + cheat_gain_thresh + abs(i)+abs(j)) {
                ++res;
            }
        }
    }
    return res;
}

void first_part(fstream& file) {
    u64 res = 0;
    char_grid grid(file);
    coord start = grid.find('S');
    coord end = grid.find('E');
    u64 max_time = grid.row*grid.col;
    u64_grid tfs(max_time, grid.row, grid.col); // time_from_start
    tfs(start) = 0;
    // cout << grid << endl;
    grid(end) = '.';

    coord current = start;
    u64 time=0;
    while(current != end) {
        if(grid(current+Left) == '.' && tfs(current+Left) == max_time)
            current = current+Left;
        else if(grid(current+Right) == '.' && tfs(current+Right) == max_time)
            current = current+Right;
        else if(grid(current+Up) == '.' && tfs(current+Up) == max_time)
            current = current+Up;
        else if(grid(current+Down) == '.' && tfs(current+Down) == max_time)
            current = current+Down;
        ++time;
        tfs(current) = time;
    }
    cout << "Time to reach end: " << tfs(end) << endl;
    current = start;

    u64 progress = tfs(start);
    while(current != end) {

        if(grid.safe_get(current+Left+Left) == '.' && tfs(current+Left+Left) >= tfs(current) + cheat_gain_thresh +2)
            ++res;
        if(grid.safe_get(current+Right+Right) == '.' && tfs(current+Right+Right) >= tfs(current) + cheat_gain_thresh +2)
            ++res;
        if(grid.safe_get(current+Up+Up) == '.' && tfs(current+Up+Up)>= tfs(current) + cheat_gain_thresh +2)
            ++res;
        if(grid.safe_get(current+Down+Down) == '.' && tfs(current+Down+Down) >= tfs(current) + cheat_gain_thresh +2)
            ++res;


        ++progress;
        if(tfs(current+Left) == progress)
            current = current+Left;
        else if(tfs(current+Right) == progress)
            current = current+Right;
        else if(tfs(current+Up) == progress)
            current = current+Up;
        else if(tfs(current+Down) == progress)
            current = current+Down;
    }
    cout << "Answer is: " << res << endl;

    res = 0;
    current = start;
    progress = tfs(start);
    while(current != end) {

        res += valid_cheat(grid, tfs, current);

        ++progress;
        if(tfs(current+Left) == progress)
            current = current+Left;
        else if(tfs(current+Right) == progress)
            current = current+Right;
        else if(tfs(current+Up) == progress)
            current = current+Up;
        else if(tfs(current+Down) == progress)
            current = current+Down;
    }
    cout << "Answer is: " << res << endl;
}

void second_part(fstream& file) {
}

int main (int argc, char *argv[])
{
    fstream file;
    if(argc > 1 && !strcmp(argv[1], "-e")) {
        file.open("example.txt", ios::in);
        cheat_gain_thresh = 70;
    }
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

