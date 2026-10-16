#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
typedef uint64_t u64;
struct device { int unused; };
#define GFP_KERNEL 0
#define dev_err(dev, ...) ((void)(dev))
