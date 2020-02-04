/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Hash.h
 * Author: yury
 *
 * Created on April 4, 2017, 8:49 AM
 */

#ifndef HASH_H
#define HASH_H
#include <iostream>
#include "List.h"

class Hash{
    IntList* mas;
    size_t Numb;
    size_t count;
    
public:
    Hash(size_t Number);
    ~Hash();
    void setHash(size_t data);
    void getHash();
    
};


#endif /* HASH_H */

