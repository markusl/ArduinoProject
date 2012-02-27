
#include <SoftwareSerial.h>
 
SoftwareSerial gsmSerial(7, 8);
const int _gsmTimeout = 30;
const int _gsmMaxBuffer = 128;
char _gsmBuffer[_gsmMaxBuffer] = "";
 
void initializeGsm()
{
  debug("initializeGsm");
  gsmSerial.begin(19200);               // the GPRS baud rate

//  _gsmBuffer.reserve(_gsmMaxBuffer);
}

void _writePinCode()
{
  gsmSerial.write("AT+CPIN=7201\r");
  debug("PIN code written");
  _gsmReadBytesOrDisplayError(4); // AT\r\n
}

void _gsmSerialHandleLine()
{
  String s = _gsmBuffer;
  if(s.length() > 0)
    debug(_gsmBuffer);
  if(s.indexOf("+CPIN: SIM PIN") != -1)
  {
    _writePinCode();
  }
  _gsmBuffer[0] = 0;
}
 
void gsmPollContent()
{
  if(gsmSerial.available())
  {
    gsmSerial.readBytesUntil('\n', _gsmBuffer, _gsmMaxBuffer);  
    _gsmSerialHandleLine();
  }
}

boolean _gsmReadBytesOrDisplayError(char numberOfBytes)
{
  if(!_gsmWaitForBytes(numberOfBytes, _gsmTimeout))
  {
    debug("Error reading bytes from device!");
    return false;
  }
  
  while(gsmSerial.available())
  {
    char next = gsmSerial.read();
    Serial.print(next);
  }
  
  return true;
}

char _gsmWaitForBytes(char numberOfBytes, int timeout)
{
  while(gsmSerial.available() < numberOfBytes)
  {
    delay(200);
    timeout -= 1;
    if(timeout == 0)
    {
      return 0;
    }
  }
  return 1;
}

