#include "../utils.cpp"

using namespace std;

using u64 = unsigned long long;

void first_part(fstream& file) {
    char ch;
    u64 res = 0;

    while (file.get(ch)) {
        if(ch == '\n') {
        } else if(ch >= '0' && ch <= '9') {
        }
        else {
            cout << "Unknown char " << ch << endl;
        }
    }
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

