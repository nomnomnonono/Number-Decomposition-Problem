#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<queue>
#include<set>
#include<iomanip>
#include <algorithm>

using ld = long double;
using namespace std;

string output_string(vector<vector<int>> result, int n) {
    vector<int> idx(n);
    for (int m=0; m<int(result.size()); m++) {
        for (int i=0; i<int(result[m].size()); i++) {
            idx[result[m][i]] = m;
        }
    }
    string output = "";
    for (int i=0; i<int(idx.size()); i++) {
        if (idx[i] < 10) output += to_string(idx[i]).c_str();
        else output += (char)('A'+idx[i]-10);
    }
    return output;
}

ld reproduce_from_string(string output, vector<ld> data, int num_groups) {
    vector<ld> div(num_groups);
    for (int i=0; i<output.size(); i++) {
        int idx;
        if (output[i] - '0' < 17) idx = output[i] - '0';
        else idx = (output[i] - '0') - 7;
        div[idx] += data[i];
    }
    return *max_element(div.begin(), div.end()) - *min_element(div.begin(), div.end());
}

int main(int args, char *argv[]) {
    int num_groups = atoi(argv[1]);
    ifstream ifs(argv[2]);
    if (ifs.fail()) {
        cerr << "Cannot open file\n";
        exit(0);
    }

    priority_queue<pair<ld, int>> q;
    vector<ld> data;

    string str; ld value; int idx = 0;
    while (getline(ifs, str)) {
        stringstream ss(str);
        ss >> value;
        q.push({value, idx}); data.push_back(value);
        idx += 1;
    }

    vector<ld> sums(num_groups);
    vector<vector<int>> groups(num_groups);

    while (!q.empty()) {
        pair<ld, int> top = q.top(); q.pop();
        int min_idx = 0; ld min_value = 1e9;
        for (int i=0; i<num_groups; i++) {
            if (min_value > sums[i]) {
                min_value = sums[i];
                min_idx = i;
            }
        }
        sums[min_idx] += top.first;
        groups[min_idx].push_back(top.second);
    }

    cout << "# result: " << fixed << setprecision(20) << *max_element(sums.begin(), sums.end()) - *min_element(sums.begin(), sums.end()) << endl;
    string result = output_string(groups, data.size());
    cout << result << endl;
    ld repro = reproduce_from_string(result, data, num_groups);
    cout << "# repro: " << fixed << setprecision(20) << repro << endl;
}
