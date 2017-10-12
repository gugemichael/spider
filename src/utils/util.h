#ifndef __UTILS_H__
#define __UTILS_H__

#pragma once

#include <string>
#include <sstream>

#include <cassert>

#define invariant(cond) assert(cond)

namespace utils {

static const uint64_t HUMAN_KB = 1024;
static const uint64_t HUMAN_MB = 1024 * HUMAN_KB;
static const uint64_t HUMAN_GB = 1024 * HUMAN_MB;

static const std::string HUMAN_BYTES_STR = "bytes";
static const std::string HUMAN_KB_STR = "KB";
static const std::string HUMAN_MB_STR = "MB";
static const std::string HUMAN_GB_STR = "GB";

static std::string human_readable_capacity(uint64_t number) {
    std::ostringstream os;
    if (number < HUMAN_KB)
        os << number << HUMAN_BYTES_STR;
    else if (number < HUMAN_MB)
        os << number / HUMAN_KB << HUMAN_KB_STR;
    else if (number < HUMAN_GB)
        os << number / HUMAN_MB << HUMAN_MB_STR;
    else
        os << number / HUMAN_GB << HUMAN_GB_STR;

    return os.str();
}

} // end namespace

#endif