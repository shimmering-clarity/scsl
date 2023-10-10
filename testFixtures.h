#ifndef KLIB_TESTFIXTURES_H
#define KLIB_TESTFIXTURES_H


#include <string.h>
#include "TLV.h"


#define ARENA_SIZE	128
#define ARENA_FILE	"arena_test.bin"

/* strlen=13 */
#define TEST_STR1	"Hello, world"
#define TEST_STRLEN1	13
#define TEST_STR2	"Bye, world!!"
#define TEST_STRLEN2	13
#define TEST_STR3	"Hello, arena"
#define TEST_STRLEN3	13

/* strlen 35 */
#define TEST_STR4	"How is a raven like a writing desk?"
#define TEST_STRLEN4	35


namespace klib {


static bool
cmpRecord(TLV::Record &a, TLV::Record &b)
{
	if (a.Tag != b.Tag) {
		return false;
	}

	if (a.Len != b.Len) {
		return false;
	}

	if (memcmp(a.Val, b.Val, a.Len) != 0) {
		return false;
	}

	return true;
}


} // namespace klib


#endif // KLIB_TESTFIXTURES_H
