#include <IRremote.h>
#include <Adafruit_NeoPixel.h>
#include <TimerOne.h>

#define PIN 6
#define TIMER_US 500

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

byte RNB = false;
uint16_t ii = 0, jj = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);

/***********************************************************************************************
 *                                               Code Setup                                    *
 ***********************************************************************************************/

void setup()
{

  Timer1.initialize(TIMER_US);                  // Initialise timer 1
  Timer1.attachInterrupt( timerIsr );
  irrecv.enableIRIn(); // Start the receiver
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


/***********************************************************************************************
 *                                             Code Loop                                       *
 ***********************************************************************************************/
 
void loop() {
  static byte R, G, B;
  static bool Update = true;

  if (irrecv.decode(&results))
  {
    RNB = false;
    switch (results.value)
    {
      case 0xFFA25D: //1
        Update = true;
        R = 255, G = 0, B = 0;
        break;

      case 0xFF629D: //2
        Update = true;
        R = 0, G = 255, B = 0;
        break;

      case 0xFFE21D: //3
        Update = true;
        R = 0, G = 0, B = 255;
        break;

      case 0xFF22DD: //4
        Update = true;
        R = 127, G = 127, B = 127;
        break;

      case 0xFF02FD: //5
        Update = true;
        R = 255, G = 255, B = 0;
        break;

      case 0xFFC23D: //6
        Update = true;
        R = 255, G = 215, B = 0;
        break;

      case 0xFFE01F: //7
        Update = true;
        R = 205, G = 92, B = 92;
        break;

      case 0xFFA857: //8
        Update = true;
        R = 255, G = 20, B = 147;
        break;

      case 0xFF906F: //9
        Update = true;
        R = 139, G = 69, B = 19;
        break;

      case 0xFF6897: //*
        Timer1.initialize(5000);
        RNB = true;
        break;

      case 0xFF9867: //0
        R = 0, G = 10, B = 0;
        break;

      case 0xFFB04F: //#
        RNB = true;
        Timer1.initialize(30000);
        break;

      case 0xFF18E7://UP
        break;

      case 0xFF10EF: //LEFT"
        break;

      case 0xFF38C7: //OK
        break;

      case 0xFF5AA5: //RIGHT
        break;

      case 0xFF4AB5: //DOWN"
        break;

      case 0xFF52AD: //#
        break;
      case 0xFFFFFFFF://REPEAT
        break;

      default:
      break;
    }// End Case
    irrecv.resume(); // Receive the next value
  }

  if (!RNB)colorWipe(strip.Color(R, G, B), 4); // Red
  delay(700);
}

/***********************************************************************************************
 *                                               Wipe Colors                                   *
 ***********************************************************************************************/
void colorWipe(uint32_t c, uint8_t wait)
{
  // Fill the dots one after the other with a color
  for (byte PixelNumber = 0; PixelNumber < strip.numPixels(); PixelNumber++)
  {
    strip.setPixelColor(PixelNumber, c);
    strip.show();
    delay(wait);
  }
}


/***********************************************************************************************
 *                                               Rainbow                                       *
 ***********************************************************************************************/
void rainbow()
{
  strip.setPixelColor(ii, Wheel((ii + jj) & 255));
  strip.show();
  ii++;
  if (ii >= strip.numPixels())
  {
    jj++;
    ii = 0;
    if (jj >= 256)jj = 0;
  }
}


/***********************************************************************************************
 *                                               Wheel                                         *
 ***********************************************************************************************/
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else
  {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


/***********************************************************************************************
 *                                               Timer interrupt                               *
 ***********************************************************************************************/
void timerIsr()
{
  if (RNB)rainbow();
}
