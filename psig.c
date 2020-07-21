// psig.c ... functions on page signatures (psig's)
// part of SIMC signature files
// Written by John Shepherd, March 2020

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "psig.h"

Bits makePageSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	//DONE
	Bits result = newBits(psigBits(r));
	char **val = tupleVals(r, t);
	for (int i = 0; i < nAttrs(r); i++) {
		// deal wit var[i]
		// bits set per attribute: codeBits(r)
		// width of tuple signature: psigBits(r)
		Bits cWord = codeWord(val[i], psigBits(r), codeBits(r));
		orBits(result, cWord);
		freeBits(cWord);
	}
	freeVals(val, nAttrs(r));
	return result;
}

void findPagesUsingPageSigs(Query q)
{
	assert(q != NULL);
	//DONE
	Reln curRel = q->rel;
	char *queryStr = q->qstring;
	// generate signature for query string
	Bits querySig = makePageSig(curRel, queryStr);
	// loop through each sig page
	for (int i = 0; i < nPsigPages(curRel); i++) {
		Page curPage = getPage(curRel->psigf, i);
		// loop through each sig item
		for (int j = 0; j < pageNitems(curPage); j++) {
			// data page corresponding to Psig
			Offset curPos = j * curRel->params.psigSize;
			Bits curSig = newBits(psigBits(curRel));
			getBits(curPage, curPos, curSig);
			if (isSubset(querySig, curSig)) {
				Count sigOffset = i * maxPsigsPP(curRel) + j;
				Count pageOffset = sigOffset;
				setBit(q->pages, pageOffset);
			}
			q->nsigs++;
			freeBits(curSig);
		}
		free(curPage);
		q->nsigpages++;
	}
	freeBits(querySig);
}

