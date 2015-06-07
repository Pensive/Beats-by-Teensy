// Pinout : Serial Data Out (SID), Serial Clock (SCLK), A0, Reset, Chip Select
ST7565 glcd(2, 1, 0, 3, 4);
#include "beatsbyteensy_glcd.h" // import the logo
#define LCD_LEDPIN_GREEN 21
#define LCD_LEDPIN_RED 5
#define LCD_LEDPIN_BLUE 20
// Red, Green Blue
#define BLRed 0
#define BLGrn 1
#define BLBlu 2
byte BacklightWhite[3] = {0,110,0}; // Flash white to BPM
byte BacklightMagenta[3] = {0,255,0}; // Mastering Mode
byte BacklightCyan[3] = {255,110,0}; // Menu and Settings
byte BacklightYellow[3] = {0,110,255}; //Bank D
byte BacklightRed[3] = {0,255,255}; //Bank A
byte BacklightGreen[3] = {255,0,255}; //Bank B
byte BacklightBlue[3] = {255,255,0}; //Bank C

const byte backlightFadeSteps = 10;
unsigned long backlightFadeLastMillis = 0;
unsigned int backlightFadeMillis = 15;

static float backlightRedStep,backlightGreenStep,backlightBlueStep;
static byte backlightCurrentStep;
static byte backlightCurrentRGB[3], backlightTargetRGB[3], backlightFlashRGB[3];
static bool backlightFading = false;
static bool backlightFlash = false;
static bool backlightFlashback = false;

//char potValueChar[5];



Metro lcdMetro = Metro(33);  // Instantiate an instance, 66ms = 15fps, 33ms = 30fps

//******** Forward Declarations of all Functions ****************
void setBacklightRGB(byte BLColour[3]);
void fadeBacklightRGB(byte BLColour[3]);
void flashBacklightRGB(byte BLColour[3]);
void updateBacklightFade();
void setupLCD(void);
void loopLCD(void);

//***************************************************************


void setBacklightRGB(byte BLColour[3])
{
	analogWrite(LCD_LEDPIN_RED, BLColour[BLRed]);
	analogWrite(LCD_LEDPIN_GREEN, BLColour[BLGrn]);
	analogWrite(LCD_LEDPIN_BLUE, BLColour[BLBlu]);
	
	backlightCurrentRGB[BLRed] = BLColour[BLRed];
	backlightCurrentRGB[BLGrn] = BLColour[BLGrn];
	backlightCurrentRGB[BLBlu] = BLColour[BLBlu];
}

void flashBacklightRGB(byte BLColour[3])
{
	backlightFlashRGB[BLRed] = BLColour[BLRed];
	backlightFlashRGB[BLGrn] = BLColour[BLGrn];
	backlightFlashRGB[BLBlu] = BLColour[BLBlu];
	
	//set flash in motion
	backlightFlash = true;
	backlightFlashback = false;
}

void fadeBacklightRGB(byte BLColour[3]) // calculates fading steps per colour, and sets fade in motion.
{
	backlightTargetRGB[BLRed] = BLColour[BLRed];
	backlightTargetRGB[BLGrn] = BLColour[BLGrn];
	backlightTargetRGB[BLBlu] = BLColour[BLBlu];
	
	backlightRedStep = (backlightTargetRGB[BLRed] - backlightCurrentRGB[BLRed]) / backlightFadeSteps;
	backlightGreenStep = (backlightTargetRGB[BLGrn] - backlightCurrentRGB[BLGrn]) / backlightFadeSteps;
	backlightBlueStep = (backlightTargetRGB[BLBlu] - backlightCurrentRGB[BLBlu]) / backlightFadeSteps;
	//set fading step
	backlightCurrentStep = 0;
	//set fade in motion
	backlightFading = true;
}

void updateBacklightFade() // this executes once per fade step
{
	if (backlightCurrentStep < backlightFadeSteps){
		backlightCurrentRGB[BLRed]+= backlightRedStep;
		backlightCurrentRGB[BLGrn]+= backlightGreenStep;
		backlightCurrentRGB[BLBlu]+=backlightBlueStep;
		setBacklightRGB(backlightCurrentRGB);
		backlightCurrentStep++;
		} else 	{
		setBacklightRGB(backlightTargetRGB);
		backlightFading = false;
	}
	backlightFadeLastMillis = millis();
	
}

