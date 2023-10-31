#define PREFER_SDFAT_LIBRARY 1
#include <Adafruit_NeoTrellis.h>

Adafruit_NeoTrellis trellis;

#define DEBUG
#ifdef DEBUG
    #define DEBUG_PRINTF(...) \
    { \
        char DEBUG_MSG_BUFFER[256]; \
        sprintf(DEBUG_MSG_BUFFER,__VA_ARGS__); \
        Serial.print(millis()); \
        Serial.print("|"); \
        Serial.print(__PRETTY_FUNCTION__); \
        Serial.print("|"); \
        Serial.print(DEBUG_MSG_BUFFER); \
        Serial.println(); \
        Serial.flush(); \
    }
#else
  #define DEBUG_PRINTF(...)
#endif

enum DIAG_BTN_ID {
  BTN_ID_LONG_PRESS1=0,
  BTN_ID_LONG_PRESS2,
  BTN_ID_LONG_PRESS3,
  BTN_ID_BLINK,
  BTN_ID_CYCLE,
};

// See https://www.w3schools.com/colors/colors_picker.asp
#define COLOR_RED        0xFF0000
#define COLOR_ORANGE     0xB34700
#define COLOR_YELLOW     0xFFFF00
#define COLOR_OLIVE      0x66DD00
#define COLOR_GREEN      0x008000
#define COLOR_AQUA       0x00FF66
#define COLOR_TEAL       0x00BFFF
#define COLOR_BLUE       0x0080FF
#define COLOR_NAVY       0x000080
#define COLOR_MAROON     0x800000
#define COLOR_PURPLE     0x800080
#define COLOR_PINK       0xFF66B3
#define COLOR_WHITE      0xFFFFFF
#define COLOR_DARKGREY   0x101010
#define COLOR_LIGHTGREY  0xA0A0A0
#define COLOR_BLACK      0x000000

uint32_t colors[] = { COLOR_BLUE, COLOR_RED, COLOR_ORANGE, COLOR_NAVY, COLOR_YELLOW, COLOR_PURPLE };

TrellisCallback longPress(keyEvent event) {
    DEBUG_PRINTF("Key event: EDGE[%d] NUM[%d] Reg[%x]",event.bit.EDGE, event.bit.NUM, event.reg);
    switch (event.bit.EDGE) {
        case SEESAW_KEYPAD_EDGE_RISING:
            trellis.pixels.setPixelColor(event.bit.NUM, COLOR_RED);
            DEBUG_PRINTF("Key %d pressed",event.bit.NUM);
            break;
        
        case SEESAW_KEYPAD_EDGE_FALLING:
            trellis.pixels.setBlink(BTN_ID_LONG_PRESS2, COLOR_RED, 200);
            trellis.pixels.setPixelColor(event.bit.NUM, COLOR_ORANGE);
            if(trellis.wasLongPressed(event.bit.NUM)) trellis.pixels.enableCycling(BTN_ID_LONG_PRESS3, false);
            DEBUG_PRINTF("Key %d released",event.bit.NUM);
            break;

        default:
            break;
    }
    return 0;
}

TrellisCallback duringLongPress(keyEvent event) {
    DEBUG_PRINTF("Key event: EDGE[%d] NUM[%d] Reg[%x]",event.bit.EDGE, event.bit.NUM, event.reg);
    trellis.pixels.setCycle(BTN_ID_LONG_PRESS3, colors, 6, 100);
    trellis.pixels.setCycle(event.bit.NUM, colors, 6, 100);
    return 0;
}

TrellisCallback blink(keyEvent event) {
    DEBUG_PRINTF("Key event: EDGE[%d] NUM[%d] Reg[%x]",event.bit.EDGE, event.bit.NUM, event.reg);
    switch (event.bit.EDGE) {
        case SEESAW_KEYPAD_EDGE_RISING:
            trellis.pixels.setPixelColor(event.bit.NUM, COLOR_RED);
            DEBUG_PRINTF("Key %d pressed",event.bit.NUM);
            break;
        
        case SEESAW_KEYPAD_EDGE_FALLING:
            trellis.pixels.enableCycling(event.bit.NUM, !trellis.pixels.isCyclingEnabled(event.bit.NUM));
            DEBUG_PRINTF("Key %d released",event.bit.NUM);
            break;

        default:
            break;
    }
    return 0;
}

TrellisCallback cycle(keyEvent event) {
    DEBUG_PRINTF("Key event: EDGE[%d] NUM[%d] Reg[%x]",event.bit.EDGE, event.bit.NUM, event.reg);
    switch (event.bit.EDGE) {
        case SEESAW_KEYPAD_EDGE_RISING:
            trellis.pixels.setPixelColor(event.bit.NUM, COLOR_RED);
            DEBUG_PRINTF("Key %d pressed",event.bit.NUM);
            break;
        
        case SEESAW_KEYPAD_EDGE_FALLING:
            trellis.pixels.enableCycling(event.bit.NUM, !trellis.pixels.isCyclingEnabled(event.bit.NUM));
            DEBUG_PRINTF("Key %d released",event.bit.NUM);
            break;

        default:
            break;
    }
    return 0;
}

TrellisCallback switchOff(keyEvent event) {
    DEBUG_PRINTF("Key event: EDGE[%d] NUM[%d] Reg[%x]",event.bit.EDGE, event.bit.NUM, event.reg);
    switch (event.bit.EDGE) {
        case SEESAW_KEYPAD_EDGE_RISING:
            trellis.pixels.setPixelColor(event.bit.NUM, 0);
            DEBUG_PRINTF("Key %d pressed",event.bit.NUM);
            break;
        
        case SEESAW_KEYPAD_EDGE_FALLING:
            DEBUG_PRINTF("Key %d released",event.bit.NUM);
            break;

        default:
            break;
    }
    return 0;
}

void setup() {

    Serial.begin(115200);
    while(!Serial);

    DEBUG_PRINTF("Setup");
    if (!trellis.begin()) {
        DEBUG_PRINTF("Could not start NeoPixel Trellis");
        return;
    }

    for(int i=0; i<NEO_TRELLIS_NUM_KEYS; i++){
        trellis.activateKey(i, SEESAW_KEYPAD_EDGE_HIGH,false);
        trellis.activateKey(i, SEESAW_KEYPAD_EDGE_LOW,false);
        trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING);
        trellis.activateKey(i, SEESAW_KEYPAD_EDGE_FALLING);
        trellis.registerCallback(i, switchOff);
        trellis.pixels.setPixelColor(i, COLOR_NAVY);
    }

    trellis.registerCallback(BTN_ID_BLINK, blink);
    trellis.registerCallback(BTN_ID_CYCLE, cycle);

    trellis.registerCallback(BTN_ID_LONG_PRESS1, longPress);
    trellis.registerCustomCallback(BTN_ID_LONG_PRESS1, duringLongPress);

    trellis.pixels.setBlink(BTN_ID_BLINK, COLOR_GREEN, 200);
    trellis.pixels.setCycle(BTN_ID_CYCLE, colors, 3 , 600);
}


// Main diag loop function
void loop() {
    trellis.read();
    trellis.pixels.showCycle();
}