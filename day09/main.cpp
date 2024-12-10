#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

std::ostream& operator<<(std::ostream& ostr, const std::vector<char>& vec)
{
    for (const int i : vec)
        ostr << i;
    return ostr;
}

void build_disk_map(fstream& file, vector<char>& disk_map) {
    char ch;
    char current = 0;
    while (file.get(ch)) {
        if(ch == '\n') {

        } else if(ch >= '0' && ch <= '9') {
            disk_map.push_back(ch - 48);
        }
        else {
            cout << "Unknown char " << ch << endl;
        }
    }
}

int id_of(int idx) {
    return idx/2;
}

void first_part(fstream& file) {
    long res = 0;
    vector<char> disk_map = {};
    build_disk_map(file, disk_map);

    int idx_begin = 0;
    int idx_end = disk_map.size()-1;
    int id = 0;
    int last_id = idx_end/2;
    int expanded_rank = 0;


    while(idx_begin < idx_end ||
        (idx_begin%2 == 0 && disk_map[idx_begin] > 0)) {
        // begin is file
        if(idx_begin % 2 == 0) {
            if(disk_map[idx_begin] > 0) {
                res += expanded_rank*id_of(idx_begin);
                expanded_rank++;
                disk_map[idx_begin]--;
            } else {
                idx_begin++;
            }
        } else {
            if(disk_map[idx_begin] > 0) {
                if(disk_map[idx_end] > 0) {
                    res += expanded_rank*id_of(idx_end);
                    expanded_rank++;
                    disk_map[idx_begin]--;
                    disk_map[idx_end]--;
                } else {
                    idx_end -= 2;
                }
            } else {
                idx_begin++;
            }
        }
    }
    cout << "Answer is: " << res << endl;
}

struct block {
    bool is_free;
    char size;
    int file_id;
};

std::ostream& operator<<(std::ostream& ostr, const std::list<block>& l)
{
    for (const block b : l) {
        for(int i = 0; i<b.size; ++i) {
            if(b.is_free)
                ostr << '.';
            else
                ostr << b.file_id;
        }
    }
    return ostr;
}

void second_part(fstream& file) {
    long res = 0;
    vector<char> disk_map = {};
    build_disk_map(file, disk_map);
    list<block> disk = {};
    for(int i = 0; i<disk_map.size(); ++i) {
        if(i%2==0) {
            disk.push_back({false, disk_map[i], id_of(i)});
        } else {
            disk.push_back({true, disk_map[i], 0});
        }
    }

    // vector<int> expanded = {};
    // for(int i = 0; i<disk_map.size(); ++i) {
    //     if(i%2==0) {
    //         for(int j=0; j<disk_map[i]; ++j)
    //             expanded.push_back(id_of(i));
    //     } else {
    //         for(int j=0; j<disk_map[i]; ++j)
    //             expanded.push_back(-1);
    //     }
    // }

    for(auto last_it = disk.end(); last_it != disk.begin(); --last_it) {
        if(last_it->is_free) continue;
        for(auto it = disk.begin(); it != last_it; ++it) {
            if(!it->is_free || it->size < last_it->size) continue;
            char remaing_space = it->size - last_it->size;
            it->is_free = false;
            it->file_id = last_it->file_id;
            it->size = last_it->size;
            last_it->is_free = true;
            if(remaing_space > 0) {
                disk.insert(++it, {true,remaing_space,0});
            }
            break;
        }
        // cout << disk << "   "<< last_it->file_id << endl;
    }
    int rank = 0;
    for(auto b : disk) {
        if(!b.is_free) {
            for(int i = rank; i < rank+b.size; ++i) {
                // cout << i << '*'<<b.file_id << endl;
                res += i*b.file_id;
            }
        }
        rank += b.size;
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

