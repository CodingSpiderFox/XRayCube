#include <Arduino.h>

// NeoPixelTest
// This example will cycle between showing four pixels as Red, Green, Blue, White
// and then showing those pixels as Black.
//
// Included but commented out are examples of configuring a NeoPixelBus for
// different color order including an extra white channel, different data speeds, and
// for Esp8266 different methods to send the data.
// NOTE: You will need to make sure to pick the one for your platform
//
//
// There is serial output of the current state so you can confirm and follow along
//

#include <NeoPixelBus.h>

const uint8_t PixelCount = 16; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

uint8_t BlockCount = 4, BlockSize = 4;
uint8_t redValue = 0;
uint8_t greenValue = 0;
uint8_t blueValue = 0;
uint8_t circleLedIds[3] =  {0,2,3};
uint8_t dotId = 1;

RgbColor black(0,0,0);

RgbColor layout[4]{
  RgbColor(255,0,0),
  RgbColor(0,255,0),
  RgbColor(0,0,255),
  RgbColor(255,255,255),
};

#define colorSaturation 128

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbFeature, Neo400KbpsMethod> strip(PixelCount, PixelPin);

// For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.
// There are other Esp8266 alternative methods that provide more pin options, but also have
// other side effects.
//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);
//
// NeoEsp8266Uart800KbpsMethod uses GPI02 instead

// You can also use one of these for Esp8266,
// each having their own restrictions
//
// These two are the same as above as the DMA method is the default
// NOTE: These will ignore the PIN and use GPI03 pin
//NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma400KbpsMethod> strip(PixelCount, PixelPin);

// Uart method is good for the Esp-01 or other pin restricted modules
// NOTE: These will ignore the PIN and use GPI02 pin
//NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart400KbpsMethod> strip(PixelCount, PixelPin);

// The bitbang method is really only good if you are not using WiFi features of the ESP
// It works with all but pin 16
//NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang400KbpsMethod> strip(PixelCount, PixelPin);

// four element pixels, RGBW
//NeoPixelBus<NeoRgbwFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

RgbColor * colors = new RgbColor[PixelCount];

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();


    Serial.println();
    Serial.println("Running...");
}


void loop()
{

    setCircleColor(&layout[0]);
    setCircleDotColor(&layout[1]);
    applyNewPixels();

    delay(2000);

    for (size_t i = 0; i <= sizeof(layout)/sizeof(layout[0]); i++) {
      layout[i] = RgbColor(random(0,255),random(0,255),random(0,255));
    }

    setColorsSimilarEachSide(layout);
    applyNewPixels();
}

void applyNewPixels() {
  for (size_t i = 0; i <= 16; i++) {
    strip.SetPixelColor(i, colors[i]);
    String a = "";
  }
  strip.Show();
}

void funnyRandomStuff() {

}

void setColorsSimilarEachSide(RgbColor* layout) {
  for (int i = 1; i <= 15; i+=BlockSize) {
    for (int j = 1; j <= BlockCount; j++) {
      colors[i+j-2] = layout[i*j/i-1];
    }
  }
}

void setCircleColor(RgbColor* color) {
  for (size_t i = 1; i < BlockCount*BlockSize; i+=BlockSize) {
    for (size_t j = 0; j < sizeof(circleLedIds)/sizeof(circleLedIds[0]); j++) {
      colors[i+circleLedIds[j]-1] = *color;
    }
  }
}

void setCircleDotColor(RgbColor* dotColor) {
  for (size_t i = 1; i < BlockCount*BlockSize; i+=BlockSize) {
    for (size_t j = 0; j < BlockCount; j+=BlockSize) {
      colors[i+dotId-1] = *dotColor;
    }
  }
}

void setSymbolColor(RgbColor* color) {
  setCircleDotColor(color);
  setCircleColor(color);
}
