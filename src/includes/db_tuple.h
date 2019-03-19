//
// Created by tejun on 3/19/2019.
//


#pragma once

#include <array>

template<int PreNumAttr, int PostNumAttr>
class DbTuple {

public:

    // TODO add any metadata here as well

    explicit DbTuple(std::array<int, PreNumAttr> dataBeforeDdl);

    explicit DbTuple(std::array<int, PostNumAttr> dataAfterDdl);

    bool ddlOperationComplete = false;

    std::array<int, PreNumAttr> dataBeforeDdl;

    std::array<int, PostNumAttr> dataAfterDdl;

};


