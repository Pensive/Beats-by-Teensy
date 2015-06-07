//***

IntervalTimer seqTimer;
//Metro seqMetro=Metro(5);

struct sequencerStruct
{
	// union arrangement to access pad statuses easily from one 16bit integer.
	//union{
		//volatile bool PAD[16];
		//volatile uint16_t PAD;
		//struct {
			//bool p1 : 1;
			//bool p2 : 1;
			//bool p3 : 1;
			//bool p4 : 1;
			//bool p5 : 1;
			//bool p6 : 1;
			//bool p7 : 1;
			//bool p8 : 1;
			//bool p9 : 1;
			//bool p10 : 1;
			//bool p11 : 1;
			//bool p12 : 1;
			//bool p13 : 1;
			//bool p14 : 1;
			//bool p15 : 1;
			//bool p16 : 1;
		//} pad;
	//} padStatus;
	//uint16_t padStatus; //supports status of up to 16 pads
	
	volatile bool padStatus[16];
	volatile bool pulseContent; // If there is _anything_ other than all zeroes in BOTH the padStatuses, this should be true.
	// velocity storage - 0-15 per pad, 
	union {
		uint32_t PV1to8;
		uint32_t PV9to16;
		struct {
			uint8_t  padVel1 : 4; // bit position 0..3
			uint8_t  padVel2 : 4; // bit positions 4..7
			uint8_t  padVel3 : 4; // bit positions 8..11
			uint8_t  padVel4 : 4; // bit positions 12..15
			uint8_t  padVel5 : 4; // bit position 16..19
			uint8_t  padVel6 : 4; // bit positions 20..23
			uint8_t  padVel7 : 4; // bit positions 24..27
			uint8_t  padVel8 : 4; // bit positions 28..31
		} pV1to8;
		struct {
			uint8_t  padVel9 : 4; // bit position 0..3
			uint8_t  padVel10 : 4; // bit positions 4..7
			uint8_t  padVel11 : 4; // bit positions 8..11
			uint8_t  padVel12 : 4; // bit positions 12..15
			uint8_t  padVel13 : 4; // bit position 16..19
			uint8_t  padVel14 : 4; // bit positions 20..23
			uint8_t  padVel15 : 4; // bit positions 24..27
			uint8_t  padVel16 : 4; // bit positions 28..31
		} pV9to16;
	} padVelocity;
};

typedef struct sequencerStruct sequencerStructure;
sequencerStructure sequencerCurrentBar[pulsesPerBar]; 

//******** Forward Declarations of all Functions ****************
void setupSequencer(void);
void loopSequencer(void);
void updateBPM(byte newBPM);
//***************************************************************
//functions

void updateBPM(byte newBPM) {
	byte newMs;
	int32_t newMicroSeconds;
	if (newBPM==0){
		seqTimer.end();
		Serial.println("Sequencer Halted");
		Serial.print(sequencerBPM);
		Serial.println("bpm");
		}
		else
		{
			//newMs = int16_t((float(float(60)/float(newBPM))/ppqn)*1000);
			newMicroSeconds = int32_t(((float(float(60)/float(newBPM))/ppqn)*1000)*1000);
			seqTimer.begin(loopSequencer, newMicroSeconds);
			Serial.print(sequencerBPM);
			Serial.println("bpm set");
			tick1 = pulsesPerBar / 4;
			tick2 = tick1 * 2;
			tick3 = tick2+tick1;			
		}
}

