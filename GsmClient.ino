
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

void _receiveTextMessage()
{
  debug("RecvSms: NOT IMPLEMENTED");
}

void _gsmSerialHandleLine()
{
  String s = _gsmBuffer;
  debug(s);

  if(s.indexOf("+CPIN: SIM PIN") != -1)
  {
    _writePinCode();
  }
  else if(s.indexOf("+CMTI:") != -1) // +CMTI: "SM",1
  {
    _receiveTextMessage();
  }
  else
  {
    // Currently unhandled!
  }
}
 
void gsmPollContent()
{
  if(gsmSerial.available())
  {
    byte charsRead = gsmSerial.readBytesUntil('\n', _gsmBuffer, _gsmMaxBuffer);
    if(charsRead)
    {
      _gsmBuffer[charsRead] = 0; // Terminate string
      _gsmSerialHandleLine();
    }
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

