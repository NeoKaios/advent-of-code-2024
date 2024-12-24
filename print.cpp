#include "type.cpp"
#include <vector>

std::ostream& operator<<(std::ostream& os, const vector<int>& vec) {
    for (const int i : vec) os << i << ' ';
    return os;
}

std::ostream& operator<<(std::ostream& os, const vector<u64>& vec) {
    for (const u64 i : vec) os << i << ' ';
    return os;
}

std::ostream& operator<<(std::ostream& os, const vector<string>& vec) {
    for (const auto i : vec) os << i << ' ';
    return os;
}


std::ostream& operator<<(std::ostream& os, const coord& c) {
    return os << c.first << ' ' << c.second;
}

