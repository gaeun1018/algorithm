#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

int main() {
    string str;
    cin >> str;
    string* num = new string[str.size()];

    for (int i = 0; i < str.size(); i++)
        num[i] = str[i];
    sort(num, num + str.size(), greater<>());

    for (int i = 0; i < str.size(); i++)
        cout << num[i];
}