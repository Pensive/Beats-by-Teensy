/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Teensy 3.1, Platform=avr, Package=teensy
*/

#define ARDUINO 106
#define ARDUINO_MAIN
#define __AVR__
#define F_CPU 96000000
#define USB_SERIAL
#define LAYOUT_US_ENGLISH
#define __MK20DX256__
#define TEENSYDUINO 120
#define F_CPU 96000000
#define USB_SERIAL
#define LAYOUT_US_ENGLISH
extern "C" void __cxa_pure_virtual() {;}

void pressPad(int PadNumber);
void releasePad(int PadNumber);
void refreshPad();
void setupPads();
//
void printStatusReport();
//

#include "C:\Program Files (x86)\Arduino\hardware\teensy\cores\teensy3\arduino.h"
#include <beatsbyteensy.ino>
#include <AudioSampleMainclik.cpp>
#include <AudioSampleMainclik.h>
#include <AudioSampleMinrclik.cpp>
#include <AudioSampleMinrclik.h>
#include <bbt_audio.h>
#include <bbt_file.h>
#include <bbt_input.h>
#include <bbt_lcd.h>
#include <bbt_ps2input.h>
#include <bbt_seq.h>
#include <bbt_synth.h>
#include <beatsbyteensy_glcd.h>
#include <play_serialflash.cpp>
#include <play_serialflash.h>
