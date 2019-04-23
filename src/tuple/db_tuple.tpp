//
// Created by tejun on 3/19/2019.
//

#include "includes/tuple/db_tuple.h"

//////// DDL Operations ////////

template<int NumAttr>
template<int PrevNumAttr>
DbTuple<NumAttr>::DbTuple(DbTuple<PrevNumAttr> &toCopy) {

    auto &to_copy_data = toCopy.getData();

    // Copy all pre-existing data directly
    for (int i = 0; i < to_copy_data.size() && i < this->data.size(); i++) {
        this->data[i] = to_copy_data[i];
    }

    // If extra columns, default them to the last column
    for (int i = static_cast<int>(to_copy_data.size()); i < this->data.size(); i++) {
        this->data[i] = to_copy_data[i - to_copy_data.size()];
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