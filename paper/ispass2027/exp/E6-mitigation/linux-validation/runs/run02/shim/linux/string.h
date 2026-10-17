#include <string.h>
static inline void *memchr_inv(const void *p, int c, size_t n) { const unsigned char *s=p; while(n--) {if(*s!=(unsigned char)c) return (void *)s; s++;} return NULL;}
