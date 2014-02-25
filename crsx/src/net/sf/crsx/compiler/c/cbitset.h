// Copyright (c) 2010, 2014 IBM Corporation.

//
// Compressed bitset implementation, inspired by https://sdm.lbl.gov/fastbit/
//
// The binary encoding consists of two kinds of words, literal and filler.
//
// A literal word just encoded non-compressed bitset value.
//
// A filler word as the following format:
// - 63rd bit:
//       - 0 = indicates next sequence of words are literals words
//       - 1 = indicates this word is a same bit filler word
// - 62nd bit: when 63rd bit is set, then:
//       - 0 = this is a filler word with no bit set
//       - 1 = this is a filler word with all bits set
// - remaining bits: indicates the number of literal words or filler words.
//
//

#ifndef _BITSET_H
# define _BITSET_H
#ifdef __cplusplus
extern "C" {
#endif

#define EMPTY_CBITSET 0


struct _CBitSet {
	size_t refcount; // Reference count
	size_t numword;  // Total number of words (filler and literal). Always more than 1.
	size_t* bitset;  // Binary encoded bitset
};

typedef struct _CBitSet* CBitSet;

// Appendable bit set
struct _COpenBitSet {
	CBitSet cbitset; // Bitset being constructed.
	size_t last;    // index of last filler word
};

typedef struct _COpenBitSet* COpenBitSet;

/**
 * @Brief Perform logical 'or' operation between 2 sets.
 *
 * Both set references are transferred.
 */
CBitSet cbitset_or(CBitSet set1, CBitSet set2);


#ifdef __cplusplus
}
#endif

#endif
