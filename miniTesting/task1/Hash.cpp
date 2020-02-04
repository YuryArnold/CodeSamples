#include "Hash.h"

Hash::Hash(size_t Number){
    Numb = Number;
    /*Allocation memory for hash item*/
    mas = new IntList[Numb];
    
}

Hash::~Hash(){
    delete [] mas;
}

void Hash::setHash(size_t data){
   size_t ind = data % Numb;
   mas[ind].AddLast(data);
}

void Hash::getHash(){
    for(size_t index = 0;index < Numb; ++index){
         cout<<index<<": ";
             if (!mas[index].GetItemsCount())
                 cout<<endl;
             else
                 cout<<mas[index].GetAllItemsInfo();
    }
}