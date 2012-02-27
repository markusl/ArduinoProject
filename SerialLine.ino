
int debugLevel = 1;
const int _maxSerialBuffer = 128;
char _serialBuffer[_maxSerialBuffer] = "";

void debug(String line)
{
  if(debugLevel)
    Serial.println("dbg>" + line);
}

void intializeSerial()
{
  Serial.begin(9600);
}

void _reportMem()
{
  String s = "Memory free: ";
  s += freeMemory();
  debug(s);
}

void _serialHandleLine()
{
  String s = _serialBuffer;
  if(s.indexOf("at>") == 0)
  {
    debug("Got AT command line>");
    debug(_serialBuffer + 3);  
    gsmSerial.println(_serialBuffer + 3);
  }
  else if(s.indexOf("mem") == 0)
  {
    _reportMem();
  }
}

void _serialReadNextLine()
{
  byte charsRead = Serial.readBytesUntil('\n', _serialBuffer, _maxSerialBuffer);  
  if(charsRead)
  {
    _serialBuffer[charsRead] = 0; // Terminate string
    _serialHandleLine();
  }
}

void serialPollContent()
{
  if(Serial.available())
    _serialReadNextLine();
}

