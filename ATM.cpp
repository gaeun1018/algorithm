#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

int main() {
	int num;
	cin >> num;
	int* result=new int[num];
	for (int i = 0; i < num; i++) {
		int n;
		cin >> n;
		result[i] = n;
	}
	sort(result, result + num);

	int cnt = 0;

	for (int i = 0; i < num; i++) {
		cnt += result[i] * (num - i);
	}

	cout << cnt;
	delete[] result;
}