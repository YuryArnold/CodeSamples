#ifndef LIST_H
#define LIST_H

#include <fstream>
#include <string>
#include <sstream> 

using namespace std;

class IntList {
private:

    struct ListItem {
        int item;
        ListItem* next;

        ListItem(int i, ListItem* n = NULL) {
            item = i;
            next = n;
        }
    };

    int itemsCount;
    ListItem* first;
    ListItem* last;

public:

    IntList() {
        itemsCount = 0;
        first = last = NULL;
    }
    ~IntList();
    
    void AddFirst(int item);
    void AddLast(int item);

    int GetItemsCount() {
        return itemsCount;
    }

    string GetAllItemsInfo();

};

#endif

