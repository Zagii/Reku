
#include "CKomora.h"


void CKomora::pomiarTemp()
{
  if(millis()-_tempMillis>5000)
  {
    _tempMillis=millis();
    sensors.requestTemperaturesByAddress(termometrAddr[id]);
    _temp = sensors.getTempC(termometrAddr[id]);
   
    DPRINT(__func__);DPRINT(" komoraID=");DPRINT(id);DPRINT(" temp=");DPRINTLN(_temp);
  }

}
void CKomora::pomiarCisnienia(){}
void CKomora::pomiarWilgotnosci(){}

// function to print a device address
void CKomora::printAddress(DeviceAddress deviceAddress,char * buf)
{
  strcpy(buf,"");
  char tmp[4];
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) strcat(buf,"0");

    sprintf(tmp,"%X",deviceAddress[i]);
    strcat(buf,tmp);
  }
}
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
  printAddress(termometrAddr[id],addressStr);
  sensors.setResolution(termometrAddr[id], TEMPERATURE_PRECISION);
  _tempMillis=millis();
}


void CKomora::loop()
{
	pomiarTemp();
	pomiarCisnienia();
	pomiarWilgotnosci();
	
}
