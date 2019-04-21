//
// Created by grace on 3/24/19.
//

#pragma once

constexpr int SMALL_NUM_COLS = 1024;     // 2^10 = 1 KB
constexpr int BIG_NUM_COLS = 2 * 1024;   // 2^11 = 2 KB
constexpr int NUM_TUPLES = NUMBER_TUPLE_GROUPS * NUMBER_TUPLES_PER_GROUP;
constexpr int NUM_FULL_SCANS_PRE_DDL = 5;
constexpr int NUM_FULL_SCANS_POST_DDL = (NUMBER_TUPLE_GROUPS / MAX_MATERIALIZATIONS_PER_QUERY) + NUM_FULL_SCANS_PRE_DDL;