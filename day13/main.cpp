#include "../utils.cpp"

using namespace std;

void first_part(fstream& file) {
    char ch;
    u64 res = 0;
    u64 res2 = 0;

    while(!file.eof()) {
        vector<u64> parsing_out = {};
        if(parse_u64_list(file, "Button A: X+%, Y+%", parsing_out)) return;
        if(parse_u64_list(file, "Button B: X+%, Y+%", parsing_out)) return;
        if(parse_u64_list(file, "Prize: X=%, Y=%", parsing_out)) return;
        parse_u64_list(file, "", parsing_out);
        if(parsing_out.size() != 6) {
            cout << "Issue with parsing " << parsing_out << endl;
            return;
        }
        u64 a = parsing_out[0],
            b = parsing_out[2],
            c = parsing_out[1],
            d = parsing_out[3],
            r = parsing_out[4],
            s = parsing_out[5];
        i64 det = a*d-b*c;
        if(det != 0) {
            i64 x = (i64)(d*r-b*s)/det;
            i64 y = (i64)(a*s-c*r)/det;
            if(a*x+b*y == r && c*x+d*y == s) res+= 3*x+y;
            r += 10000000000000;
            s += 10000000000000;
            x = (i64)(d*r-b*s)/det;
            y = (i64)(a*s-c*r)/det;
            if(a*x+b*y == r && c*x+d*y == s) res2+= 3*x+y;
        }
        else cout << "Det is 0" << endl;

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

