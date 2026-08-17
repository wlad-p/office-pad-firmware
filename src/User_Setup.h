#define ILI9488_DRIVER 
#define TFT_WIDTH  480
#define TFT_HEIGHT 320 

#define TFT_MOSI  11  // Automatically assigned with ESP8266 if not defined
#define TFT_SCLK  10  // Automatically assigned with ESP8266 if not defined

#define TFT_CS    9  // Chip select control pin D8
#define TFT_DC    7  // Data Command control pin
#define TFT_RST   8  // Reset pin (could connect to NodeMCU RST, see next line)
#define TFT_BL    18
#define SPI_FREQUENCY  40000000