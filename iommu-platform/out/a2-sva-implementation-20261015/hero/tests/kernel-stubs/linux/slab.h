#pragma once
#include <stddef.h>
void *kzalloc(size_t size, int flags);
void *kcalloc(size_t count, size_t size, int flags);
void kfree(void *ptr);
