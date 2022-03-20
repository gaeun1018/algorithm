#include <algorithm>
#include <vector>

using namespace std;

int solution(vector<int> people, int limit) {
    sort(people.begin(), people.end());
    int answer = 0;
    bool* peo = new bool[people.size()];
    for (int i = 0; i < people.size(); i++) {
        peo[i] = true;
    }
    for (int i = 0; i < people.size(); i++) {
        int a = 0;
        if (peo[i]) {
            peo[i] = false;
            a += people[i];
            for (int j = people.size() - 1; j > i; j--) {
                if (a + people[j] <= limit and peo[j]) {
                    peo[j] = false;
                    a += people[j];
                }
            }
            answer++;
            if (peo == false)
                break;
        }
    }
    delete peo;
    return answer;
}