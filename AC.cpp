#include <iostream>
#include <deque>
#include <string>

using namespace std;

int main() {
	int num;
	cin >> num;
	for (int i = 0; i < num; i++) {
		string RD;
		cin >> RD;

		bool rev = false;
		bool error = false;
		deque<string> dq;

		int n;
		cin >> n;
		string a;
		if (n == 0)
			cin >> a;
		cin.ignore();
		for (int j = 0; j < n; j++) {
			if (j != n - 1)
				getline(cin, a, ',');
			else
				cin >> a;
			if (j == 0)
				a.erase(0,1);
			if (j == n - 1)
				a.erase(a.size()-1,1);
			dq.push_back(a);
		}

		for (int j = 0; j < RD.size(); j++) {
			if (RD[j] == 'R') {
				rev = !rev;
			}
			else {
				if (dq.size() == 0) {
					cout << "error" << endl;
					error = true;
					break;
				}
				else if(rev)
				{
					dq.pop_back();
				}
				else if (!rev) {
					dq.pop_front();
				}
			}
		}
		if (error) {
			continue;
		}
		int size = dq.size();
		cout << '[';
		if (n == 0 or size==0){
			cout << "]" << endl;
			continue;
		}

		if (rev) {
			for (int j = 0; j < size - 1; j++) {
				cout << dq.back() << ',';
				dq.pop_back();
			}
		}
		else {
			for (int j = 0; j < size - 1; j++) {
				cout << dq.front() << ',';
				dq.pop_front();
			}
		}
		
		cout << dq.back() << ']' << endl;
	}
}