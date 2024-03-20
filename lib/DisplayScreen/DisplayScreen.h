// #ifndef DISPLAYSCREEN_H
// #define DISPLAYSCREEN_H

// #define TFT_CS  34
// #define TFT_RST  35
// #define TFT_DC   33

// #include <Adafruit_GFX.h>
// #include <Adafruit_ST7735.h>

// // Color definitions
// #define BLACK    0x0000
// #define BLUE     0x001F
// #define RED      0xF800
// #define GREEN    0x07E0
// #define CYAN     0x07FF
// #define MAGENTA  0xF81F
// #define YELLOW   0xFFE0 
// #define WHITE    0xFFFF


// class DisplayScreen{
// public:

//     DisplayScreen() {
//         this->screen = new Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
//     }

//     void printTo(String text){
//                 // initialise the display
//         screen->setFont();
//         screen->fillScreen(BLACK);
//         screen->setTextColor(MAGENTA);
//         screen->setTextSize(1);
        
//         // draw punchline
//         screen->setCursor(0,0);
//         screen->print(text);
//         delay(12000);

//         // home the cursor
//         screen->setCursor(0,0);
        
//         // change the text color to background color
//         // to erase setup
//         screen->setTextColor(BLACK);
//         screen->print(text);

//         // home the cursor
//         screen->setCursor(0,0);
//         screen->setTextColor(MAGENTA);
//         screen->print(text);

//         screen->fillScreen(BLACK);
//     }


// private:

// Adafruit_ST7735 * screen;


// };
// #endif 