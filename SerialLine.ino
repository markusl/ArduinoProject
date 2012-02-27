
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

void _serialHandleLine()
{
  String s = _serialBuffer;
  if(s.indexOf("at>") == 0)
  {
    debug("Got AT command line>");
    debug(_serialBuffer + 3);  
    gsmSerial.println(_serialBuffer + 3);
  }
  _serialBuffer[0] = 0;
}

void _serialReadNextLine()
{
  Serial.readBytesUntil('\n', _serialBuffer, _maxSerialBuffer);  
  _serialHandleLine();
}

void serialPollContent()
{
  if(Serial.available())
    _serialReadNextLine();
}

