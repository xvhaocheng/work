#pragma once
#define THIS_MODULE ((void *)1)
void __module_get(void *module);
void module_put(void *module);
