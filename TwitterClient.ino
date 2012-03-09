/*
 * ArduinoProject HTTP/Twitter control.
 *
 * Functionality to receive updates via fetching Twitter feed.
 *
 * @date 02/2012
 * @author Markus Lindqvist
 */

#include <SPI.h>
#include <Ethernet.h>

const char _twitterServerName[] = "api.twitter.com";  // twitter API
String _twitterFetchUrl = "GET http://api.twitter.com/1/statuses/user_timeline.xml?include_entities=false&include_rts=false&count=1&screen_name=nS9mSKK0b89K HTTP/1.1";

// Changing string to store latest line read from HTPP connection
String _latestLine = "";
const int _maxLength = 256; // max length to store
const int _maxTweet = 150;
String latestTweet = "<nutin>";
int _latestTweetUpdate = 0;

// Settings for local tryouts
IPAddress ip(192, 168, 0, 17);
IPAddress gw(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/* Ethernet shield attached to pins 10, 11, 12, 13 */
EthernetClient client;

/** Initializes attached ethernet shield, tries DHCP and
  * fallbacks to hard-coded interface settings. */
void initializeEthernet(void (*lcdUpdate)(const String&))
{
  debug("initializeEthernet");

  // attempt a DHCP connection:
  if (!Ethernet.begin(mac)) {
    // if DHCP fails, start with a hard-coded address:
    Ethernet.begin(mac, ip);
  }
  
  // give the Ethernet shield a second to initialize:
  delay(1600);
  if(!_checkNetworkIsUp())
  {
    debug("Network seems to be down! :(");
  }
  {
    debug("Network up");
  }
  
  latestTweet.reserve(_maxTweet);
  _latestLine.reserve(_maxLength);
}

// Do HTTP request to fetch some content!
boolean _doRequest(const char* server, String url)
{
  debug("connecting...");
  if (client.connect(server, 80)) {
    debug("connected.. writing request..");
    debug(url);
    debug("Host: " + String(server));
    client.println(url);
    client.println("Host: " + String(server));
    client.println("Connection: close");
    client.println();
    return true;
  } 
  else {
    debug("connection failed");
    return false;
  } 
}

boolean _isEol() { return _latestLine.indexOf("\r\n") != -1; }

// Read a line until \r\n and store to _latestLine
void _httpReadLine()
{
  _latestLine = "";
  while(client.available() && !_isEol() &&
        _latestLine.length() < _maxLength)
  {
    char c = client.read();
    _latestLine += c;
  }
}

// Check _latestLine and store to latestTweet
// Returns true if a new tweet was found
boolean _storeLatestTweetIfFound()
{
    int textIdx = _latestLine.indexOf("<text>");
    int textEndIdx = _latestLine.indexOf("</text>");
    if(textIdx != -1 && textEndIdx != -1)
    {
      String currentTweet = _latestLine.substring(textIdx+6, textEndIdx);
      if(currentTweet != latestTweet)
      {
        latestTweet = currentTweet;
        debug("Found a NEW tweet! " + latestTweet);
        return true;
      }
      else
      {
        debug("Found the same tweet! " + latestTweet);
      }
      
      // In any case, stop the connection if we found <text>..</text>
      client.stop();
    }
    return false;
}

boolean _readContent(boolean readLatestTweet)
{
  while(client.connected())
  {
    _httpReadLine();
    _latestLine.trim();
    
    if(readLatestTweet)
    {
      boolean newTweet = _storeLatestTweetIfFound();
      if(newTweet)
        return true;
    }
    
    if(_latestLine.length() > 0)
    {
      debug("Got line> " + _latestLine);
    }
  }
  
  return false;
}

void httpPollContent()
{
  _doRequest(_twitterServerName, _twitterFetchUrl);
  if(_readContent(true))
  {
    _latestTweetUpdate = millis()/1000;
  }
  debug("disconnecting.");
  client.stop();
}

boolean _checkNetworkIsUp()
{
  boolean result = _doRequest(_twitterServerName, "GET / HTTP/1.1");
  client.flush();
  client.stop();
  return result;
}

