#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<queue>
#include<set>
#include<iomanip>
#include <algorithm>
#include <numeric>

using ld = long double;
using namespace std;

string output_string(vector<vector<pair<ld, int>>> result, int n) {
    vector<int> idx(n);
    for (int m=0; m<int(result.size()); m++) {
        for (int i=0; i<int(result[m].size()); i++) {
            idx[result[m][i].second] = m;
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

ld calc_sum(vector<pair<ld, int>> data) {
    ld sum = 0.0;
    for (int j=0; j<data.size(); j++) {
        sum += data[j].first;
    }
    return sum;
}

int main(int args, char *argv[]) {
    int num_groups = atoi(argv[1]);
    ifstream ifs(argv[2]);
    if (ifs.fail()) {
        cerr << "Cannot open file\n";
        exit(0);
    }
    string str;
    priority_queue<tuple<ld, vector<vector<pair<ld, int>>>, vector<ld>>> q;

    ld value;
    int idx = 0;
    vector<ld> data;
    while (getline(ifs, str)) {
        stringstream ss(str);
        ss >> value;
        vector<vector<pair<ld, int>>> part(num_groups); part[0].push_back({value, idx});
        vector<ld> sums(num_groups, 0); sums[0] = value;
        q.push({value, part, sums});
        data.push_back(value);
        idx += 1;
    }

    tuple<ld, vector<vector<pair<ld, int>>>, vector<ld>> first, second;
    while (q.size() != 1) {
        first = q.top(); q.pop();
        second = q.top(); q.pop();

        vector<pair<ld, vector<pair<ld, int>>>> fd(num_groups), sd(num_groups);
        for (int i=0; i<num_groups; i++) {
            fd[i] = {calc_sum(get<1>(first)[i]), get<1>(first)[i]};
            sd[i] = {calc_sum(get<1>(second)[i]), get<1>(second)[i]};
        }
        sort(fd.rbegin(), fd.rend()); sort(sd.begin(), sd.end());
        vector<vector<pair<ld, int>>> part(num_groups); vector<ld> sums(num_groups);
        for (int i=0; i<num_groups; i++) {
            sums[i] = fd[i].first + sd[i].first;
            fd[i].second.insert(fd[i].second.end(), sd[i].second.begin(), sd[i].second.end());
            part[i] = fd[i].second;
        }
        q.push({*max_element(sums.begin(), sums.end()) - *min_element(sums.begin(), sums.end()), part, sums});
    }

    first = q.top();
    cout << "# result: " << fixed << setprecision(20) << get<0>(first) << endl;
    string result = output_string(get<1>(first), data.size());
    cout << result << endl;
    ld diff = reproduce_from_string(result, data, num_groups);
    cout << "# reproduce: " << fixed << setprecision(20) << diff << endl;
}
