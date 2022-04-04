#include <iostream>
#include <string>

using namespace std;

int main(){
    int num;
    cin>>num;
    int result=num;
    for(int i=0;i<num;i++){
        string str;
        cin>>str;
        bool br=true;
        for(int j=0;j<str.size()-1;j++){
            if(br){
                for(int k=j+1;k<str.size();k++){
                    if(str[j]==str[k] and str[k]!=str[k-1]){
                        result--;
                        br=false;
                        break;
                    }
                }
            }
            
        }
    }
    cout<<result;
}
