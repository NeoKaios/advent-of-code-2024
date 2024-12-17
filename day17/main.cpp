#include "../utils.cpp"
#include <cmath>

using namespace std;

class Computer {
private:
    u64 a;
    u64 b;
    u64 c;
    u64 pc = 0;
public:
    vector<char> program = {};
    bool debug = false;
    Computer(vector<u64>& _register, vector<int>& _program) {
        a = _register[0];
        b = _register[1];
        c = _register[2];
        for(auto i : _program) program.push_back((char)i);
    }

    void setA_and_reset(u64 _a) {
        a = _a;
        b = 0;
        c = 0;
        pc = 0;
    }

    char step() {
        char out = '\xfe';
        bool skip_pc = false;
        if(debug) show_state();
        while(out == '\xfe' && pc<program.size()-1) {
            auto opcode = program[pc];
            auto operand = program[pc+1];
            switch(opcode) {
                case 0:
                    a = a/pow(2,combo(operand));
                break;
                case 1:
                    b = b ^ operand;
                break;
                case 2:
                    b = combo(operand)%8;
                break;
                case 3:
                    if(a!=0) {
                        pc = operand;
                        skip_pc = true;;
                    }
                break;
                case 4:
                    b = b^c;
                break;
                case 5:
                    out = (char)(combo(operand)%8);
                break;
                case 6:
                    b = a/pow(2,combo(operand));
                break;
                case 7:
                    c = a/pow(2,combo(operand));
                break;
                default:
                    cout << "ERROR, unkown opcode " << opcode << " at pc " << pc << endl;
                    return '\xff';
            }
            if(skip_pc) skip_pc = false;
            else pc += 2;
            if(debug) show_state();
        }
        return out;
    }

    void run() {
        while(pc<program.size()-1) {
            char out = step();
            if(out == '\xff') cout << "Invalid program" << endl;
            else if(out == '\xfe') cout << endl;
            else cout << (int)out << ',';
        }
        cout << endl;
    }

    u64 combo(char operand) {
        switch(operand) {
            case 4:
            return a;
            case 5:
            return b;
            case 6:
            return c;
            case 7:
            cout << "Reserved" << endl;
            return 0;
            default:
            return (u64)operand;
        }
    }

    void show_state() {
        cout << "PC: " << pc << endl;
        cout << "A: " << a << "  B: " << b << "  C: " << c << endl;
    }
};

void first_part(fstream& file) {
    vector<u64> out = {};
    parse_u64_list(file, "Register A: %", out);
    parse_u64_list(file, "Register B: %", out);
    parse_u64_list(file, "Register C: %", out);
    parse_str(file, "\nProgram: ");
    vector<int> program = {};
    get_int_list(file, ',', program);
    Computer comp(out, program);
    comp.run();
}

u64 find_prev_a(Computer& comp, u64 a, int i) {
    if(i<0) return a;
    if(a>1) a*=8;
    // cout << "Looking for " <<(int) comp.program[i] << " with a=" << a << "  " << i << endl;
    for(u64 na=a; na<a+8; ++na) {
        comp.setA_and_reset(na);
        char out = comp.step();
        // cout << "I=" << i << " A: " << na << " gives " << (int)out << endl;
        if(out == comp.program[i]) {
            u64 res = find_prev_a(comp, na, i-1);
            if(res == 0) continue;
            else return res;
        }
    }
    // cout << "No valid a found for a=" << a << " i=" << i << endl;
    return 0;
}

void second_part(fstream& file) {
    vector<u64> out = {};
    parse_u64_list(file, "Register A: %", out);
    parse_u64_list(file, "Register B: %", out);
    parse_u64_list(file, "Register C: %", out);
    parse_str(file, "\nProgram: ");
    vector<int> program = {};
    get_int_list(file, ',', program);
    Computer comp(out, program);
    // comp.debug = true;
    u64 a = find_prev_a(comp, 1, comp.program.size()-1);

    // Checker
    cout << program << endl;
    comp.setA_and_reset(a);
    comp.run();
    cout << "Answer is: " << a << endl;
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

