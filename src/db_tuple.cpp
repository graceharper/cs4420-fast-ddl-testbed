//
// Created by tejun on 3/19/2019.
//

#include "db_tuple.h"

//////// Constructor ////////

template<int PreNumAttr, int PostNumAttr>
DbTuple<PreNumAttr, PostNumAttr>::DbTuple(std::array<int, PreNumAttr> dataBeforeDdl) {
    this->dataBeforeDdl = dataBeforeDdl;
}

template<int PreNumAttr, int PostNumAttr>
DbTuple<PreNumAttr, PostNumAttr>::DbTuple(std::array<int, PostNumAttr> dataAfterDdl) {
    this->dataAfterDdl = dataAfterDdl;
    this->ddlOperationComplete = true;
}

// Example usage (don't delete, needed for linking!)
template
class DbTuple<10, 20>;