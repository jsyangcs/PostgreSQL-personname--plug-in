// bsig.c ... functions on Tuple Signatures (bsig's)
// part of SIMC signature files
// Written by John Shepherd, March 2020

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "bsig.h"
#include "psig.h"

void findPagesUsingBitSlices(Query q)
{
	assert(q != NULL);
	//DONE
	
	Reln curRel = q->rel;
	char *queryStr = q->qstring;
	// generate signature for query string
	Bits querySig = makePageSig(curRel, queryStr);
	Bits bSig = newBits(bsigBits(curRel));
	Page bSigPage = NULL;
	Count bSigSize = curRel->params.bsigSize;
	Offset bSigPos = 0;
	setAllBits(q->pages);
	Count bSigOff = 0;
	int goodPage = 0;
	// loop through each sig page but the last page
	for (int i = 0; i < nBsigPages(curRel) - 1; i++) {
		// loop through each sig item
		for (int j = 0; j < maxBsigsPP(curRel); j++) {
			bSigOff = i * maxBsigsPP(curRel) + j;
			if (bitIsSet(querySig, bSigOff)) {
				goodPage = 1;
				break;
			}
		}
		if (goodPage) {
			bSigPage = getPage(curRel->bsigf, i);
			for (int j = 0; j < maxBsigsPP(curRel); j++) {
				bSigOff = i * maxBsigsPP(curRel) + j;
				if (bitIsSet(querySig, bSigOff)) {
					bSigPos = bSigSize * j;
					getBits(bSigPage, bSigPos, bSig);
					// zero bits in Pages which are zero in Slice
					for (int j = 0; j < nPages(curRel); j++) {
						if (!bitIsSet(bSig, j)) {
							unsetBit(q->pages, j);
						}
					}
					q->nsigs++;
				}
			}
			free(bSigPage);
			q->nsigpages++;
		}
		goodPage = 0;
	}
	// deal with last bsig page
	Count rbSigNitems = nBsigs(curRel) - (nBsigPages(curRel) - 1) * maxBsigsPP(curRel);
	for (int j = 0; j < rbSigNitems; j++) {
		bSigOff = (nBsigPages(curRel) - 1) * maxBsigsPP(curRel) + j;
		if (bitIsSet(querySig, bSigOff)) {
			goodPage = 1;
			break;
		}
	}
	if (goodPage) {
		bSigPage = getPage(curRel->bsigf, nBsigPages(curRel) - 1);
		for (int j = 0; j < pageNitems(bSigPage); j++) {
			//printf("Wow! it is %d, at most %d\n", j, pageNitems(bSigPage) - 1);
			bSigOff = (nBsigPages(curRel) - 1) * maxBsigsPP(curRel) + j;
			if (bitIsSet(querySig, bSigOff)) {
				bSigPos = bSigSize * j;
				getBits(bSigPage, bSigPos, bSig);
				// zero bits in Pages which are zero in Slice
				for (int j = 0; j < nPages(curRel); j++) {
					if (!bitIsSet(bSig, j)) {
						unsetBit(q->pages, j);
					}
				}
				q->nsigs++;
			}
		}
		free(bSigPage);
		q->nsigpages++;
	}
}

