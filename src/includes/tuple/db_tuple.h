/*
** Created by tejun on 3/19/2019.
**
*******************************************************************************
** This header file defines the tuple interface. Tuples are internally
** represented as an array; currently, we only support integer attribute types.
*/

#pragma once


#include <array>


template<int NumAttr>
class DbTuple {

public:

    DbTuple() = default;

    /*
    ** @param DbTuple, initializes new tuple with contents of provided tuple 
    */
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
#include "tuple/db_tuple.tpp"


