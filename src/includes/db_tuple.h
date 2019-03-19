//
// Created by tejun on 3/19/2019.
//


#pragma once

#include <array>

template<int NumAttr>
class DbTuple {

public:

    // TODO add any metadata here as well

    explicit DbTuple(std::array<int, NumAttr> data);

    std::array<int, NumAttr> data;

};


