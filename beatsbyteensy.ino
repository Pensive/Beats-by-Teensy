// ****************** Core Libraries ****************************
#include <Audio.h>				// Audio Library
#include <Wire.h>				// I2C Access Library
#include <SPI.h>				// SPI access library
#include <SD.h>					// SD Card access library
#include <Bounce.h>				// used for debouncing directly connected buttons (masterKnob push).
#include "ST7565.h"				// LCD Display Library from Adafruit
#include "PS2MiniPac.h"			// Custom adapation of PS2Keyboard library
#include <flash_spi.h>			// Affords access to FLASH ROM
#include "play_serialflash.h"	// Plays samples rom FLASH rom
#include <Encoder.h>			// Rotary Encoder - Quadrature GRAY code switch
#include <Metro.h>			// Metro non-interrupt timing library for low priority functions like LCD update.
#include "AudioSampleMainclik.h" //import click Major sample
#include "AudioSampleMinrclik.h" // import Minor click sample
// ****************** Core Libraries ****************************

//******** Forward Declarations of all Functions ****************
void pressPad(int PadNumber);
void releasePad(int PadNumber);
void printStatusReport();
void setupPads();
//***************************************************************

byte padMode; //current mode the pads are in: 0 = Sampler; 1= Beatmasher; 2 = Synth
const byte padModeSampler = 0;
const byte padModeLooper = 1;
const byte padModeSynthA = 2;
const byte padModeSynthB = 3;


volatile bool keyPressed[32];
volatile bool keyPressedThisCycle;
bool masterKnobPressed = false;
bool masterKnobReleased = true;
uint32_t Pot1,Pot2,Pot3,Pot4, Fader, Volpot;
volatile int16_t currentPulse;
volatile long posMasterKnob  = -999;
///Seq variables
const int16_t pulsesPerBar=32; //96ppqn = 384 (4/4 = 4 * 1/4 note beat to a bar) = 4 * 96 = 384
byte ppqn = 8;
int16_t lastPulse = 0;
volatile byte sequencerBPM = 120; //120bpm
volatile byte beatsperbar = 4; //  >>>> 4/4 time signature
volatile byte notes = 4;       //       4/4 <<< time signature
volatile bool seqPlay, seqRec, seqOverdub,seqQuantize, seqUndo = false;
byte synthACurrentNote = 255;  // MIDI note number of currently playing note, 0 - 127 or 255 for no note
int16_t tick1,tick2,tick3;
bool playTick = false;

//  /seq variables


// ****************** Code Units ********************************
// LCD
#include "bbt_lcd.h"
// Audio
#include "bbt_audio.h"
// File Access
#include "bbt_file.h"
// PS/2 MiniPac Button interface
#include "bbt_ps2input.h"
// Sequencer
#include "bbt_seq.h"
// Analog POTs, Encoders, Piezo's and Directly connected push buttons
#include "bbt_input.h"
// Synth Module
#include "bbt_synth.h"


// ****************** Program Declarations **********************




struct padSettingStruct
{
	bool Playing; // true = playing
	bool ReleaseAction; // true = pay attention to pad release, false = ignore it.
	bool Released; // true = key is released but still playing
	bool loopState; // false = OneShot, true = Loop
	byte loopIteration; // 0 = infinite, 1-254 = quantity of times to loop
	uint32_t loopStartPoint; // default  0 - start.
	uint32_t loopStopPoint; // default  0 - end.
	uint32_t sampleLength;
	byte loopCurrentIteration; // keeps track of current loop iteration
	byte Velocity; // velocity of 0-255
};

typedef struct padSettingStruct padSettingStructure;

padSettingStructure padSetting[16];

unsigned long SerialMillisecondCounter = 0;
unsigned int statusReportInterval = 10000; //every 10 seconds, print the status report to serial

// Procedures

