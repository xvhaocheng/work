#pragma once
#define check_add_overflow(a,b,out) __builtin_add_overflow(a,b,out)
