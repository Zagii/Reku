
#include "CKomora.h"


void CKomora::pomiarTemp()
{
  if(millis()-_tempMillis>5000)
  {
    _tempMillis=millis();
    _temp = sensors.getTempC(termometrAddr[id]);
  }

}
void CKomora::pomiarCisnienia(){}
void CKomora::pomiarWilgotnosci(){}

void CKomora::begin(uint8_t komoraID)
{
  DPRINT(__func__);DPRINT(" komoraID=");DPRINTLN(komoraID);
	id=komoraID;
  if(!czyBylTempBegin)
  {
    czyBylTempBegin=true;
    sensors.begin();  
  }
  if (!sensors.getAddress(termometrAddr[id], id))
  {
     DPRINT(__func__);DPRINT(" problem z termometrem id=");DPRINTLN(id);
      
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
