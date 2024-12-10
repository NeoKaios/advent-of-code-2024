#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

using u64 = unsigned long long;

u64 concat(u64 a, u64 b) {
    return a*pow(10,(u64)log10(b)+1) + b;

}

bool rec(u64 temp_res, u64 actual_res, u64* last, u64* current_value) {
    if(temp_res > actual_res) return false;
    u64 add_temp_res = temp_res + *current_value;
    u64 mult_temp_res = temp_res * *current_value;
    if(current_value == last)
        return actual_res == add_temp_res ||
            actual_res == mult_temp_res;

    u64* new_value = ++current_value;
    return rec(add_temp_res, actual_res, last, new_value) ||
        rec(mult_temp_res, actual_res, last, new_value);
}

bool rec2(u64 temp_res, u64 actual_res, u64* last, u64* current_value) {
    if(temp_res > actual_res) return false;
    u64 add_temp_res = temp_res + *current_value;
    u64 mult_temp_res = temp_res * *current_value;
    u64 concat_temp_res = concat(temp_res, *current_value);
    if(current_value == last)
        return actual_res == add_temp_res ||
            actual_res == mult_temp_res ||
            actual_res == concat_temp_res;

    u64* new_value = ++current_value;
    return rec2(add_temp_res, actual_res, last, new_value) ||
        rec2(concat_temp_res, actual_res, last, new_value) ||
        rec2(mult_temp_res, actual_res, last, new_value);
}

void first_part(fstream& file) {
    char ch;
    u64 res = 0;
    u64 res2 = 0;
    u64 test_res = 0;
    u64 current = 0;
    vector<u64> values = {};

    while (file.get(ch)) {
        if(ch == '\n') {
            values.push_back(current);
            if(rec(values[0], test_res, &values[values.size()-1], &values[1])) {
                res += test_res;
                res2 += test_res;
            } else if(rec2(values[0], test_res, &values[values.size()-1], &values[1])) {
                res2 += test_res;
            }
            test_res = 0;
            current = 0;
            values.clear();
        } else if(ch >= '0' && ch <= '9') {
            current = 10*current + (ch - 48);
        } else if(ch == ':') {
            test_res = current;
            current = 0;
        } else if(ch == ' ') {
            if(current != 0) {
                values.push_back(current);
                current = 0;
            }
        }
        else {
            cout << "Unknown char " << ch << endl;
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

