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
     * Adds the specified number of "integer" columns to the end of the data structure.
     *
     * @param numColumns the number of columns to add
     */
    virtual void addColumnsToEnd(int numColumns) = 0;

    /**
     * Removes the specified number of "integer" columns from the end of the data structure.
     *
     * @param numColumns the number of columns to remove
     * @throws length_error if the data structure would end up with less than 0 columns
     */
    virtual void removeColumnsFromEnd(int numColumns) = 0;

};