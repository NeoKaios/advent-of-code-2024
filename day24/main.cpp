#include "../utils.cpp"
#include <map>
#include <vector>
#include <set>

using namespace std;

enum gate_type {
    AND,
    XOR,
    OR
};

std::ostream& operator<<(std::ostream& os, const gate_type& gt) {
    switch(gt) {
        case AND:
            os << "AND";
            break;
        case OR:
            os << "OR";
            break;
        case XOR:
            os << "XOR";
            break;
    }
    return os;
}

struct gate {
    string a;
    string b;
    enum gate_type gt;
    string out;
    bool solved;
};

bool compute_gate(const gate& g, map<string, bool>& state) {
    bool out;
    switch(g.gt) {
        case AND:
            out = state[g.a] && state[g.b];
            break;
        case OR:
            out = state[g.a] || state[g.b];
            break;
        case XOR:
            out = state[g.a] ^ state[g.b];
            break;
    }
    state[g.out] = out;
    return out;
}

std::ostream& operator<<(std::ostream& os, const gate& g) {
    return os << g.a << ' ' << g.gt << ' '<< g.b << " -> " << g.out;
}

bool operator==(const gate& g1, const gate& g2) {
    return g1.a.compare(g2.a) == 0 &&
        g1.gt == g2.gt &&
        g1.b.compare(g2.b) == 0 &&
        g1.out.compare(g2.out) == 0;

}

u64 solve_gates(vector<gate>& gates, map<string, bool>& state) {
    u64 res = 0;
    u64 mult = 1;
    bool flag = true;
    while(flag) {
        flag = false;
        for(auto& g: gates) {
            if(g.solved) continue;
            if(state.count(g.a)>0 && state.count(g.b)>0) {
                compute_gate(g, state);
                g.solved = true;
                if(g.out[0] == 'z') {
                    int rank = (g.out[1] -48)*10 + g.out[2] - 48;
                    res |= (u64)state[g.out] << rank;
                }
                flag = true;
            }
        }
    }
    return res;
}

u64 solve_gates_n_test(vector<gate>& gates, map<string, bool>& state, u64 z) {
    u64 res = 0;
    u64 mult = 1;
    bool flag = true;
    while(flag) {
        flag = false;
        for(auto& g: gates) {
            if(g.solved) continue;
            if(state.count(g.a)>0 && state.count(g.b)>0) {
                bool val = compute_gate(g, state);
                g.solved = true;
                if(g.out[0] == 'z') {
                    int rank = (g.out[1] -48)*10 + g.out[2] - 48;
                    // cout << g.out << endl;
                    if(val != (z & (u64)1 << rank)) {
                        // cout << "Failing " << val << "!="  << (z & (u64)1 << rank) << endl;
                        return false;
                    }
                }
                flag = true;
            }
        }
    }
    return true;
}

void debug_print(map<string, bool> state) {
    string x = "x00";
    string y = "y00";
    string z = "z00";
    int i = 0;
    while(state.count(x)>0) {
        cout << x << "  " << state[x] << '+' << state[y] << "=" << state[z] << endl;
        ++i;
        string digits = "";
        digits = digits + (char)(i/10+48) + (char)((i%10)+48);
        x = "x" + digits;
        y = "y" + digits;
        z = "z" + digits;
    }
}

void reset_state(map<string,bool>& state, u64 x, u64 y) {
    map<string,bool> new_state = {};
    for(auto s: state) {
        string cable = s.first;
        if(cable[0] == 'x' || cable[0] == 'y') {
            int rank = (cable[1] -48)*10 + cable[2] - 48;
            new_state[cable] = (cable[0] == 'x' ? x : y) & ((u64)1 << rank);
        }
    }
    state = new_state;
}

void reset_gates(vector<gate>& gates) {
    for(auto& g: gates) {
        g.solved = false;
    }
}

set<string> swapped = {};
void swap_out(vector<gate>& gates, int a, int b) {
    string outb = gates[b].out;
    gates[b].out = gates[a].out;
    gates[a].out = outb;
    cout << "Swapped: " << gates[a].out << ' ' << gates[b].out << endl;
    swapped.insert(gates[a].out);
    swapped.insert(gates[b].out);
}

void backtrack(map<string, gate>& backtracker, string cable, set<string>& hierarchy) {
    if(cable[0] == 'x' || cable[0] == 'y') {
        hierarchy.insert(cable);
        return;
    }
    backtrack(backtracker, backtracker[cable].a, hierarchy);
    backtrack(backtracker, backtracker[cable].b, hierarchy);
}


int find_gate_idx(vector<gate>& gates, gate_type gt, string cable) {
    for(int i=0; i<gates.size(); ++i) {
        gate g = gates[i];
        if(g.gt == gt && (g.a.compare(cable) == 0 || g.b.compare(cable) == 0)) {
            return i;
        }
    }
    // cout << gt << ' ' << cable << endl;
    return -1;
    // throw invalid_argument("No matching gate");
}

gate find_gate(vector<gate>& gates, gate_type gt, string cable) {
    return gates[find_gate_idx(gates, gt, cable)];
}

