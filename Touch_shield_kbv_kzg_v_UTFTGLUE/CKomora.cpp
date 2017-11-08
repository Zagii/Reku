
#include "CKomora.h"

void CKomora::pomiarTemp(){}
void CKomora::pomiarCisnienia(){}
void CKomora::pomiarWilgotnosci(){}

void CKomora::begin()
{
	
}


void CKomora::loop()
{
	pomiarTemp();
	pomiarCisnienia();
	pomiarWilgotnosci();
	
}
