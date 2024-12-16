#include "../utils.cpp"
#include <queue>
#include <set>

using namespace std;

coord flip(const coord c) {
    return {c.second,c.first};
}

typedef pair<coord_dir,u64> prio_coord_dir;

struct {
    bool operator()(const prio_coord_dir l, const prio_coord_dir r) const {
        return l.second > r.second;
    }
} customLess;
typedef priority_queue<prio_coord_dir, vector<prio_coord_dir>, decltype(customLess)> prioq;

void add_side(coord ij, coord offset, u64 prio, prioq& queue, char_grid& grid, class grid& visited) {
    coord flipped = flip(offset);
    if(grid(ij+flipped) != '#' && (visited(ij+flipped) == 0 || visited(ij+flipped) >= prio+1001)) {
        queue.push({{ij+flipped, coord_offset_to_dir(flipped)}, prio+1001});
        visited(ij+flipped) = prio+1001;
    }
    if(grid(ij-flipped) != '#' && (visited(ij-flipped) == 0 || visited(ij-flipped) >= prio+1001)) {
        queue.push({{ij-flipped, coord_offset_to_dir(-flipped)}, prio+1001});
        visited(ij-flipped) = prio+1001;
    }
}

void backtracker(set<coord>& s, const char_grid& grid, const vector<vector<vector<coord>>>& origin, coord end) {
    for(auto c : origin[end.first][end.second]) {
        if(s.count(c)>0) continue;
        s.insert(c);
        backtracker(s,grid,origin,c);
    }
}

u64 seats_available(char_grid& grid, const vector<vector<vector<coord>>>& origin, coord start, coord end) {
    u64 res = 0;
    set<coord> s = {};
    s.insert(end);
    backtracker(s,grid,origin,end);
    return s.size();
}

void first_part(fstream& file) {
    char ch;
    u64 res = 0;
    char_grid grid(file);
    coord start = grid.find('S');
    coord end = grid.find('E');
    prioq queue;
    class grid visited;
    vector<vector<vector<coord>>> origin = {};
    for(auto v : grid) {
        origin.push_back({});
        visited.push_back({});
        for(auto c : v) {
            origin.back().push_back({});
            visited.back().push_back(0);
        }
    }

    queue.push({{start,Right}, 0});
    queue.push({{start,Up}, 1000});

    while(queue.size()>0 && (res == 0 || queue.top().second <= res)) {
        auto elem = queue.top();
        queue.pop();
        u64 prio = elem.second;
        coord ij = elem.first.first;
        if(grid(ij) == 'E') {
            res = prio;
        }
        dir dir = elem.first.second;
        coord offset = dir_to_coord_offset(dir);
        for(auto c : origin[ij.first][ij.second]) {
            if(c == ij-offset) {
                goto continue_while; //optional (for speed)
            }
        }
        origin[ij.first][ij.second].push_back(ij-offset);

        if(grid(ij+offset) != '#' && (visited(ij+offset) == 0 || visited(ij+offset) >= prio+1 || visited(ij+offset+offset) >= prio+2)) {
            queue.push({{ij+offset, dir}, prio+1});
            visited(ij+offset) = prio+1;
        }
        add_side(ij+offset, offset, prio+1, queue, grid, visited);

        continue_while:;
    }

    cout << "Answer is: " << res << endl;
    origin[start.first][start.second].clear();
    cout << "Answer is: " << seats_available(grid, origin, start, end) << endl;
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

