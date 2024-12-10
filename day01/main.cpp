#include <iostream>
#include <fstream>
#include <list>

using namespace std;

std::ostream& operator<<(std::ostream& ostr, const std::list<int>& list)
{
    for (const int i : list)
        ostr << ' ' << i;
    return ostr;
}

void first_part(fstream& file) {
    list<int> l1, l2;
    char ch;
    int current = 0;

    while (!file.eof()) {
        file >> noskipws >> ch;

        if(ch == '\n') {
            if (current != 0) {
                l2.push_back(current);
                current = 0;
            }
        } else if(ch >= '0' && ch <= '9') {
            current = 10*current + (ch - 48);
        } else if (ch == ' ') {
            if (current != 0) {
                l1.push_back(current);
                current = 0;
            }
        }
        else {
            cout << "Unknown char " << ch << endl;
        }
    }
    l1.sort();
    l2.sort();
    int sum = 0;
    for (const int i1 : l1) {
        int i2 = l2.front();
        l2.pop_front();
        sum += abs(i2 - i1);
    }
    cout << "Answer is: " << sum << endl;
}

void second_part(fstream& file) {
    list<int> l1, l2;
    char ch;
    int current = 0;

    while (!file.eof()) {
        file >> noskipws >> ch;

        if(ch == '\n') {
            if (current != 0) {
                l2.push_back(current);
                current = 0;
            }
        } else if(ch >= '0' && ch <= '9') {
            current = 10*current + (ch - 48);
        } else if (ch == ' ') {
            if (current != 0) {
                l1.push_back(current);
                current = 0;
            }
        }
        else {
            cout << "Unknown char " << ch << endl;
        }
    }
    l1.sort();
    l2.sort();
    auto it_l2 = l2.begin();
    int sum = 0;
    int current_score = 0;
    int past_i = -1;
    for(const int i1 : l1) {
        if(past_i == i1) {
            sum += current_score;
            continue;
        }
        current_score = 0;
        past_i = i1;
        if(*it_l2 > i1) {
            continue;
        }
        while(*it_l2 < i1 && it_l2 != l2.end()) {
            it_l2++;
        }
        while(*it_l2 == i1 && it_l2 != l2.end()) {
            current_score += i1;
            it_l2++;
        }
        sum += current_score;
    }
    cout << "Answer is: " << sum << endl;
}

int main() {
    fstream file;
    // file.open("example.txt", ios::in);
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

