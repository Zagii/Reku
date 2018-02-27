
#include "CKomora.h"


void CKomora::pomiarTemp()
{
  if(millis()-_tempMillis>1000)
  {
    _tempMillis=millis();
    _temp = sensors.getTempC(termometrAddr[id]);
  }

}
void CKomora::pomiarCisnienia(){}
void CKomora::pomiarWilgotnosci(){}

void CKomora::begin(uint8_t komoraID)
{
  dPrintf("%s:%d: %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
	id=komoraID;
  if(!czyBylTempBegin)
  {
    czyBylTempBegin=true;
    sensors.begin();  
  }
  if (!sensors.getAddress(termometrAddr[id], id))
  {
    dPrintf("Problem z termometrem komora id=%d",id);
      
  }
  sensors.setResolution(termometrAddr[id], TEMPERATURE_PRECISION);
  _tempMillis=0;
}


void CKomora::loop()
{
	pomiarTemp();
	pomiarCisnienia();
	pomiarWilgotnosci();
	
}
