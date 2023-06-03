#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<queue>
#include<set>
#include<iomanip>

using namespace std;

void create(set<int> &zero, set<int> &one, int idx, vector<int> row, queue<pair<int, vector<int>>> &buf) {
    if (zero.count(idx)) {
        for (auto x: row) {
            one.insert(x);
        }
    }
    else if (one.count(idx)) {
        for (auto x: row) {
            zero.insert(x);
        }
    }
    else {
        int which = -1;
        for (auto x: row) {
            if (zero.count(x)) {
                which = 0;
                break;
            }
            else if (one.count(x)) {
                which = 1;
                break;
            }
            else continue;
        }
        if (which == 0) {
            one.insert(idx);
            for (auto x: row) {
                zero.insert(x);
            }
        }
        else if (which == 1) {
            zero.insert(idx);
            for (auto x: row) {
                one.insert(x);
            }
        }
        else {
            buf.push({idx, row});
        }
    }
}

double reproduce(set<int> zero, set<int> one, vector<double> data) {
    double sum_zero, sum_one;
    for (auto itr=zero.begin(); itr!=zero.end(); ++itr) {
        sum_zero += data[*itr];
    }
    for (auto itr=one.begin(); itr!=one.end(); ++itr) {
        sum_one += data[*itr];
    }
    return abs(sum_zero - sum_one);
}

string output_string(set<int> zero, set<int> one, int n) {
    string output = "";
    for (int i=0; i<n; i++) {
        output += '_';
    }
    for (auto itr=zero.begin(); itr!=zero.end(); ++itr) {
        output[*itr] = '0';
    }
    for (auto itr=one.begin(); itr!=one.end(); ++itr) {
        output[*itr] = '1';
    }
    return output;
}

double reproduce_from_string(string output, vector<double> data) {
    double sum_zero, sum_one;
    for (int i=0; i<output.size(); i++) {
        if (output[i] == '0') sum_zero += data[i];
        else sum_one += data[i];
    }
    return abs(sum_zero - sum_one);
}

int main(int args, char *argv[]) {
    ifstream ifs(argv[1]);
    if (ifs.fail()) {
        cerr << "Cannot open file\n";
        exit(0);
    }
    string str;
    priority_queue<pair<double, int>> q;
    double tmp;
    int idx = 0;
    vector<double> data;
    while (getline(ifs, str)) {
        stringstream ss(str);
        ss >> tmp;
        q.push({tmp, idx});
        data.push_back(tmp);
        idx += 1;
    }
    pair<double, int> first, second;
    vector<vector<int>> table(q.size());
    while (q.size() != 1) {
        first = q.top(); q.pop();
        second = q.top(); q.pop();
        q.push({first.first - second.first, first.second});
        table[first.second].push_back(second.second);
        table[second.second].push_back(first.second);
    }
    queue<pair<int, vector<int>>> buf;
    set<int> zero, one;
    zero.insert(0);
    for (auto x: table[0]) {
        one.insert(x);
    }
    for (int i=1; i<table.size(); i++) {
        create(zero, one, i, table[i], buf);
    }
    while (!buf.empty()) {
        pair<int, vector<int>> tmp = buf.front();
        buf.pop();
        create(zero, one, tmp.first, tmp.second, buf);
    }
    cout << "# result: " << fixed << setprecision(20) << q.top().first << endl;
    cout << "# " << zero.size() << " " << one.size() << endl;
    double repro = reproduce(zero, one, data);
    cout << "# repro: " << fixed << setprecision(20) << repro << endl;
    string output = output_string(zero, one, data.size());
    double repro_str = reproduce_from_string(output, data);
    cout << "# repro from str: " << fixed << setprecision(20) << repro_str << endl;
    cout << output << endl;
}
