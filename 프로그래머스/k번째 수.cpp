#include <algorithm>
#include <vector>
#include<iostream>

using namespace std;

vector<int> solution(vector<int> array, vector<vector<int>> commands) {
    vector<int> answer;
    for (int i = 0; i < commands.size(); i++) {
        vector<int> array_i = array;
        array_i.erase(array_i.begin() + commands[i][1], array_i.end());
        array_i.erase(array_i.begin(), array_i.begin() + commands[i][0] - 1);

        sort(array_i.begin(), array_i.end());
        int ans = array_i[commands[i][2] - 1];
        answer.push_back(ans);
    }

    return answer;
}