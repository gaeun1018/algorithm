#include <string>
#include <vector>

using namespace std;

vector<int> solution(vector<int> lottos, vector<int> win_nums) {
    vector<int> answer;
    int a = 0;
    int zero = 0;
    for (int i = 0; i < 6; i++) {
        if (lottos[i] == 0) {
            zero++;
            continue;
        }
        for (int j = 0; j < 6; j++) {
            if (lottos[i] == win_nums[j]) {
                a++;
                break;
            }
        }
    }
    if (a + zero == 0) //³«¼­°¡ ¾ø°í ³«Ã·ÀÎ °æ¿ì
        answer.push_back(6);
    else
        answer.push_back(7 - a - zero);

    if (a == 0)
        answer.push_back(6);
    else
        answer.push_back(7 - a);
    return answer;
}