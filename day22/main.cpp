#include "../utils.cpp"

using namespace std;

u64 prune = 16777216;

u64 inline next_pseudorandom(u64 seed) {
    seed = ((64*seed) ^ seed) % prune;
    seed = ((seed/32) ^ seed) % prune;
    return ((seed*2048) ^ seed) % prune;
}

u64 nth_pseudorandom(u64 seed, u64 nth) {
    for(int i=0; i<nth; ++i) {
        seed = next_pseudorandom(seed);
    }
    return seed;
}

void first_part(fstream& file) {
    u64 res = 0;
    vector<u64> secrets = {};
    while (!file.eof()) {
        get_num_list(file, " ", secrets);
    }
    for(auto seed: secrets) res += nth_pseudorandom(seed, 2000);
    cout << "Answer is: " << res << endl;
}

inline void register_price(unsigned short bananas[19][19][19][19], bool by_buyers[19][19][19][19], signed char var[4], size_t vh, u64 seed) {
    bool& already_reached = by_buyers[var[vh]][var[(vh+3)%4]][var[(vh+2)%4]][var[(vh+1)%4]];
    if(already_reached) return;
    bananas[var[vh]][var[(vh+3)%4]][var[(vh+2)%4]][var[(vh+1)%4]] += seed%10;
    already_reached = true;
}

void second_part(fstream& file) {
    u64 res = 0;
    vector<u64> secrets = {};
    while (!file.eof()) {
        get_num_list(file, " ", secrets);
    }
    unsigned short bananas[19][19][19][19] = {0};
    for(auto seed: secrets) {
        bool by_buyer[19][19][19][19] = {0};
        signed char variations[4] = {0};
        size_t var_head = 0;
        for(int i=0; i<3; ++i) {
            u64 next = next_pseudorandom(seed);
            var_head = (var_head+1)%4;
            variations[var_head] = (next%10)-(seed%10)+9;
            seed = next;
        }
        for(int i=3; i<2000; ++i) {
            u64 next = next_pseudorandom(seed);
            var_head = (var_head+1)%4;
            variations[var_head] = (next%10)-(seed%10)+9;
            seed = next;
            register_price(bananas, by_buyer, variations, var_head, seed);
        }
    }
    for(auto w=0; w<19; ++w) {
        for(auto x=0; x<19; ++x) {
            for(auto y=0; y<19; ++y) {
                for(auto z=0; z<19; ++z) {
                    unsigned short b = bananas[w][x][y][z];
                    if(b>res) res = b;
                }
            }
        }
    }
    cout << "Answer is: " << res << endl;
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

