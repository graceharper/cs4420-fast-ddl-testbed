//
// Created by tejun on 3/18/2019.
//

#pragma once

/**
 * Represents a class that supports DDL Operations.
 *
 * Any class of this type must implement all the "DDL" functions below.
 */
class DdlOperable {

public:

    /**
     * Virtual destructor required
     * https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
     */
    virtual ~DdlOperable() = default;

    /**
     * Runs the DDL operation to move tuples from one data structure to another.
     */
    virtual void runDdlOperation() = 0;

};