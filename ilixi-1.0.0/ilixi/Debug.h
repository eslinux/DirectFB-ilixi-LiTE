/*
 * debug.h
 *
 *  Created on: Aug 13, 2014
 *      Author: nvthanh
 */

#ifndef DEBUG_H_
#define DEBUG_H_
#include <stdio.h>

#if 1
#define DBG(x...) {printf("\e[033;34m%s::%s<%d> \e[0m",__FILE__, __FUNCTION__,__LINE__); printf(x);}
#define FATAL(x...) {printf("\e[033;31m%s::%s<%d> \e[0m",__FILE__, __FUNCTION__,__LINE__); printf(x);}
#else
#define DBG(x...)
#define FATAL(x...)
#endif
#endif /* DEBUG_H_ */
