// Copyright (c) 2010, 2014 IBM Corporation.

//
// Copy-on-write Bit Set implementation
//

#ifndef _CWBITSET_H
# define _CWBITSET_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

struct _CWBitSet {
	size_t nr;       // Reference counting
	size_t size;     // Number of words
	size_t* bits;    // Array of words
};

typedef struct _CWBitSet* CWBitSet;

/**
 * @Brief Decrement reference and eventually free set
 */
void cwbitset_unlink(CWBitSet set);

/**
 * @Brief Test if i'th bit is set
 */
bool cwbitset_isSet(CWBitSet bitset, size_t i);

/**
 * @Brief Set the i'th bit.
 *
 * This function tries to update the bitset in place if the reference is unique.
 * Otherwise it makes a copy of the bitset.
 */
CWBitSet cwbitset_setBit(CWBitSet bitset, size_t i);

/**
 * @Brief Clear the i'th bit.
 *
 * This function tries to update the bitset in place if the reference is unique.
 * Otherwise it makes a copy of the bitset.
 */
CWBitSet cwbitset_clearBit(CWBitSet bitset, size_t i);

/**
 * @Brief Perform logical or between the two sets.
 *
 * This function tries to update one of the bitsets in place if the reference is unique.
 * Otherwise a fresh bitset is returned.
 *
 */
CWBitSet cwbitset_or(CWBitSet set1, CWBitSet set2);

#ifdef __cplusplus
}
#endif

#endif
