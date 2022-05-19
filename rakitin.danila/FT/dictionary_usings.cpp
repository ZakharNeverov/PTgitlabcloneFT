#include "dictionary_usings.h"

bool rakitin::operator<(const word_refs& ref1, const word_refs& ref2)
{
	return ref1.first < ref2.first;
}
