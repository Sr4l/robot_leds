#include "Adafruit_NeoPixel.h"

/* *************************************
 * 
 * ************************************ */
 
// sine look-up table
uint8_t  sine_wave[256] = {
  0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
  0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
  0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
  0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
  0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
  0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
  0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
  0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
  0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
  0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
  0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
  0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
  0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
  0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
  0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
  0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
  0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
  0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
  0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
  0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
  0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
  0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
  0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
  0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
  0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
  0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
  0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
  0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
  0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
  0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
};

/* *************************************
 * convert HSV to RGB, not working (bug somewhere)
 * ************************************ */
uint32_t HSV2RGB(uint8_t h, uint8_t s, uint8_t v)
{
  uint32_t rgb;
  uint8_t r, g, b;
  uint16_t region, fpart;
  uint16_t p, q, t;
    
  /* make hue 0-5 */
  region = h / 43;
  /* find remainder part, make it from 0-255 */
  fpart = (h - (region * 43)) * 6;
  
  /* calculate temp vars, doing integer multiplication */
  p = (v * (255 - s)) >> 8;
  q = (v * (255 - ((s * fpart) >> 8))) >> 8;
  t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;
      
  /* assign temp vars based on color cone region */
  switch(region) {
    case 0:
      r = v; g = t; b = p; break;
    case 1:
      r = q; g = v; b = p; break;
    case 2:
      r = p; g = v; b = t; break;
    case 3:
      r = p; g = q; b = v; break;
    case 4:
      r = t; g = p; b = v; break;
    default:
      r = v; g = p; b = q; break;
  }
  
  rgb = r;
  rgb = (rgb << 8) + g;
  rgb = (rgb << 8) + b;
  
  return rgb;
}

 
class LedStripe
{
  public:
    LedStripe(uint16_t leds_count, uint8_t digital_pin_number);
    ~LedStripe();
    void update(uint8_t anim, uint8_t anip, uint8_t p1, uint8_t p2, uint8_t p3);
    void cycle();
    
    void setAllLeds(uint8_t local_V);
    void setLedsZoom();
    void setLedsZoomPingPong();
    
 
  private:
    Adafruit_NeoPixel* stripe;
    uint16_t num_leds;
    uint8_t digital_pin;
  
    uint8_t animation_mode = 0;
    uint8_t animation_parameter = 0;
    uint8_t H = 0;
    uint8_t S = 0;
    uint8_t V = 0;
    
    uint8_t animation_step = 0;
    unsigned long previousMillis = 0;
};

LedStripe::LedStripe(uint16_t leds_count, uint8_t digital_pin_number)
{
  num_leds = leds_count;
  digital_pin = digital_pin_number;
  stripe =  new Adafruit_NeoPixel(num_leds, digital_pin, NEO_GRB + NEO_KHZ800);
  stripe->begin();
  //stripe->show();
}
 
LedStripe::~LedStripe()
{
  delete stripe;
}


void LedStripe::update(uint8_t anim, uint8_t anip, uint8_t p1, uint8_t p2, uint8_t p3)
{
  animation_mode = anim;
  animation_parameter = anip;
  H = p1;
  S = p2;
  V = p3;
  
  // USB2 over current protection
  if (V > 128) V = 128;
}

void LedStripe::cycle()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= animation_parameter)
  {
    previousMillis = currentMillis;
    if (animation_mode == 0) setAllLeds(V);
    else if (animation_mode == 1) setAllLeds(sine_wave[animation_step]);
    else if (animation_mode == 2) setLedsZoom();
    else if (animation_mode == 3) setLedsZoomPingPong();
    animation_step += 1;
    stripe->show();
  }
}

void LedStripe::setAllLeds(uint8_t local_V)
{ 
  for (uint16_t i = 0; i < num_leds; i++)
  {
    stripe->setPixelColor(i, HSV2RGB(H, S, local_V));
  }
}

void LedStripe::setLedsZoom()
{
  uint8_t sel = 0;
  for (uint16_t i = 0; i < num_leds/2; i++)
  {
    sel = (-64+(animation_step + i*(381/num_leds))) % 255;
    stripe->setPixelColor(i, HSV2RGB(H, S, sine_wave[sel]));
    stripe->setPixelColor(num_leds-i, HSV2RGB(H, S, sine_wave[sel]));
  }
}

void LedStripe::setLedsZoomPingPong()
{
  // todo not working
  uint16_t sel = 0;
  uint8_t offset= 0;
  if (animation_step >= 128) offset = 255;
    
  for (uint16_t i = 0; i < num_leds/2; i++)
  {
    if (animation_step < 128)
      sel = (-64+animation_step*2 + i*(381/num_leds)) % 255;
    else
      sel = ((-64+ 255-animation_step*2 + i*(381/num_leds))) % 255;
      
    stripe->setPixelColor(i, HSV2RGB(H, S, sine_wave[sel]));
    stripe->setPixelColor(num_leds-i, HSV2RGB(H, S, sine_wave[sel]));
  }
}

