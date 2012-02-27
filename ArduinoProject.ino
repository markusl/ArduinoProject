/*
  ArduinoProject
 
 */

boolean useHttp = false;
boolean useGsm = true;
boolean useKeypad = false;
boolean useDhcp = false;

void setup()
{
  intializeSerial();
  if(useKeypad)
    initializeKeypad();
    
  if(useGsm)
    initializeGsm();
    
  if(useHttp)
    initializeEthernet(useDhcp);
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