struct full_adder {
    gate prev_and;
    gate carry_and;
    gate carry_or;
    gate current_xor;
    gate full_xor;
};

std::ostream& operator<<(std::ostream& os, const full_adder& fa) {
    os << fa.prev_and << endl;
    os << fa.carry_and << endl;
    os << fa.carry_or << endl;
    os << fa.current_xor << endl;
    os << fa.full_xor << endl << endl;
    return os;
}

void first_part(fstream& file) {
    char ch;
    u64 res = 0;

    map<string, bool> state = {};
    vector<gate> gates = {};
    map<string, gate> backtracker = {};

    while (!file.eof()) {
        vector<string> out = {};
        get_string_list(file, ": ", out);
        if(out.size() == 1) break;
        state[out[0]] = out[1].compare("1") == 0;
    }
    while (!file.eof()) {
        vector<string> out = {};
        get_string_list(file, " ", out);
        if(out.size() != 5) break;
        gate_type gt;
        gt = out[1].compare("AND") == 0 ? AND :
            out[1].compare("OR") == 0 ? OR : XOR;
        gates.push_back({out[0], out[2], gt, out[4], false});
        backtracker[out[4]] = gates.back();
    }
    res = solve_gates(gates, state);
    cout << "Answer is: " << res << endl;

    u64 x = ((u64)1<<50)-1;
    u64 y = 0;
    int nb_g = gates.size();

    vector<full_adder> adder_history = {};
    adder_history.push_back({});

    full_adder add01;
    add01.prev_and = find_gate(gates, AND, "x00");
    add01.carry_or = find_gate(gates, AND, "x00");
    add01.current_xor = find_gate(gates, XOR, "x01");
    add01.full_xor = backtracker["z01"];
    adder_history.push_back(add01);
    full_adder previous_fa = adder_history.back();

    int i = 1;
    int limit = 45;
    while(i<limit) {
        ++i;
        string digits = "";
        digits = digits + (char)(i/10+48) + (char)((i%10)+48);
        cout << digits << endl;
        full_adder fa;
        int prev_and_idx = find_gate_idx(gates, AND, previous_fa.current_xor.a);
        int carry_and_idx = find_gate_idx(gates, AND, previous_fa.carry_or.out);
        int current_xor_idx = find_gate_idx(gates, XOR, 'x'+digits);
        fa.prev_and = gates[prev_and_idx];
        fa.carry_and = gates[carry_and_idx];
        fa.current_xor = gates[current_xor_idx];
        int or_1 = find_gate_idx(gates, OR, fa.prev_and.out);
        int or_2 = find_gate_idx(gates, OR, fa.carry_and.out);
        if(or_1 == or_2) {
            fa.carry_or = gates[or_1];
        } else {
            cout << "or issue" << endl;
            break;
        }
        int idx_1 = find_gate_idx(gates, XOR, fa.carry_or.out);
        int idx_2 = find_gate_idx(gates, XOR, fa.current_xor.out);
        gate gate_zout = backtracker["z"+digits];
        int idx_3 = find_gate_idx(gates, gate_zout.gt, gate_zout.a);
        if(idx_1 == idx_2 && idx_1 >= 0) {
            if(idx_1 == idx_3) {
                fa.full_xor = gates[idx_1];
            } else {
                swap_out(gates, idx_1, idx_3);
                fa.full_xor = gates[idx_1];
                backtracker[gates[idx_1].out] = gates[idx_1];
                backtracker[gates[idx_3].out] = gates[idx_3];
            }
        } else if ((idx_1 == -1) != (idx_2 == -1)) {
            if(idx_1 == -1) {
                if(idx_2 != idx_3) {
                    cout << "xor issue 2" << endl;
                    break;
                }
                fa.full_xor = gate_zout;
                string correct_cable =
                    fa.full_xor.a.compare(fa.current_xor.out) == 0 ?
                    fa.full_xor.b : fa.full_xor.a;
                gate gate_w_cable = backtracker[correct_cable];
                int idx_to_swap = find_gate_idx(gates, gate_w_cable.gt, gate_w_cable.a);
                swap_out(gates, idx_to_swap, or_1);
                fa.carry_or = gates[or_1];
            } else {
                if(idx_1 != idx_3) {
                    cout << "xor issue 2" << endl;
                    break;
                }
                fa.full_xor = gate_zout;
                string correct_cable =
                    fa.full_xor.a.compare(fa.carry_or.out) == 0 ?
                    fa.full_xor.b : fa.full_xor.a;
                gate gate_w_cable = backtracker[correct_cable];
                int idx_to_swap = find_gate_idx(gates, gate_w_cable.gt, gate_w_cable.a);
                swap_out(gates, idx_to_swap, current_xor_idx);
                fa.current_xor = gates[current_xor_idx];
            }
        } else {
            cout << "xor issue" << endl;
            break;
        }
        adder_history.push_back(fa);
        previous_fa = fa;
        // cout << fa;
    }
    for(auto s: swapped) {
        cout << s << ',';
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

