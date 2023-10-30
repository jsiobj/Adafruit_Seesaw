# Adafruit_Seesaw

Arduino driver for seesaw multi-use chip

> [!WARNING] 
> This is not the original Adafruit library. It was modified for my own needs

Additions :

#### Make LED cycle through red, blue, green, changing every 200ms
```C++
Adafruit_NeoTrellis trellis;

void setup() {
    trellis.begin();
    uint32_t COLOR_LIST = { 0xFF0000, 0x00FF00, 0x0000FF };
    uint8_t color_count = 3;
    uint16_t pixel = 6;
    uint32_t delay = 200;
    trellis.pixels.setPixelColor(pixel, COLOR_LIST, color_count, delay);
}

void loop() {
    trellis.showCycle();
}
```

#### Detection of long press
```C++
Adafruit_NeoTrellis trellis;

#define KEY 1

TrellisCallback onStandardPress(keyEvent event) {
    if(event.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
        // Do something when button is pressed
    }
    
    if(event.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
        if(!trellis.wasLongPressed(event.bit.NUM)) {
            // Do something when key was released and
            // the press was long
        }
        else {
            // Do something when key was released and
            // the press was not long
        }
    }
    return 0;
}

TrellisCallback onLongPress(keyEvent event) {
    // Do something right away after detecting the long press BEFORE the button is released
    return 0;
}

void setup () {
    trellis.begin()
    trellis.setLongPressDelay(200)
    trellis.registerCallback(KEY, onStandardPress);
    trellis.registerCustomCallback(KEY, onLongPress);
}
```

Check out the [documentation](https://adafruit.github.io/Adafruit_Seesaw/html/class_adafruit__seesaw.html) for a listing and explanation of other available methods!
