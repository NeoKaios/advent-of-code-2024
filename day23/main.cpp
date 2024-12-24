#include "../utils.cpp"
#include <map>
#include <set>

using namespace std;

struct computer {
    char a;
    char b;
};

struct trio {
    computer a;
    computer b;
    computer c;
};

bool trio_has_t(const trio& t) {
    return t.a.a == 't' || t.b.a == 't' || t.c.a == 't';
}

bool operator==(const computer& c1, const computer& c2) {
    return c1.a == c2.a && c1.b == c2.b;
}

bool operator<(const computer& c1, const computer& c2) {
    return c1.a < c2.a || c1.a == c2.a && c1.b < c2.b;
}

bool operator<(const trio& t1, const trio& t2) {
    return t1.a < t2.a ||
        t1.a == t2.a && t1.b < t2.b ||
        t1.a == t2.a && t1.b == t2.b && t1.c < t2.c;
}

ostream& operator<<(ostream& os, const computer& c) {
    return os << c.a << c.b;
}

ostream& operator<<(ostream& os, const vector<computer>& c) {
    for(const auto co : c)
        os << co << ' ';
    return os;
}

trio create_trio(const computer& x, const computer& y, const computer& z) {
    if(x < y && y < z ) return {x,y,z};
    if(x < z && z < y) return {x,z,y};
    if(y < x && x < z) return {y,x,z};
    if(y < z && z < x) return {y,z,x};
    if(z < x && x < y) return {z,x,y};
    if(z < y && y < x) return {z,y,x};
    cout << "Issue when creating" << endl;
    return {x,y,z};
}

void find_third(const vector<computer>& l1, const vector<computer>& l2, vector<computer>& out) {
    for(auto c: l1) {
        for(auto d: l2) {
            if(c == d) {
                out.push_back(c);
            }
        }
    }
}

bool starts_with_t(const computer& c1) {
    return c1.a == 't';
}

void first_part(fstream& file) {
    char ch;
    u64 res = 0;
    char a = ' ',b = ' ';
    computer c1, c2;
    map<computer, vector<computer>> links= {};

    while (file.get(ch)) {
        if(ch == '\n') {
            c2 = {a,b};
            if(links.count(c1)>0) links[c1].push_back(c2);
            else links[c1] = {c2};

            if(links.count(c2)>0) links[c2].push_back(c1);
            else links[c2] = {c1};
            a = ' ';
            b = ' ';
        } else if(ch == '-') {
            c1 = {a,b};
            a = ' ';
            b = ' ';
        } else {
            if(a == ' ') a = ch;
            else b = ch;
        }
    }

    set<trio> trios = {};

    for(auto current: links) {
        for(auto linked: current.second) {
            vector<computer> third = {};
            find_third(links[linked], current.second, third);
            for(auto t: third) {
                trios.insert(create_trio(current.first, linked, t));
            }
        }
    }

    cout << "Trios: " << trios.size() << endl;
    for(auto trio: trios) {
        if(trio_has_t(trio)) ++res;
    }

    cout << "Answer is: " << res << endl;


    set<set<computer>> previous_group = {};
    set<set<computer>> size_up = {};
    for(auto trio: trios) {
        size_up.insert({trio.a, trio.b, trio.c});
    }
    cout << "Transition finished" << endl;
    while(size_up.size()!=1) {
        previous_group = size_up;
        size_up = {};
        for(auto current: previous_group) {
            auto comp = current.extract(current.begin()).value();
            for(auto other: links[comp]) {
                set<computer> new_set = current;
                new_set.insert(other);
                if(new_set.size() > current.size() &&
                    previous_group.count(new_set)>0) {
                    new_set.insert(comp);
                    size_up.insert(new_set);
                }
            }
        }
        cout << "size_up size: " << size_up.size() << " with sets of size: " << size_up.begin()->size() << endl;
    }
    for(auto s: size_up) {
        for(auto c: s) {
            cout << c << ',';
        }
        cout << endl;
    }


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

