#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(string number, int k) {
    string answer = "";
   
    int idx = -1;
    
    for (int i = 0; i <number.size()-k; i++) {
        int max=0;
        for (int j = idx + 1; j <= k+i; j++) {
            if (number[j] - '0' > max) {
                max = number[j] - '0';
                idx = j;
            }
            if (max == 9)
                break;
        }
        answer.append(to_string(max));
    }

    return answer;
}

int main() {
    int k;
    string str;
    cin >> str >> k;
    cout<<solution(str, k);
}