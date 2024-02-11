#include "../../includes.h"

char* CLocalize::FindSafe(const char* tokenName)
{
	return CALL_VIRTUAL(char*, 17, this, tokenName);
}
