#include <vector>
#include <iostream>
#include <cmath>

using namespace std;
bool sosu(int num) {
    if (num < 2) return false;
    int a = (int)sqrt(num);
    for (int i = 2; i <= a; i++) if (num % i == 0) return false;
    return true;
}

int solution(vector<int> nums) {
    int answer = 0;
    int size = nums.size();
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            for (int k = j + 1; k < size; k++) {
                if (sosu(nums[i] + nums[j] + nums[k]) == true) {
                    answer++;
                }
            }
        }
    }
    return answer;
}