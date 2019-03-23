//
// Created by tejun on 3/19/2019.
//


#pragma once

#include <array>

template<int NumAttr>
class DbTuple {

public:

    // TODO add any metadata here as well

    DbTuple() = default;

    template<int PrevNumAttr>
    DbTuple(DbTuple<PrevNumAttr> &toCopy);

    void setData(std::array<int, NumAttr> data);

    /**
     * @return an array of data in the tuple (by reference, so you can change it)
     */
    std::array<int, NumAttr> &getData();

protected:

    // Default-initialization of array
    std::array<int, NumAttr> data;

};

// Link to template implementation
#include "db_tuple.tpp"


