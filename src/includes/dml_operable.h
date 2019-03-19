//
// Created by tejun on 3/18/2019.
//

#pragma once

/**
 * Represents a class that supports DML Operations.
 *
 * Any class of this type must implement all the "DML" functions below.
 */
template<typename TupleType>
class DmlOperable {

    /**
     * Virtual destructor required
     * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
     */
    virtual ~DmlOperable() = default;

    /**
     * Start (or re-start) the scan on the given data structure
     */
    virtual void startScan() = 0;

    /**
     * Returns the next tuple in the scan for the given data structure.
     * Similar to tuple-at-a-time / volcano style iterator model.
     *
     * @return tuple of the templated type (by reference)
     */
    virtual TupleType &getNextTuple() = 0;

};