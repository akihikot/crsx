// Copyright (c) 2010, 2014 IBM Corporation.

#include "cwbitset.h"

#define EMPTY_SET 0
#define BITS_PER_WORD  (sizeof(size_t) * 8)
#define NO_BITS   0ULL
#define ONE_BIT   1ULL
#define IS_EMPTY_SET(set) (!set)

typedef size_t BITS

#define ASIZE(size)   ((size / BITS_PER_WORD) + 1)
#define AINDEX(index) (index / BITS_PER_WORD)
#define BINDEX(index) (index % BITS_PER_WORD)


// @Brief Copy bit set
static inline CWBitSet
cwbitset_copy(CWBitSet set)
{
	CWBitSet clone = cwbitset_new(set->size);
    memcpy((void*)clone->bits, (void*)set->bits, set->size);
    return clone;
}

// @Brief Return CWBitSet with only one reference
// set reference is transferred.
static inline CWBitSet
cwbitset_unique(CWBitSet set)
{
	if (set->nr == 1)
		return set;

	set->nr --;
	return cwbitset_copy(set);
}

void
cwbitset_unlink(CWBitSet set)
{
	set->nr --;
	if (set->nr == 0)
	{
		free(set->bits)
		free(set);
	}
}

CWBitSet
cwbitset_new(size_t numwords)
{
	CWBitSet set = malloc(sizeof(struct _CWBitSet));
	set->nr = 1;
	set->size = numwords;
	set->bits = calloc(numwords, sizeof(BITS));
	return set;
}

bool
cwbitset_isSet(CWBitSet bitset, size_t index)
{
	return (bitset->bits[AINDEX(index)] & BINDEX(index)) != 0;
}

CWBitSet
cwbitset_setBit(CWBitSet bitset, size_t index)
{
	bitset = cwbitset_unique(bitset);

	size_t ai = AINDEX(index);
	size_t bi = BINDEX(index);

	bitset->bits[ai] = bitset->bits[ai] | (ONE_BIT << bi);

	return bitset;
}

CWBitSet
cwbitset_clearBit(CWBitSet bitset, size_t index)
{
	bitset = cwbitset_unique(bitset);

	size_t ai = AINDEX(index);
	size_t bi = BINDEX(index);

	bitset->bits[ai] = bitset->bits[ai] & (~(ONE_BITS << bi));
	return bitset;
}

CWBitSet
cwbitset_or(CWBitSet set1, CWBitSet set2)
{
	// Handle trivial cases
	if (IS_EMPTY_SET(set1))
		return set2;
	if (IS_EMPTY_SET(set2))
		return set1;
	if (set1 == set2)
	{
		cwbitset_unlink(set2);
		return set1;
	}

	if (set2->size > set1->size)
	{
		CWBitSet t = set2;
		set2 = set1;
		set1 = t;
	}

	size_t min = set2->size;
	while (--min)
		set1->bits[min] |= set2->bits[min];

	cwbitset_unlink(set2);
	return set1;
}

