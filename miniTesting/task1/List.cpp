/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "List.h"

IntList::~IntList() {
    ListItem* current = NULL;
    ListItem* next = first;
    while (next) {
        current = next;
        next = next->next;
        delete current;
    }
}


void IntList::AddFirst(int item) {
    ListItem* newItem = new ListItem(item, first);
    if (!first) {
        last = newItem;
    }
    first = newItem;
    itemsCount++;
}

void IntList::AddLast(int Item) {
    ListItem* newItem = new ListItem(Item);
    if (!last) {
        first = newItem;
    } else {
        last->next = newItem;
    }
    last = newItem;
    itemsCount++;
}

string IntList::GetAllItemsInfo() {
    stringstream stream; 
    ListItem* current = first;
    while (current) {
        stream << current->item << ' ';
        current = current->next;
    }
    stream << endl;
    return stream.str();
}