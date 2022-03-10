#include <iostream>
#include <deque>
#include <algorithm>

using namespace std;

int main() {
	deque<int> dq;
	int N, M, result = 0;
	cin >> N >> M;
	for (int i = 1; i <= N; i++) {
		dq.push_back(i);
	}
	for (int i = 1; i <= M; i++) {
		int num;
		cin >> num;
		for (int j = 0; j < dq.size(); j++) {
			if (dq[0] == num) {
				dq.pop_front();
			}
			else if (dq[j] == num) {
				int k = dq.size();
				result += min(j, k - j);
				if (j < k - j) {
					for (int a = 0; a < j; a++) {
						dq.push_back(dq.front());
						dq.pop_front();
					}
					dq.pop_front();
				}
				else if (j >= k - j) {
					for (int a = 0; a < k-j; a++) {
						dq.push_front(dq.back());
						dq.pop_back();
					}
					dq.pop_front();
				}
			}
		}
	}
	cout << result;
}