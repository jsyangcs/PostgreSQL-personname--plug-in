// tsig.c ... functions on Tuple Signatures (tsig's)
// part of SIMC signature files
// Written by John Shepherd, March 2020

#include <unistd.h>
#include <string.h>
#include "defs.h"
#include "tsig.h"
#include "reln.h"
#include "hash.h"
#include "bits.h"

// make a tuple signature

Bits makeTupleSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	//DONE
	//printf("making tuplesig\n");
	Bits result = newBits(tsigBits(r));
	char **val = tupleVals(r, t);
	for (int i = 0; i < nAttrs(r); i++) {
		// deal wit var[i]
		// bits set per attribute: codeBits(r)
		// width of tuple signature: tsigBits(r)
		Bits cWord = codeWord(val[i], tsigBits(r), codeBits(r));
		orBits(result, cWord);
		freeBits(cWord);
	}
	freeVals(val, nAttrs(r));
	return result;
}

// find "matching" pages using tuple signatures

void findPagesUsingTupSigs(Query q)
{
	assert(q != NULL);
	//DONE
	Reln curRel = q->rel;
	char *queryStr = q->qstring;
	// generate signature for query string
	Bits querySig = makeTupleSig(curRel, queryStr);
	// loop through each sig page
	for (int i = 0; i < nTsigPages(curRel); i++) {
		Page curPage = getPage(curRel->tsigf, i);
		// loop through each sig item
		for (int j = 0; j < pageNitems(curPage); j++) {
			Offset curPos = j * curRel->params.tsigSize;
			Bits curSig = newBits(tsigBits(curRel));
			getBits(curPage, curPos, curSig);
			if (isSubset(querySig, curSig)) {
				Count sigOffset = i * maxTsigsPP(curRel) + j;
				Count pageOffset = sigOffset/maxTupsPP(curRel);
				setBit(q->pages, pageOffset);
			}
			q->nsigs++;
			freeBits(curSig);
		}
		q->nsigpages++;
		free(curPage);

	}
	freeBits(querySig);
	// The printf below is primarily for debugging
	// Remove it before submitting this function
	//printf("Matched Pages:"); showBits(q->pages); putchar('\n');
}
