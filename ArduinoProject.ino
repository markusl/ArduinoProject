/*
  ArduinoProject
 */

const boolean useHttp = false;
const boolean useGsm = true;
const boolean useKeypad = false;

void setup()
{
  intializeSerial();
  if(useKeypad)
    initializeKeypad();
    
  if(useGsm)
    initializeGsm();
    
  if(useHttp)
    initializeEthernet();
}

void loop()
{
  if(useHttp)
    httpPollContent();

  if(useGsm)
    gsmPollContent();

  if(useKeypad)
    keypadPoll();

  serialPollContent();
}

