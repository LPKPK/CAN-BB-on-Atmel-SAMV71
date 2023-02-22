/** @file sli_assert.h
 *
 * @brief Definition of assert macro
 *
 * @par
 * Copyright (c) 2021 Sunrise Labs, Inc.
 * All rights reserved.
 */

#ifndef SLI_ASSERT_H
#define SLI_ASSERT_H

// Here is a hook to create your own SLI_ASSERT
#include "bb_port.h"

#ifdef DEVICE_GDN 
#ifdef VECTORCAST
extern int VECTORCAST_ASSERT_VALUE;
#define SLI_ASSERT(x) if (!(x)) { VECTORCAST_ASSERT_VALUE = 12345; }
#endif
#endif

#ifndef SLI_ASSERT
#include <assert.h>
#define SLI_ASSERT(x) \
   assert(x)

#endif // SLI_ASSERT

#endif //SLI_ASSERT_H
