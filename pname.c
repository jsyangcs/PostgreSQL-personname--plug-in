/******************************************************************************
  This file contains routines that can be bound to a Postgres backend and
  called by the backend in the process of processing queries.  The calling
  format for these routines is dictated by Postgres architecture.
******************************************************************************/

#include "postgres.h"
#include "utils/hashutils.h"
#include <string.h>
#include <regex.h>
#include "fmgr.h"

PG_MODULE_MAGIC;

typedef struct PersonName
{
	int32 vl_len_;                              //varlena header
	char name[FLEXIBLE_ARRAY_MEMBER];
} PersonName;

static int validPersonName(char *str);

/*****************************************************************************
 * Check-input functions
 *****************************************************************************/
static int
validPersonName(char *str)
{
	regex_t re;
	int status;
	bool res = 0;
	const char *pattern = "^([A-Z][-|'|A-Z|a-z]+)(( [A-Z][-|'|A-Z|a-z]+)*), ?([A-Z][-|'|A-Z|a-z]+)(( [A-Z][-|'|A-Z|a-z]+)*)$";
	//const char *pattern = "^(([A-Z][A-Z]?[-|'|A-Z|a-z]+))(( [A-Z]?[-|'|A-Z|a-z]+)*), ?([A-Z][A-Z]?[-|'|A-Z|a-z]+)(( [A-Z]?[-|'|A-Z|a-z]{2,}+)*)$";
	if (regcomp(&re, pattern, REG_EXTENDED) == 0)
	{
		// if regcomp(&re, pattern, REG_EXTENDED) == 0, matched
		status = regexec(&re, str, 0, NULL, 0);
		if (status == 0)
		{
			res = 1;
		}
	}

	regfree(&re);

	return res;
}

/*****************************************************************************
 * turn input string into canonicalPersonName form
 *****************************************************************************/
static char *
canonicalPersonName(char *str)
{
	/* check if the input string is already canonical */
	char *pt = strchr(str, ',');
	if (*(pt + 1) == ' ')
	{
		for (pt++; *pt != '\0'; pt++)
		{
			*pt = *(pt + 1);
		}
	}
	return str;
}

/*****************************************************************************
 * Input functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(pname_in);

Datum
pname_in(PG_FUNCTION_ARGS)
{
	char *str = PG_GETARG_CSTRING(0);
	PersonName *result;
	int length;

	if (!validPersonName(str))
	{
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type %s: \"%s\"",
						"PersonName", str)));
	}

	char *cpy = (char *)palloc(strlen(str) + 1);
	memcpy((void *)cpy, (void *)str, strlen(str) + 1);
	char *name = (char *)canonicalPersonName((char *)cpy);
	length = strlen(name);
	result = (PersonName *)palloc(VARHDRSZ + length + 1);
	SET_VARSIZE(result, VARHDRSZ + length + 1);
	memcpy((void *)VARDATA(result), (void *)name, length + 1);
	PG_RETURN_POINTER(result);
}


/*****************************************************************************
 * Output functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(pname_out);

Datum
pname_out(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *)PG_GETARG_POINTER(0);
	char *result;
	result = psprintf("%s", VARDATA(pname));
	PG_RETURN_CSTRING(result);
}


/*****************************************************************************
 * Given(name) functions
 *****************************************************************************/
PG_FUNCTION_INFO_V1(given);

Datum
given(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *)PG_GETARG_CSTRING(0);
	int length;
	char *name;
	char *result;
	char *gname;
	length = VARSIZE(pname) - VARHDRSZ;                         // length of content
	name = (char *)palloc(length);
	memcpy((void *)name, (void *)(VARDATA(pname)), length);     // get a copy of content of pname
	char *pt = strchr(name, ',');							// find the seperator
	gname = pt + 1;
	result = psprintf("%s", gname);
	PG_RETURN_TEXT_P(cstring_to_text(result));
}

/*****************************************************************************
 * Family(name) functions
 *****************************************************************************/
PG_FUNCTION_INFO_V1(family);

Datum
family(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *)PG_GETARG_CSTRING(0);
	int length;
	char *name;
	char *fname;
	char *result;
	length = VARSIZE(pname) - VARHDRSZ; /* length of content */
	name = (char *)palloc(length);
	memcpy(name, VARDATA_ANY(pname), length); /* get a copy of content of pname */
	char *pt = strchr(name, ',');			  /* find the seperator */
	*pt = '\0';
	fname = name;
	result = psprintf("%s", fname);
	PG_RETURN_TEXT_P(cstring_to_text(result));
}

