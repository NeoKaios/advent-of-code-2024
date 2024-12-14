#include "../utils.cpp"

using namespace std;

int row = 103;
int col = 101;

typedef tuple<i64,i64,i64,i64> pos_speed;

void fill_pos_speed(fstream& file, vector<pos_speed>& out) {
    while (!file.eof()) {
        vector<int> parsing_output = {};
        if(parse_int_list(file, "p=%,% v=%,%", parsing_output)) break;
        out.push_back({
            parsing_output[1],
            parsing_output[0],
            parsing_output[3],
            parsing_output[2]}
                      );
    }
}

void show_state(vector<pos_speed>& robots_inits, i64 step_to_show) {
    char_grid grid('.', row, col);
    for(auto t : robots_inits) {
        i64 y, x, vy, vx;
        tie(x,y,vx,vy) = t;

        x = ((x+vx*step_to_show)%row + row)%row;
        y = ((y+vy*step_to_show)%col + col)%col;

        if(grid[x][y]=='.')
            grid[x][y]='1';
        else grid[x][y]++;
    }
    cout << "Steps " << step_to_show << ':' << endl;
    cout << grid << endl;
}

void first_part(fstream& file) {
    char ch;
    u64 q1 = 0,
        q2 = 0,
        q3 = 0,
        q4 = 0;

    int mid_x = row/2,
        mid_y = col/2;

    int steps = 100;

    vector<pos_speed> robots_inits = {};
    fill_pos_speed(file, robots_inits);
    for(auto t : robots_inits) {
        i64 y, x, vy, vx;
        tie(x,y,vx,vy) = t;

        x = ((x+vx*steps)%row + row)%row;
        y = ((y+vy*steps)%col + col)%col;

        if(x<mid_x) {
            if(y<mid_y) ++q1;
            else if(y>mid_y) ++q2;
        } else if(x>mid_x) {
            if(y<mid_y) ++q3;
            else if(y>mid_y) ++q4;
        }
    }
    cout << "Answer is (" << q1 << ',' << q2 << ',' << q3 << ',' << q4 << "): " << q1*q2*q3*q4 << endl;
}

void second_part(fstream& file) {
    u64 q1 = 0,
        q2 = 0,
        q3 = 0,
        q4 = 0;

    int mid_x = row/2,
        mid_y = col/2;

    vector<pos_speed> robots_inits = {};
    fill_pos_speed(file, robots_inits);
    i64 steps = 0;
    i64 y, x, vy, vx, x_, y_;
    // looking for cycle length
    auto first = robots_inits[0];
    continue_looking:
    tie(x,y,vx,vy) = first;
    while(1) {
        ++steps;
        if(x == ((x+vx*steps)%row + row)%row &&
        y == ((y+vy*steps)%col + col)%col) {
            break;
        }
    }
    for(auto t : robots_inits) {
        tie(x,y,vx,vy) = t;

        if(x == ((x+vx*steps)%row + row)%row &&
            y == ((y+vy*steps)%col + col)%col)
            continue;
        goto continue_looking;
    }
    cout << "Cycle is: " << steps << endl;
    i64 cycle = steps;
    u64 min_avg = (row*row + col*col)*robots_inits.size();

    steps=0;
    i64 min_avg_steps =0;
    while(++steps<cycle) {
        u64 avg = 0;
        for(auto t : robots_inits) {
            tie(x,y,vx,vy) = t;

            x = ((x+vx*steps)%row + row)%row;
            y = ((y+vy*steps)%col + col)%col;

            avg += (x-mid_x)*(x-mid_x) + (y-mid_y)*(y-mid_y);


        }
        if(avg < min_avg) {
            min_avg_steps = steps;
            min_avg = avg;
        }
    }
    show_state(robots_inits, min_avg_steps);
    cout << "Answer is: " << min_avg_steps << " ?" << endl;
}

int main (int argc, char *argv[])
{
    fstream file;
    if(argc > 1 && !strcmp(argv[1], "-e")) {
        file.open("example.txt", ios::in);
        row = 7;
        col = 11;
    }
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

