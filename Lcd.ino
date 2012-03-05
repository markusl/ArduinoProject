/*
 * ArduinoProject LCD screen control.
 *
 * Provides methods for accessing the LCD display attached.
 *
 * @date 03/2012
 * @author Markus Lindqvist
 */
 // include the library code:
#include <LiquidCrystal.h>

/* LCD display attached to pins 10, 11, 12, 13 */
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int _lcdRows = 2;
const int _lcdColumns = 16;

void initializeLcd()
{
  lcd.begin(_lcdColumns, _lcdRows);
  debug2("Initializing device");
}

void lcdPrintString(String &line)
{
  lcd.setCursor(0, 1); // Go to start of second row
  lcd.print(line);
}

