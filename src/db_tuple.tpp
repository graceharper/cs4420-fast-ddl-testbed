//
// Created by tejun on 3/19/2019.
//

#include "db_tuple.h"

//////// Other ////////

template<int NumAttr>
std::array<int, NumAttr> &DbTuple<NumAttr>::getData() {
    return this->data;
}

template<int NumAttr>
void DbTuple<NumAttr>::setData(std::array<int, NumAttr> data) {
    this->data = data;
}

// Example usages needed for linking!
template
class DbTuple<4>;

template
class DbTuple<7>;