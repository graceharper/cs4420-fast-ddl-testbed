//
// Created by tejun on 3/19/2019.
//

#include "db_tuple.h"

//////// DDL Operations ////////

template<int NumAttr>
template<int PrevNumAttr>
DbTuple<NumAttr>::DbTuple(DbTuple<PrevNumAttr> &toCopy) {

    // Copy all data directly
    auto &to_copy_data = toCopy.getData();

    for (int i = 0; i < to_copy_data.size() && i < this->data.size(); i++) {
        this->data[i] = to_copy_data[i];
    }

}

//////// Other ////////

template<int NumAttr>
std::array<int, NumAttr> &DbTuple<NumAttr>::getData() {
    return this->data;
}

template<int NumAttr>
void DbTuple<NumAttr>::setData(std::array<int, NumAttr> data) {
    this->data = data;
}