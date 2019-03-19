//
// Created by tejun on 3/19/2019.
//

#include "db_tuple.h"

//////// Constructor ////////

template<int NumAttr>
DbTuple<NumAttr>::DbTuple(std::array<int, NumAttr> data) {

    this->data = data;

}

// Example usage (don't delete, needed for linking!)
template
class DbTuple<10>;