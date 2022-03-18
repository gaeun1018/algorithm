#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solution(int n, vector<int> lost, vector<int> reserve) {
    int answer = 0;
    answer = n - lost.size();
    sort(lost.begin(), lost.end());
    sort(reserve.begin(), reserve.end());
    bool* res = new bool[reserve.size()];
    for (int i = 0; i < reserve.size(); i++) {
        res[i] = true;
    }
    bool* los = new bool[lost.size()];
    for (int i = 0; i < lost.size(); i++) {
        los[i] = true;
    }
    for (int i = 0; i < lost.size(); i++) {
        for (int j = 0; j < reserve.size(); j++) {
            if (lost[i] == reserve[j]) {
                los[i] = false;
                res[j] = false;
                answer++;
                break;
            }
        }
    }
    for (int i = 0; i < lost.size(); i++) {
        for (int j = 0; j < reserve.size(); j++) {
            if (abs(lost[i] - reserve[j]) == 1) {
                if (res[j] == true and los[i] == true) {
                    res[j] = false;
                    answer++;
                    break;
                }
            }
        }
    }
    delete res;
    delete los;
    return answer;
}