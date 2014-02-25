// Copyright (c) 2010, 2014 IBM Corporation.

#include "cbitset.h"
#include <stdbool.h>

#define WORD_KIND_MASK (((size_t) 0x1) << 63)
#define COUNT_MASK (~(((size_t) 0x3) << 62))

#define FILLER_ONE_LITERAL 0x1

inline CBitSet
cbitset_link(CBitSet set)
{
	if (set == CBITSET_EMPTY)
		return CBITSET_EMPTY;

	set->refcount ++;
	return set;
}

inline void
cbitset_unlink(CBitSet set)
{
	if (set != CBITSET_EMPTY)
	{
		set->refcount --;
		if (set->refcount == 0)
			free(set);
	}
}

static inline bool
cbitset_isLiteralWord(size_t word)
{
	return (word & WORD_KIND_MASK) == 0;
}

static inline bool
cbitset_isFillerWord(size_t word)
{
	return (word & WORD_KIND_MASK) == WORD_KIND_MASK;
}

static inline size_t
cbitset_wordAt(CBitSet set, size_t index)
{
	return set->bitset[index];
}

static inline size_t
cbitset_length(size_t word)
{
	return word & COUNT_MASK;
}

static inline COpenBitSet
cbitset_new()
{
	COpenBitSet set = malloc(sizeof(struct _COpenBitSet));
	set->bitset = malloc(sizeof(struct _CBitSet));
	set->cbitset->bitset = malloc(64 * sizeof(size_t));
	set->cbitset->refcount = 1;
}

static inline COpenBitSet
cbitset_append(COpenBitSet set, size_t literal)
{
	if (!set)
	{
		set = cbitset_new();

		set->cbitset->bitset[0] = FILLER_ONE_LITERAL;
		set->cbitset->bitset[1] = literal;
		set->cbitset->numword = 2;

		set->cidx = 0;
	}
	else
	{
		size_t filler = set->cbitset->bitset[set->last];
		if (cbitset_isLiteralWord(filler))
		{
			// Don't need to change filler word: just append literal
			set->cbitset->bitset[set->cbitset->numword++] = literal;
			set->cbitset->bitset[set->last] ++;
		}
		else
		{
			// A filler word is required.
			set->last = set->cbitset->numword;
			set->cbitset->bitset[set->cbitset->numword++] = FILLER_ONE_LITERAL;
			set->cbitset->bitset[set->cbitset->numword++] = literal;
		}
	}
}

CBitSet
cbitset_or(CBitSet set1, CBitSet set2)
{
	// Handle trivial cases
	if (cbitset_empty(set1))
		return set2;

	if (cbitset_empty(set2))
		return set1;

	if (set1 == set2)
	{
		cbitset_unlink(set2);
		return set1;
	}

	// Need to combine
	state = 1;
	size_t idx1 = 0;
	size_t idx2 = 0;
	size_t word1 = cbitset_wordAt(set1, idx1);
	size_t word2 = cbitset_wordAt(set2, idx2);

	size_t idxr = 0;
	COpenBitSet rset = 0;
	do
	{
		switch (state)
		{
			case 0: // Done.
			break;
			case 1: // Both on filler word.
			{
				const bool literal1 = cbitset_isLiteralWord(word1);
				const bool literal2 = cbitset_isLiteralWord(word2);
				if (literal1 && literal2)
				{
					// Both literal
					const size_t length1 = cbitset_length(word1);
					const size_t length2 = cbitset_length(word2);

					size_t min = count1 < count2 ? count1 : count2;
					while (--min)
						cbitset_append(rset, cbitset_wordAt(set1, ++idx1) | cbitset_wordAt(set2, ++idx2));

					if (count1 < count2)
					{

					}
					else if (count1 > count2)
					{

					}
				}

				// Not implemented yet..
			}
			break;

		}
	} while (state != 0);


}
