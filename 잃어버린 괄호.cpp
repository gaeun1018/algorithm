#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

int main() {
	string sen;
	cin >> sen;
	string list[50];
	string a = "";

	int result=0;
	bool minus = true;

	for (int i = 0; i < sen.size(); i++) {
		if (sen[i] == '+' or i ==sen.size()-1) {
			if(i == sen.size() - 1)
				a.append(1, sen[i]);
			if (minus)
				result += stoi(a);
			else
				result -= stoi(a);
			a.clear();
		}
		else if (sen[i] == '-') {
			if (minus)
				result += stoi(a);
			else
				result -= stoi(a);
			a.clear();
			minus = false;
		}
		else {
			a.append(1,sen[i]);
		}
	}
	cout << result;
}