/*****************************************************************************
 * Show(name) functions
 * appends the entire Family name to the first Given name
 * everything before the first space, if any
 * separated by a single space.
 *****************************************************************************/
PG_FUNCTION_INFO_V1(show);

Datum
show(PG_FUNCTION_ARGS)
{

	PersonName *pname = (PersonName *)PG_GETARG_CSTRING(0);
	int length;
	char *name;
	char *fname; /* the famliy part of result */
	char *gname; /* the given part of result */
	int lenWord;
	char *blank;
	char *result;
	int lenFamily;
	int lenResult;
	length = VARSIZE(pname) - VARHDRSZ;		      // length of content
	name = (char *)palloc(length + 1);		      // one more bit for extra blank
	memcpy(name, VARDATA_ANY(pname), length);     // get a copy of content of pname
	char *delimeter = strchr(name, ',');
	gname = delimeter + 1;
	*delimeter = '\0';
	fname = name;
	lenFamily = strlen(fname);
	blank = strchr(gname, ' ');
	// deal with blank
	if (blank == NULL)
	{
		lenWord = strlen(gname);
		gname[lenWord] = ' ';
		gname[lenWord + 1] = '\0';
	}
	else
	{
		*blank = ' ';
		*(blank + 1) = '\0';
	}
	lenWord = strlen(gname);
	lenResult = lenWord + lenFamily + 1;
	result = (char *)palloc(lenResult);
	memcpy((void *)result, (void *)gname, lenWord + 1);
	strncat((text *)result, fname, lenFamily + 1);
	PG_RETURN_TEXT_P(cstring_to_text(result));
}

/*****************************************************************************
 * New Operators
 *
 * A practical PersonName datatype would provide much more than this, of course.
 *****************************************************************************/
static int
pname_cmp_internal(PersonName *a, PersonName *b)
{
	int diff = strcmp(VARDATA(a), VARDATA(b));
	if (diff > 0)
	{
		return 1;
	}
	else if (diff < 0)
	{
		return -1;
	}
	return 0;
}

PG_FUNCTION_INFO_V1(pname_cmp);

Datum
pname_cmp(PG_FUNCTION_ARGS)
{
    PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
    PersonName *b = (PersonName *) PG_GETARG_POINTER(1);

    PG_RETURN_INT32(pname_cmp_internal(a, b));
}

/*****************************************************************************
 * Operator class for defining B-tree index
 *
 * It's essential that the comparison operators and support function for a
 * B-tree index opclass always agree on the relative ordering of any two
 * data values.  Experience has shown that it's depressingly easy to write
 * unintentionally inconsistent functions.  One way to reduce the odds of
 * making a mistake is to make all the functions simple wrappers around
 * an internal three-way-comparison function, as we do here.
 *****************************************************************************/

PG_FUNCTION_INFO_V1(pname_lt);

Datum
	pname_lt(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *)PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *)PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(pname_cmp_internal(a, b) < 0);
}

PG_FUNCTION_INFO_V1(pname_le);

Datum
	pname_le(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *)PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *)PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(pname_cmp_internal(a, b) <= 0);
}

PG_FUNCTION_INFO_V1(pname_eq);

Datum
	pname_eq(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *)PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *)PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(pname_cmp_internal(a, b) == 0);
}

PG_FUNCTION_INFO_V1(pname_ge);

Datum
	pname_ge(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *)PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *)PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(pname_cmp_internal(a, b) >= 0);
}

PG_FUNCTION_INFO_V1(pname_gt);

Datum
	pname_gt(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *)PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *)PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(pname_cmp_internal(a, b) > 0);
}

PG_FUNCTION_INFO_V1(pname_neq);

Datum
	pname_neq(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *)PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *)PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(pname_cmp_internal(a, b) != 0);
}

PG_FUNCTION_INFO_V1(pname_hash);

Datum
	pname_hash(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *)PG_GETARG_POINTER(0);
	int length;
	char *name;
	Datum result;
	length = VARSIZE(pname) - VARHDRSZ;                      // length of content
	name = (char *)palloc(length);
	memcpy((void *)name, (void *)(VARDATA(pname)), length);  // get a copy of content of pname
	name = (char *)canonicalPersonName((char *)name);
	result = hash_any((unsigned char *)name, strlen(name));
	PG_RETURN_DATUM(result);
}