void pressPad(int PadNumber)
{
	switch (padMode) {
		case padModeSampler:
			PadFilenames[PadNumber].toCharArray(PadFilenameChar, PadFilenames[PadNumber].length() + 1);
			AudioNoInterrupts();
			Serial.print("Flash offset: ");
			Serial.println(PadFlashOffset[PadNumber]);
			Pads[PadNumber]->play(PadFlashOffset[PadNumber]);
			padSetting[PadNumber].sampleLength = Pads[PadNumber]->lengthMillis();
			Envelopes[PadNumber]->noteOn();
			padSetting[PadNumber].Playing = true;
			AudioInterrupts();
			Serial.print("Playing Pad ");
			Serial.println(PadNumber+1);
		break;
		case padModeLooper:
		//do something when var equals 2
		break;
		case padModeSynthA:
			synthANoteOn(0,synthAPadNotes[PadNumber],0);
		break;
		case padModeSynthB:
		break;
		//default:
		// if nothing else matches, do the default
		// default is optional
	}
	
	
}

void releasePad(int PadNumber)
{
	switch (padMode) {
		case padModeSampler:
			AudioNoInterrupts();
			if (!padSetting[PadNumber].ReleaseAction) { return; } //this skips any keyup commands.
			Envelopes[PadNumber]->noteOff(); // the note begins its release cycle, but is not yet stopped. playing status is picked up
			padSetting[PadNumber].Released = true;
			AudioInterrupts();
			Serial.print("Stopped Pad ");
			Serial.println(PadNumber+1);
		break;	
		case padModeLooper:
		//do something when var equals 2
		break;
		case padModeSynthA:
			synthANoteOff(0,synthAPadNotes[PadNumber],0);
		break;
		case padModeSynthB:
		break;
		//default:
		// if nothing else matches, do the default
		// default is optional
			}
}

void refreshPad() //implements looping and keeps play statuses up to date when stopPad cannot
{
	for (int i=0; i<16; i++) {
		
		
		if (padSetting[i].loopState = true) //1. IS it looping?
		{
			if (Pads[i]->positionMillis() > 1000){
				Serial.println(Pads[i]->positionMillis());
				//Pads[i]->play(PadFlashOffset[i]);
				//envelopes stuff to add
			}
			break;
		}
		else //not looping, check if sample has finally stopped, then update variable.
		if (padSetting[i].Playing) //2. Not looping, but still set as playing
		{
			if (! (Pads[i]->isPlaying())) { padSetting[i].Playing=false;}
			break;
		}
	}
}

void setupPads()
{
	padMode = 0; //Sampler by default
	
	for (int i=0; i<16; i++) {
		padSetting[i].Playing = false;
		padSetting[i].ReleaseAction = true;
		padSetting[i].Released = false;
		padSetting[i].loopState = true;
		padSetting[i].loopIteration = 0;
		padSetting[i].loopStartPoint = 0;
		padSetting[i].loopStopPoint = 0;
		padSetting[i].sampleLength = 0;
		padSetting[i].loopCurrentIteration = 0;
		padSetting[i].Velocity = 255;
		
		Serial.print(i);
		Serial.print(" : ");
		Serial.print(playRaw8.positionMillis());
		Serial.print(", ");
		Serial.println(playRaw8.lengthMillis());
		
	}
	
}


void setup()
{
	Serial.begin(9600); // open the serial bus for debug feedback
	Serial.println("Setup Process Beginning");
	setupLCD();
	setupAudio();
	setupInput();
	setupFileAccess();
	setupMiniPac();
	setupPads();
	setupSynth();
	setupSequencer();
	Serial.println("Setup Process Completed");
}

void printStatusReport()
{
	Serial.print("CPU: ");
	Serial.print(AudioProcessorUsage());
	Serial.print("% (");
	Serial.print(AudioProcessorUsageMax());
	Serial.print("% Peak), RAM: ");
	Serial.print((float)(AudioMemoryUsage()*256)/1024); //1 block = 128 samples @ 16 bits  = 256 Bytes. Divide by 1024 for Kb
	Serial.print("Kb (");
	Serial.print((float)(AudioMemoryUsageMax()*256)/1024); //1 block = 128 samples @ 16 bits  = 256 Bytes. Divide by 1024 for Kb
	Serial.println("Kb Peak) of 64Kb");
	SerialMillisecondCounter = millis();
	AudioProcessorUsageMaxReset();
	AudioMemoryUsageMaxReset();	
}

void loop()
{
	
	if(millis() - SerialMillisecondCounter >= statusReportInterval) {
		printStatusReport();
	}
	
	loopMiniPac();
	loopInput();
	loopSynthA();
	//loopSequencerQuery();
	
	if (lcdMetro.check() == 1) { 
		loopLCD();
	}
		
	refreshPad();
}

