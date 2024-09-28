#ifndef SELECTOR_H
#define SELECTOR_H

#pragma once
#include <utility>
#include <vector>
#include "struct.h"


template<typename R>
class SeleccionarMetodo{
public:
    SeleccionarMetodo(){}

    virtual bool add(R record){
        return false;
    }

    virtual vector<R> load() {
        return vector<R>();
    }

    virtual void display(){
        return;
    }

    virtual void display_all(){
        return;
    }

    virtual vector<R> rangeSearch(char* min, char* max){
        return vector<R>();
    }

    virtual bool remove( char* key ){
        return false;
    }

    virtual pair<R, bool> search(char* key){
        return make_pair(R(), false);
    }
    // ~SeleccionarMetodo(){}

    virtual ~SeleccionarMetodo() {}
};

#endif // SELECTOR_H
