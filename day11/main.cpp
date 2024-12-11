#include "../utils.cpp"
#include <map>
#include <cmath>

using namespace std;

const int max_blinks = 75;

u64 blink(u64 stone, int remaining_blinks, map<u64, vector<u64>>& history) {
    if(remaining_blinks <= 0) return 1;
    if(history.count(stone)==0) {
        vector<u64> u(max_blinks+1, 0);
        history[stone] = u;
    }
    if(history[stone][remaining_blinks] != 0) {
        return history[stone][remaining_blinks];
    }

    if(stone == 0) {
        u64 res = blink(1, remaining_blinks-1, history);
        history[stone][remaining_blinks] = res;
        return res;
    }
    int digits = log10(stone)+1;
    if(digits%2 == 0) {
        u64 a = pow(10, digits/2);
        u64 res1 = blink(stone/a, remaining_blinks-1, history);
        u64 res2 = blink(stone%a, remaining_blinks-1, history);
        history[stone][remaining_blinks] = res1 + res2;
        return res1 + res2;
    }
    u64 res = blink(stone*2024, remaining_blinks-1, history);
    history[stone][remaining_blinks] = res;
    return res;

}

void first_part(fstream& file) {
    char ch;
    u64 res = 0;
    vector<u64> stones = {};
    get_u64_list(file, ' ', stones);
    cout << stones << endl;

    map<u64, vector<u64>> history = {};
    for(u64 stone : stones)
        res += blink(stone, 25, history);
    cout << "Answer is: " << res << endl;
    res = 0;
    for(u64 stone : stones)
        res += blink(stone, 75, history);
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

