#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

std::ostream& operator<<(std::ostream& ostr, const std::vector<char>& vec)
{
    for (const int i : vec)
        ostr << ' ' << i;
    return ostr;
}

std::ostream& operator<<(std::ostream& ostr, const std::vector<vector<char>>& vec)
{
    for (const vector<char> v : vec)
        ostr << v << endl;
    return ostr;
}

void first_part(fstream& file) {
    char ch;
    int res1 = 0;
    int res2 = 0;
    vector<vector<char>> rules(100);
    for(auto& rule: rules) rule = {};

    vector<char> updates = {};

    char a = 0;
    char b = 0;
    char current = 0;

    bool at_section_rules = true;

    while (file.get(ch)) {
        if(at_section_rules) {
            if(ch == '\n') {
                if(a == 0) {
                    at_section_rules = false;
                    continue;
                }
                rules[a].push_back(current);
                a = 0;
                current = 0;
            } else if(ch >= '0' && ch <= '9') {
                current = 10*current + (ch - 48);
            } else if(ch == '|') {
                a = current;
                current = 0;
            }
            else {
                cout << "Unknown char " << ch << endl;
            }
        } else {
            if(ch == '\n') {
                if(current == 0) {
                    continue;
                }
                updates.push_back(current);
                current = 0;
                bool valid = true;

                for(std::size_t i = 0; i < updates.size(); ++i) {
                    for(std::size_t j = 0; j < i; ++j) {
                        int c = count(rules[updates[i]].begin(), rules[updates[i]].end(), updates[j]);
                        if(c > 0) {
                            valid = false;
                            goto end_of_for;
                        }
                    }
                }
                end_of_for:
                if(valid) {
                    // cout << updates << endl;
                    res1 += updates[(updates.size()-1)/2];
                } else {
                    for(std::size_t i = 0; i < updates.size(); ++i) {
                        int rank = 0;
                        for(std::size_t j = 0; j < updates.size(); ++j) {
                            if(j == i) continue;
                            int c = count(rules[updates[i]].begin(), rules[updates[i]].end(), updates[j]);
                            rank += c;
                        }
                        if(rank == (updates.size()-1)/2) {
                            res2 += updates[i];
                            goto end;

                        }
                    }
                }
                end:
                updates.clear();
            } else if(ch >= '0' && ch <= '9') {
                current = 10*current + (ch - 48);
            } else if(ch == ',') {
                updates.push_back(current);
                current = 0;
            }
            else {
                cout << "Unknown char " << ch << endl;
            }
        }
    }
    // cout << rules << endl;
    cout << "Answer is: " << res1 << endl;
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