void setupSequencer(void)
{
	Serial.println("setup sequencer");
				//zero all data.
		for (int i = 0; i < (pulsesPerBar); i++) {
			sequencerCurrentBar[i].pulseContent = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;			
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			sequencerCurrentBar[i].padStatus[0] = false;
			//sequencerCurrentBar[i].padVelocity.pV1to8.padVel1 = 15;
			//sequencerCurrentBar[i].padVelocity.pV1to8.padVel2 = 15;
			//sequencerCurrentBar[i].padVelocity.pV1to8.padVel3 = 15;
			//sequencerCurrentBar[i].padVelocity.pV1to8.padVel4 = 15;
			//sequencerCurrentBar[i].padVelocity.pV1to8.padVel5 = 15;
			//sequencerCurrentBar[i].padVelocity.pV1to8.padVel6 = 15;
			//sequencerCurrentBar[i].padVelocity.pV1to8.padVel7 = 15;
			//sequencerCurrentBar[i].padVelocity.pV1to8.padVel8 = 15;
			//sequencerCurrentBar[i].padVelocity.pV9to16.padVel9 = 15;
			//sequencerCurrentBar[i].padVelocity.pV9to16.padVel10 = 15;
			//sequencerCurrentBar[i].padVelocity.pV9to16.padVel11 = 15;
			//sequencerCurrentBar[i].padVelocity.pV9to16.padVel12 = 15;
			//sequencerCurrentBar[i].padVelocity.pV9to16.padVel13 = 15;
			//sequencerCurrentBar[i].padVelocity.pV9to16.padVel14 = 15;
			//sequencerCurrentBar[i].padVelocity.pV9to16.padVel15 = 15;
			//sequencerCurrentBar[i].padVelocity.pV9to16.padVel16 = 15;
		}
		sequencerCurrentBar[1].padStatus[0] = true;
		sequencerCurrentBar[1].pulseContent = true;
	//setup the sequencer
	updateBPM(120);
	// beats per minute = quarter notes per minute
	// 120 beats per minute * 96 pulses per quarter note = 11520
	// 2 beats per second * 96 pulses per quarter note = 192 times per second
	// 1/192 = 0.0052083

}

void loopSequencerQuery(void)
{
	int16_t thisPulse;
	thisPulse=currentPulse;
	if (sequencerCurrentBar[thisPulse].pulseContent) {
		Serial.print("pulsecontent:");
		Serial.println(thisPulse);		
	}
}

void loopSequencer(void)
{
	int16_t thisPulse=currentPulse;
	if (sequencerCurrentBar[thisPulse].pulseContent) {
		Serial.print("pulsecontent:");
		Serial.println(thisPulse);
		if (sequencerCurrentBar[thisPulse].padStatus[0]) { pressPad(0);}
		if (sequencerCurrentBar[thisPulse].padStatus[1]) { pressPad(1);}
		if (sequencerCurrentBar[thisPulse].padStatus[2]) { pressPad(2);}
		if (sequencerCurrentBar[thisPulse].padStatus[3]) { pressPad(3);}
		if (sequencerCurrentBar[thisPulse].padStatus[4]) { pressPad(4);}
		if (sequencerCurrentBar[thisPulse].padStatus[5]) { pressPad(5);}
		if (sequencerCurrentBar[thisPulse].padStatus[6]) { pressPad(6);}
		if (sequencerCurrentBar[thisPulse].padStatus[7]) { pressPad(7);}
		if (sequencerCurrentBar[thisPulse].padStatus[8]) { pressPad(8);}
		if (sequencerCurrentBar[thisPulse].padStatus[9]) { pressPad(9);}
		if (sequencerCurrentBar[thisPulse].padStatus[10]) { pressPad(10);}
		if (sequencerCurrentBar[thisPulse].padStatus[11]) { pressPad(11);}
		if (sequencerCurrentBar[thisPulse].padStatus[12]) { pressPad(12);}
		if (sequencerCurrentBar[thisPulse].padStatus[13]) { pressPad(13);}
		if (sequencerCurrentBar[thisPulse].padStatus[14]) { pressPad(14);}
		if (sequencerCurrentBar[thisPulse].padStatus[15]) { pressPad(15);}
	}
		
	if (currentPulse>pulsesPerBar) {currentPulse=0; flashBacklightRGB(BacklightWhite);}
		else { currentPulse++; }
			
			
			if (playTick){
			if (currentPulse==0) {clickTrack.play(AudioSampleMainClick);}
				else if (currentPulse==tick1) {clickTrack.play(AudioSampleMinorClick);}
				else if (currentPulse==tick2) {clickTrack.play(AudioSampleMinorClick);}
				else if (currentPulse==tick3) {clickTrack.play(AudioSampleMinorClick);}
			}
			
			
					
		
			
			//if (currentPulse == 0) {clickTrack.play(AudioSampleMainClick);} else if(currentPulse=tick1)
				
			
		
}

