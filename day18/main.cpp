#include "../utils.cpp"
#include <set>
#include <deque>

using namespace std;

u64 p1_bytes_fallen = 1024;
u64 memory_size = 71;

void update_if_min(u64& current, u64 _new) {
    if(current>_new) current = _new;
}
u64 dfs(char_grid& grid, u64_grid& dist) {
    u64 max_dist = memory_size*memory_size;
    coord start = {0,0};
    coord end = {memory_size-1,memory_size-1};
    dist(start) = 0;
    deque<coord> queue = {};
    queue.push_back(start);
    while(queue.size()>0) {
        auto head = queue.front();
        queue.pop_front();
        if(head==end) {
            return dist(end);
        }
        int i = head.first, j = head.second;
        u64 new_dist = dist(head)+1;
        if(i>0 && grid[i-1][j] != '#' && dist({i-1,j}) == max_dist) {
            queue.push_back({i-1,j});
            dist({i-1,j}) = new_dist;
        }
        if(j>0 && grid[i][j-1] != '#' && dist({i,j-1}) == max_dist) {
            queue.push_back({i,j-1});
            dist({i,j-1}) = new_dist;
        }
        if(j<grid.col-1 && grid[i][j+1] != '#' && dist({i,j+1}) == max_dist) {
            queue.push_back({i,j+1});
            dist({i,j+1}) = new_dist;
        }
        if(i<grid.row-1 && grid[i+1][j] != '#' && dist({i+1,j}) == max_dist) {
            queue.push_back({i+1,j});
            dist({i+1,j}) = new_dist;
        }
    }
    return 0;
}

void backtrace(char_grid& grid, u64_grid& dist, set<coord>& path) {
    deque<coord> queue = {};
    char_grid visited(0, grid.row, grid.col);
    queue.push_back({grid.row-1,grid.col-1});
    while(queue.size()>0) {
        auto head = queue.front();
        queue.pop_front();
        path.insert(head);
        if(dist(head)==0) {
            continue;
        }
        int i = head.first, j = head.second;
        u64 next_dist = dist(head)-1;
        if(i>0 && dist({i-1,j}) == next_dist && visited({i-1,j}) == 0) {
            queue.push_back({i-1,j});
            visited({i-1,j}) = 1;
        }
        if(j>0 && dist({i,j-1}) == next_dist && visited({i,j-1}) == 0) {
            queue.push_back({i,j-1});
            visited({i,j-1}) = 1;
        }
        if(j<grid.col-1 && dist({i,j+1}) == next_dist && visited({i,j+1}) == 0) {
            queue.push_back({i,j+1});
            visited({i,j+1}) = 1;
        }
        if(i<grid.row-1 && dist({i+1,j}) == next_dist && visited({i+1,j}) == 0) {
            queue.push_back({i+1,j});
            visited({i+1,j}) = 1;
        }
    }
}

void first_part(fstream& file) {
    u64 res = 0;
    coord start = {0,0};
    vector<coord> bytes = {};
    u64 max_dist = memory_size*memory_size;
    char_grid grid('.', memory_size, memory_size);
    deque<coord> queue = {};
    vector<int> out = {};
    while(!file.eof()) {
        get_int_list(file, ',', out);
        bytes.push_back({out[1],out[0]});
        out.clear();
    }
    bytes.pop_back();
    for(u64 i =0; i<p1_bytes_fallen; ++i) grid(bytes[i]) = '#';
    u64_grid dist(max_dist, memory_size, memory_size);

    res = dfs(grid,dist);
    cout << "Answer is: " << res << endl;


    set<coord> path = {};
    backtrace(grid, dist, path);
    for(u64 i = p1_bytes_fallen; i<bytes.size(); ++i) {
        grid(bytes[i]) = '#';
        if(path.count(bytes[i])>0) {
            for(auto& row : dist) {
                for(auto& i : row) {
                    i = max_dist;
                }
            }
            dist(start) = 0;
            if(dfs(grid,dist)>0) {
                path.clear();
                backtrace(grid, dist, path);
            } else {
                cout << "Answer is: " << bytes[i].second << ',' << bytes[i].first << endl;
                break;
            }
        }
    }
}

void second_part(fstream& file) {
}

int main (int argc, char *argv[])
{
    fstream file;
    if(argc > 1 && !strcmp(argv[1], "-e")) {
        file.open("example.txt", ios::in);
        p1_bytes_fallen = 12;
        memory_size = 7;
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