void setupLCD(void)
{
	pinMode(LCD_LEDPIN_RED, OUTPUT);
	pinMode(LCD_LEDPIN_GREEN, OUTPUT);
	pinMode(LCD_LEDPIN_BLUE, OUTPUT);
	setBacklightRGB(BacklightCyan);
	glcd.begin(0x18);
	glcd.clear();
	glcd.drawbitmap(0, 0, beatsbyteensy_glcd_bmp, BEATSBYTEENSY_GLCD_WIDTH, BEATSBYTEENSY_GLCD_HEIGHT, BLACK);
	glcd.display();
	delay(1000);
	glcd.clear();
	
	// draw a string at location (0,0)
	glcd.drawstring(0, 0, "Beats by Teensy");
	glcd.drawstring(0, 1,"(C) J Payne 2014.");
	glcd.display();
	//delay(2000);
	
	//glcd.clear();
	// draw the first ~120 characters in the font
	//for (uint8_t i=0; i < 168; i++) {
	//	glcd.drawchar((i % 21) * 6, i/21, i);
	//}
	//glcd.display();
}

void loopLCD(void)
{
	int currentPad;
	int currentx;
	glcd.clear();

	glcd.drawline(0,2,127,2,BLACK);
	currentx = (float(currentPulse)/ float(pulsesPerBar))*128;
	//currentx = currentPulse / 3;
	glcd.fillrect(currentx,0,16,5,BLACK);
	
	glcd.drawline(0,10,127,10,BLACK);
	glcd.drawline(7,8,7,12,BLACK);
	
	//current sequence
	glcd.drawstring(0, 2, "SABCDEFGHIJKLMNOPQRST"); //20 SEQUENCES OF XX BARS
	
	char b[5];   //declaring character array
	String strx;  //declaring string
	
	strx=String(Pot1); //converting integer into a string
	strx.toCharArray(b,5); //passing the value of the string to the character array
	glcd.drawstring(0, 3, b); 
	
	strx=String(Pot2); //converting integer into a string
	strx.toCharArray(b,5); //passing the value of the string to the character array
	glcd.drawstring(0, 4, b);
	
	strx=String(Pot3); //converting integer into a string
	strx.toCharArray(b,5); //passing the value of the string to the character array
	glcd.drawstring(0, 5, b);
	
	strx=String(Pot4); //converting integer into a string
	strx.toCharArray(b,5); //passing the value of the string to the character array
	glcd.drawstring(0, 6, b);
	
	strx=String(sequencerBPM);
	strx.toCharArray(b,4);
	glcd.drawstring(42, 4, b);
	glcd.drawstring(66,4,"bpm");
	
	strx=String(posMasterKnob);
	strx.toCharArray(b,5);
	glcd.drawstring(42, 6, b);
	if (masterKnobPressed) {
		glcd.fillcircle(48,51,8,BLACK);
	}
	else
	{
		glcd.drawcircle(48,51,8,BLACK);
	}
	
		
	// 21	

	for (int y=0; y<4; y++) {
		for (int x=0; x<4; x++){
			currentPad = x + (y * 4);
			if (keyPressed[currentPad]) {
				glcd.fillcircle((x*10)+92,63 -((y*10)+5),4,BLACK);
			}
			else
			{
				glcd.drawcircle((x*10)+92,63 - ((y*10)+5),4,BLACK);
			}
		}
	}
	glcd.display();
	
	
	if (backlightFading) {
		if(millis() - backlightFadeLastMillis >= backlightFadeMillis) {
			updateBacklightFade();
		}
	}
	
	//if (backlightFlash){
		//setBacklightRGB(backlightFlashRGB);
		//backlightFlash=false;
		//backlightFlashback = true;
	//}
	//if (backlightFlashback){
		//setBacklightRGB(backlightCurrentRGB);
		//backlightFlashback = false;
	//}
	
}