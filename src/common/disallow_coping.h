#pragma once

#define DISALLOW_COPYING(CLASS) \
    CLASS(const CLASS&) = delete;     \
    CLASS& operator=(const CLASS&) = delete

