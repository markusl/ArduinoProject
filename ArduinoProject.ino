/*
 * ArduinoProject main file.
 *
 * For controlling the device setup and main loop of the program.
 *
 * @date 02/2012
 * @author Markus Lindqvist
 */

// Whether to read Twitter tweets for updates
const boolean useHttp = false;
// Whether to use GPRS shield to receive updates
const boolean useGsm = true;
// Whether to use physical keypad for controlling the device
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

