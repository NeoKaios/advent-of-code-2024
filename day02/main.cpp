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

bool check_safe(list<int> report) {
    int prev = -1;
    int direction = 0;
    bool safe = true;
    for(const int level : report) {
        if(prev < 0) {
            prev = level;
            continue;
        } else if (abs(level - prev) > 3 || level == prev) {
            safe = false;
            break;
        } else if (direction == 0) {
            direction = (level < prev) ? -1 : 1;
        } else if (direction != ((level < prev) ? -1 : 1)) {
            safe = false;
            break;
        }
        prev = level;
    }
    return safe;
}

bool check_safe_with_dampener(list<int> report) {
    // cout << report << endl;
    if(check_safe(report)) return true;
    for(int skipped_idx = 0; skipped_idx < report.size(); ++skipped_idx) {
        int idx = -1;
        int prev = -1;
        bool safe = true;
        int direction = 0;
        for(const int level : report) {
            ++idx;
            // cout << "inside 2nd loop " << idx << " " << skipped_idx << endl;
            if(idx == skipped_idx)
                continue;
            if(prev < 0) {
                prev = level;
                continue;
            } else if (abs(level - prev) > 3 || level == prev) {
                safe = false;
                break;
            } else if (direction == 0) {
                direction = (level < prev) ? -1 : 1;
            } else if (direction != ((level < prev) ? -1 : 1)) {
                safe = false;
                break;
            }
            prev = level;
        }
        if(safe) return true;
    }
    return false;
}

void first_part(fstream& file) {
    char ch;
    int current = 0;
    int safe_count = 0;
    list<int> report {};

    while (!file.eof()) {
        file >> noskipws >> ch;

        if(ch == '\n') {
            if(current == 0) {
                break;
            }
            report.push_back(current);
            current = 0;
            if(check_safe(report)) safe_count++;
            report.clear();
        } else if(ch >= '0' && ch <= '9') {
            current = 10*current + (ch - 48);
        } else if (ch == ' ') {
            report.push_back(current);
            current = 0;
        }
        else {
            cout << "Unknown char (" << ch << ")" << endl;
        }
    }
    cout << "Answer is: " << safe_count << endl;
}

void second_part(fstream& file) {

    char ch;
    int current = 0;
    int safe_count = 0;
    list<int> report {};

    while (!file.eof()) {
        file >> noskipws >> ch;

        if(ch == '\n') {
            if(current == 0) {
                break;
            }
            report.push_back(current);
            current = 0;

            for(int skipped_idx = -1; skipped_idx < report.size(); ++skipped_idx) {
                cout << skipped_idx << endl;
            }
            if(check_safe_with_dampener(report)) safe_count++;
            report.clear();
        } else if(ch >= '0' && ch <= '9') {
            current = 10*current + (ch - 48);
        } else if (ch == ' ') {
            report.push_back(current);
            current = 0;
        }
        else {
            cout << "Unknown char (" << ch << ")" << endl;
        }
    }
    cout << "Answer is: " << safe_count << endl;
}

int main() {
    fstream file;
    file.open("input.txt", ios::in);
    // file.open("example.txt", ios::in);
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

