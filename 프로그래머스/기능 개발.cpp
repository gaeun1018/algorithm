#include <queue>
#include <vector>
#include<iostream>

using namespace std;

vector<int> solution(vector<int> progresses, vector<int> speeds) {
    vector<int> answer;
    queue<int> s;
    for (int i = 0; i < speeds.size(); i++) {
        s.push(progresses[i]);
    }
    int i = 0;
    while (!s.empty()) {
        i++;
        int num = 0;
        while (true) {
            if (s.front() + i * speeds[speeds.size() - s.size()] >= 100) {
                s.pop();
                num++;
                if (s.empty())
                    break;
            }
            else
                break;
        }
        if (num != 0)
            answer.push_back(num);
    }
    return answer;
}