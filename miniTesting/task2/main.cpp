#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iterator>
#include <iomanip>
#include <cmath>

using std::vector;
using std::string;
using std::istringstream;
using std::pair;
using std::for_each;
using std::istream_iterator;
using std::cout;
using std::endl;
using std::cin;
using std::getline;


int main(int argc, char *argv[]){
    string strInput;
    getline(cin, strInput);

    istringstream strStream(strInput);
    vector<pair<string, int>> tokens;
    size_t maxWordLen = 0;
    size_t maxWordWeight = 0;
    auto adderFunc = [&tokens,&maxWordLen, &maxWordWeight](const string &word)
                    {
                        if (!tokens.empty()) {
                            auto it = find_if(tokens.begin(), tokens.end(),
                                              [&word](const auto &pair)
                                              {
                                                  if (word == pair.first) {
                                                      return true;
                                                  }
                                                  return false;
                                              }
                                  );

                            if (it != tokens.end()) {
                                it->second++;
                                if(it->second > maxWordWeight) 
                                     maxWordWeight = it->second;
                                
                                return;
                            }
                        }
                        
                         if(word.length() > maxWordLen)
                             maxWordLen = word.length();
                        
                        tokens.push_back(pair<string, int>(word, 1));
                    };

    for_each(istream_iterator<string>(strStream), istream_iterator<string>(), adderFunc);

    sort(tokens.begin(), tokens.end(),
         [](auto &a, auto &b)
            { return a.second < b.second; }
    );

    for_each(tokens.begin(), tokens.end(),
             [&maxWordLen, &maxWordWeight](const auto &word) {
                 cout<<std::setfill('_')<<std::setw(maxWordLen)<<std::right<<word.first<<" "<<std::setfill('.')<<std::setw(round(((float)word.second/(float)maxWordWeight) * 10))<<std::left<<""<<endl;
             });

    return 0;
}


