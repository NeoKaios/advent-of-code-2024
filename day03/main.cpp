#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

void first_part(fstream& file) {
    char ch;
    int res = 0;
    int a = 0;
    int b = 0;
    char in_seq = ' ';

    while (file.get(ch)) {
        if (
            ch == 'm' ||
            in_seq == 'm' && ch == 'u' ||
            in_seq == 'u' && ch == 'l' ||
            in_seq == 'l' && ch == '(') {
            in_seq = ch;
        } else if (in_seq == '(' && ch == ',' && a > 0) {
            in_seq = ',';
        } else if(ch >= '0' && ch <= '9') {
            if(in_seq == '(') {
                a = 10*a + (ch - 48);
            } else if(in_seq == ','){
                b = 10*b + (ch - 48);
            }
        } else if (in_seq == ',' && ch == ')' && b > 0) {
            if(a < 1000 && b < 1000) {
                res += a*b;
            } else {
                cout << "Bigger than 1000: " << a << ' ' << b << endl;
            }
            a = 0;
            b = 0;
            in_seq = ' ';
        } else {
            in_seq = ' ';
            a = 0;
            b = 0;
        }
    }
    cout << "Answer is: " << res << endl;
}
void second_part(fstream& file) {
    char ch;
    int res = 0;
    int a = 0;
    int b = 0;
    char in_seq = ' ';
    char current = ' ';
    bool enable = true;

    while (file.get(ch)) {
        if(enable) {
            if(ch == 'd' || ch == 'm') {
                current = ch;
            }
            if (current == 'd') {
                if( ch == 'd' ||
                    in_seq == 'd' && ch == 'o' ||
                    in_seq == 'o' && ch == 'n' ||
                    in_seq == 'n' && ch == '\'' ||
                    in_seq == '\'' && ch == 't' ||
                    in_seq == 't' && ch == '(') {
                    in_seq = ch;
                } else if (in_seq == '(' && ch == ')') {
                    enable = false;
                    in_seq = ' ';
                    current = ' ';
                } else {
                    in_seq = ' ';
                    current = ' ';
                }
            }
            if(current == 'm') {
                if (
                    ch == 'm' ||
                    in_seq == 'm' && ch == 'u' ||
                    in_seq == 'u' && ch == 'l' ||
                    in_seq == 'l' && ch == '(') {
                    in_seq = ch;
                } else if (in_seq == '(' && ch == ',' && a > 0) {
                    in_seq = ',';
                } else if(ch >= '0' && ch <= '9') {
                    if(in_seq == '(') {
                        a = 10*a + (ch - 48);
                    } else if(in_seq == ','){
                        b = 10*b + (ch - 48);
                    }
                } else if (in_seq == ',' && ch == ')' && b > 0) {
                    if(a < 1000 && b < 1000) {
                        res += a*b;
                    } else {
                        cout << "bigger than 1000: " << a << ' ' << b << endl;
                    }
                    a = 0;
                    b = 0;
                    in_seq = ' ';
                    current = ' ';
                } else {
                    in_seq = ' ';
                    current = ' ';
                    a = 0;
                    b = 0;
                }
            }
        }
        else {
            if (
                ch == 'd' ||
                in_seq == 'd' && ch == 'o' ||
                in_seq == 'o' && ch == '(') {
                in_seq = ch;
            } else if (in_seq == '(' && ch == ')') {
                enable = true;
            } else {
                in_seq = ' ';
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
    else
        file.open("input.txt", ios::in);
    if (!file) {
        cout << "No such file";
        return 1;
    }
    file.clear();
    file.seekg(0, ios::beg);
    first_part(file);
    file.clear();
    file.seekg(0, ios::beg);
    second_part(file);
    file.close();
    return 0;
}

