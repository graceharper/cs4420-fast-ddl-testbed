/*
** Created by tejun on 3/19/2019.
***********************************
** This header file defines configurable variables about the database. 
** Size of the table is configured here. 
*/

#pragma once

constexpr int NUMBER_TUPLES_PER_GROUP = 1024;
constexpr int NUMBER_TUPLE_GROUPS = 64;
