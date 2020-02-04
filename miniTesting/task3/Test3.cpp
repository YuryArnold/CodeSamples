#include <iostream>
#include <string>
#include <map>
#include <stack>

using namespace std;

bool isOperator(string str){
    
    if (str[0] == '+' || str[0] == '-' || str[0] == '*' || str[0] == '/')
        return true;
    else
        return false;
    
}

int performOperation(int op1, int op2, string str){
    int ans;
    switch(str[0]){
    case '+':
        ans = op2 + op1;
        break;
    case '-':
        ans = op2 - op1;
        break;
    case '*':
        ans = op2 * op1;
        break;
    case '/':
        ans = op2 / op1;
        break;
    default :
        cerr<<"Unknown function"<<endl;
        break;
    }
    return ans;
}



int main(int argc, char** argv) {
    string str;
    stack<int> s;
    while (cin >> str){
        if(isOperator(str)){
            int op1 = s.top();
            s.pop();
            int op2 = s.top();
            s.pop();
            s.push(performOperation(op1, op2,str)); 
        }
        else
            s.push(stoi(str));
    } 
    
    cout<<s.top()<<endl;
     
    return 0;
}


