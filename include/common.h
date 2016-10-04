#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef max
# undef max 
# define max(a,b) ((a) > (b) ? a : b)
#endif

#ifdef min
# undef min
# define min(a, b) ((a) < (b) ? a : b)
#endif

#endif //_COMMON_H_
