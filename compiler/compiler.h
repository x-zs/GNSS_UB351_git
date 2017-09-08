/*! ----------------------------------------------------------------------------
 * @file	compiler.h
 * @brief
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */

#ifndef COMPILER_H_
#define COMPILER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>


#define __align4            __attribute__((aligned (4)))
#define __weak              __attribute__((weak))
#ifndef __always_inline
#define __always_inline     __attribute__((always_inline))
#endif

#ifndef __IO
#define __IO        volatile
#endif

/* Redefine CLOCKS_PER_SEC to make it work with Sleep(1) */
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC      1000


#ifdef __cplusplus
}
#endif

#endif /* COMPILER_H_ */
