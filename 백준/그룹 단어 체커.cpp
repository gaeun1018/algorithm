#include <iostream>
#include <string>

using namespace std;

int main(){
    int num;
    cin>>num;
    int result=0;
    for(int i=0;i<num;i++){
        string str;
        cin>>str;
        for(int j=0;j<str.size();j++){
            for(int k=j+1;k<str.size();k++){
                if(str[j]==str[k] and str[k]!=str[k-1])
                    break;
                else
                    result++;
            }
        }   
    }
    cout<<result;
}