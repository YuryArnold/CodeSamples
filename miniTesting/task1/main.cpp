#include <cstdlib>
#include <iostream>
#include "Hash.h"

using namespace std;

int main(int argc, char const *argv[]){
    string N;
    cin >> N;
    Hash hs(stoi(N));
    int i = 0;
    while(cin >> N){
        hs.setHash(stoi(N));
    }
    //cout<<'\n';
    hs.getHash();
    
	return 0;
}