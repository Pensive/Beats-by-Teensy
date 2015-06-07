// GUItool: begin automatically generated code
//AudioPlaySdRaw           playRaw8;       //xy=337,745
//AudioPlaySdRaw           playRaw9;       //xy=337,787
//AudioPlaySdRaw           playRaw10;      //xy=337,831
//AudioPlaySdRaw           playRaw11;      //xy=337,872
//AudioPlaySdRaw           playRaw7;       //xy=338,696
//AudioPlaySdRaw           playRaw6;       //xy=339,652
//AudioPlaySdRaw           playRaw2;       //xy=340,454
//AudioPlaySdRaw           playRaw5;       //xy=340,602
//AudioPlaySdRaw           playRaw1;       //xy=341,403
//AudioPlaySdRaw           playRaw14;      //xy=339,1001
//AudioPlaySdRaw           playRaw3;       //xy=341,502
//AudioPlaySdRaw           playRaw4;       //xy=341,551
//AudioPlaySdRaw           playRaw16;      //xy=339,1088
//AudioPlaySdRaw           playRaw15;      //xy=340,1046
//AudioPlaySdRaw           playRaw12;      //xy=341,915
//AudioPlaySdRaw           playRaw13;      //xy=341,957

AudioPlayMemory				   clickTrack;

AudioPlaySerialFlash           playRaw8;       //xy=337,745
AudioPlaySerialFlash           playRaw9;       //xy=337,787
AudioPlaySerialFlash           playRaw10;      //xy=337,831
AudioPlaySerialFlash           playRaw11;      //xy=337,872
AudioPlaySerialFlash           playRaw7;       //xy=338,696
AudioPlaySerialFlash           playRaw6;       //xy=339,652
AudioPlaySerialFlash           playRaw2;       //xy=340,454
AudioPlaySerialFlash           playRaw5;       //xy=340,602
AudioPlaySerialFlash           playRaw1;       //xy=341,403
AudioPlaySerialFlash           playRaw14;      //xy=339,1001
AudioPlaySerialFlash           playRaw3;       //xy=341,502
AudioPlaySerialFlash           playRaw4;       //xy=341,551
AudioPlaySerialFlash           playRaw16;      //xy=339,1088
AudioPlaySerialFlash           playRaw15;      //xy=340,1046
AudioPlaySerialFlash           playRaw12;      //xy=341,915
AudioPlaySerialFlash           playRaw13;      //xy=341,957

AudioPlaySerialFlash *Pads[16] = { //package them up in easier format for future works
	&playRaw1, &playRaw2, &playRaw3, &playRaw4,
	&playRaw5, &playRaw6, &playRaw7, &playRaw8,
	&playRaw9, &playRaw10, &playRaw11, &playRaw12,
	&playRaw13, &playRaw14, &playRaw15, &playRaw16
};

AudioEffectEnvelope      envelopePadsA1;      //xy=493,405
AudioEffectEnvelope      envelopePadsA2;      //xy=493,453
AudioEffectEnvelope      envelopePadsA3;      //xy=492,504
AudioEffectEnvelope      envelopePadsA4;      //xy=490,552
AudioEffectEnvelope      envelopePadsA5;      //xy=489,603
AudioEffectEnvelope      envelopePadsA6;      //xy=488,652
AudioEffectEnvelope      envelopePadsA7;      //xy=488,696
AudioEffectEnvelope      envelopePadsA8;      //xy=488,746
AudioEffectEnvelope      envelopePadsA9;      //xy=490,789
AudioEffectEnvelope      envelopePadsA10;     //xy=491,832
AudioEffectEnvelope      envelopePadsA11;     //xy=492,874
AudioEffectEnvelope      envelopePadsA12;     //xy=492,914
AudioEffectEnvelope      envelopePadsA13;     //xy=490,957
AudioEffectEnvelope      envelopePadsA14;     //xy=490,1002
AudioEffectEnvelope      envelopePadsA15;     //xy=488,1047
AudioEffectEnvelope      envelopePadsA16;     //xy=486,1089

AudioEffectEnvelope *Envelopes[16] = { //package them up in easier format for future works
	&envelopePadsA1, &envelopePadsA2, &envelopePadsA3, &envelopePadsA4,
	&envelopePadsA5, &envelopePadsA6, &envelopePadsA7, &envelopePadsA8,
	&envelopePadsA9, &envelopePadsA10, &envelopePadsA11, &envelopePadsA12,
	&envelopePadsA13, &envelopePadsA14, &envelopePadsA15, &envelopePadsA16
};

// SynthA Objects
AudioSynthWaveformSine   synthASine1;          //xy=86,359
AudioSynthWaveform       synthAWaveform;      //xy=89,302
AudioSynthWaveformSine   synthASine2;          //xy=90,419
AudioEffectMultiply      synthAMultiply;      //xy=229,389
AudioEffectEnvelope      synthAEnvelope1;      //xy=379,388
AudioFilterStateVariable synthAFilter;        //xy=383,309
AudioEffectEnvelope      synthAEnvelope2;      //xy=547,295
// End SynthA Objects

// SynthB Objects
AudioSynthWaveform       synthBWaveform1;   
AudioSynthWaveform       synthBWaveform2; 
AudioSynthWaveform       synthBWaveform3; 
AudioMixer4				 synthBWaveformMixer;

AudioSynthNoiseWhite     synthBWhiteNoise;         
AudioSynthNoisePink      synthBPinkNoise;        
AudioMixer4				 synthBNoiseMixer;

AudioMixer4				 synthBFullMixer;
AudioEffectMultiply      synthBMultiply;

AudioEffectEnvelope      synthBEnvelope;      
AudioFilterStateVariable synthBFilter;       
// End SynthB Objects

AudioMixer16              mixerPadsALeft;         //xy=1079,474
AudioMixer16              mixerPadsARight;        //xy=1084,761

AudioMixer4              mixer11;        //xy=1990,539
AudioMixer4              mixer12;        //xy=1991,606
AudioMixer4              mixer14;        //xy=1995,830
AudioMixer4              mixer15;        //xy=1995,894
AudioSynthNoiseWhite     noise1;         //xy=2000,675
AudioSynthNoiseWhite     noise2;         //xy=2011,965
AudioSynthNoisePink      pink1;          //xy=2034,711
AudioSynthNoisePink      pink2;          //xy=2048,1015
AudioMixer4              mixer13;        //xy=2200,581
AudioMixer4              mixer16;        //xy=2200,829
AudioOutputI2S           i2s1;           //xy=2370,717

AudioConnection          patchCord1(playRaw1, envelopePadsA1);
AudioConnection          patchCord2(playRaw2, envelopePadsA2);
AudioConnection          patchCord3(playRaw3, envelopePadsA3);
AudioConnection          patchCord4(playRaw4, envelopePadsA4);
AudioConnection          patchCord5(playRaw5, envelopePadsA5);
AudioConnection          patchCord6(playRaw6, envelopePadsA6);
AudioConnection          patchCord7(playRaw7, envelopePadsA7);
AudioConnection          patchCord8(playRaw8, envelopePadsA8);
AudioConnection          patchCord9(playRaw9, envelopePadsA9);
AudioConnection          patchCord10(playRaw10, envelopePadsA10);
AudioConnection          patchCord11(playRaw11, envelopePadsA11);
AudioConnection          patchCord12(playRaw12, envelopePadsA12);
AudioConnection          patchCord13(playRaw13, envelopePadsA13);
AudioConnection          patchCord14(playRaw14, envelopePadsA14);
AudioConnection          patchCord15(playRaw15, envelopePadsA15);
AudioConnection          patchCord16(playRaw16, envelopePadsA16);


AudioConnection          patchCord57(envelopePadsA1, 0, mixerPadsALeft, 0);
AudioConnection          patchCord58(envelopePadsA1, 0, mixerPadsARight, 0);
AudioConnection          patchCord59(envelopePadsA2, 0, mixerPadsALeft, 1);
AudioConnection          patchCord60(envelopePadsA2, 0, mixerPadsARight, 1);
AudioConnection          patchCord49(envelopePadsA3, 0, mixerPadsALeft, 2);
AudioConnection          patchCord50(envelopePadsA3, 0, mixerPadsARight, 2);
AudioConnection          patchCord51(envelopePadsA4, 0, mixerPadsALeft, 3);
AudioConnection          patchCord52(envelopePadsA4, 0, mixerPadsARight, 3);
AudioConnection          patchCord53(envelopePadsA5, 0, mixerPadsALeft, 4);
AudioConnection          patchCord54(envelopePadsA5, 0, mixerPadsARight, 4);
AudioConnection          patchCord55(envelopePadsA6, 0, mixerPadsALeft, 5);
AudioConnection          patchCord56(envelopePadsA6, 0, mixerPadsARight, 5);
AudioConnection          patchCord63(envelopePadsA7, 0, mixerPadsALeft, 6);
AudioConnection          patchCord64(envelopePadsA7, 0, mixerPadsARight, 6);
AudioConnection          patchCord61(envelopePadsA8, 0, mixerPadsALeft, 7);
AudioConnection          patchCord62(envelopePadsA8, 0, mixerPadsARight, 7);
AudioConnection          patchCord65(envelopePadsA9, 0, mixerPadsALeft, 8);
AudioConnection          patchCord66(envelopePadsA9, 0, mixerPadsARight, 8);
AudioConnection          patchCord67(envelopePadsA10, 0, mixerPadsALeft, 9);
AudioConnection          patchCord68(envelopePadsA10, 0, mixerPadsARight, 9);
AudioConnection          patchCord69(envelopePadsA11, 0, mixerPadsALeft, 10);
AudioConnection          patchCord70(envelopePadsA11, 0, mixerPadsARight, 10);
AudioConnection          patchCord75(envelopePadsA12, 0, mixerPadsALeft, 11);
AudioConnection          patchCord76(envelopePadsA12, 0, mixerPadsARight, 11);
AudioConnection          patchCord77(envelopePadsA13, 0, mixerPadsALeft, 12);
AudioConnection          patchCord78(envelopePadsA13, 0, mixerPadsARight, 12);
AudioConnection          patchCord71(envelopePadsA14, 0, mixerPadsALeft, 13);
AudioConnection          patchCord72(envelopePadsA14, 0, mixerPadsARight, 13);
AudioConnection          patchCord73(envelopePadsA15, 0, mixerPadsALeft, 14);
AudioConnection          patchCord74(envelopePadsA15, 0, mixerPadsARight, 14);

AudioConnection          patchCord79(envelopePadsA16, 0, mixerPadsALeft, 15);
AudioConnection          patchCord80(envelopePadsA16, 0, mixerPadsARight, 15);

AudioConnection          patchCord81(mixerPadsALeft, 0, mixer13, 0);
AudioConnection          patchCord82(mixerPadsARight, 0, mixer16, 0);

//************* Synth A PAtchCords ***********************
AudioConnection          patchCord200(synthASine1, 0, synthAMultiply, 0);
AudioConnection          patchCord201(synthAWaveform, 0, synthAFilter, 0);
AudioConnection          patchCord203(synthASine2, 0, synthAMultiply, 1);
AudioConnection          patchCord204(synthAMultiply, synthAEnvelope1);
AudioConnection          patchCord205(synthAEnvelope1, 0, synthAFilter, 1);
AudioConnection          patchCord206(synthAFilter, 0, synthAEnvelope2, 0);
AudioConnection          patchCord207(synthAEnvelope2, 0, mixer13, 1);
AudioConnection          patchCord208(synthAEnvelope2, 0, mixer16, 1);
//********************************************************

//************* Synth B PAtchCords ***********************
AudioConnection          patchCord250(synthBWaveform1, 0, synthBWaveformMixer, 0);
AudioConnection          patchCord251(synthBWaveform2, 0, synthBWaveformMixer, 1);
AudioConnection          patchCord252(synthBWaveform3, 0, synthBWaveformMixer, 2);
AudioConnection          patchCord253(synthBWhiteNoise, 0, synthBNoiseMixer, 0);
AudioConnection          patchCord254(synthBPinkNoise, 0, synthBNoiseMixer, 1);

AudioConnection			 patchCord255(synthBWaveformMixer,0,synthBFullMixer,0);
AudioConnection			 patchCord256(synthBNoiseMixer,0,synthBFullMixer,1);

AudioConnection			 patchCord257(synthBWaveform3,0,synthBMultiply,0);
AudioConnection			 patchCord258(synthBNoiseMixer,0,synthBMultiply,1);

AudioConnection			 patchCord259(synthBFullMixer,0,synthBFilter,0);
AudioConnection			 patchCord260(synthBMultiply,0,synthBFilter,1);
AudioConnection			 patchCord261(synthBFilter,0,synthBEnvelope,0);
AudioConnection			 patchCord262(synthBEnvelope,0,mixer13,2);
AudioConnection			 patchCord263(synthBEnvelope,0,mixer16,2);
AudioConnection		     patchCord264(clickTrack,0,mixer13,3);
AudioConnection		     patchCord265(clickTrack,0,mixer16,3);


//********************************************************

//AudioConnection          patchCord123(mixer9, 0, mixer13, 0); //TEMP - routing direct to master mixer
//AudioConnection          patchCord124(mixer10, 0, mixer16, 0); //TEMP - routing direct to master mixer

AudioConnection          patchCord209(mixer13, 0, i2s1, 0);
AudioConnection          patchCord210(mixer16, 0, i2s1, 1);


// Create an object to control the audio shield.
AudioControlSGTL5000 audioShield;

//Compressor
boolean compressorOn = false;

//Mixer gain settings
float mixerGainL1 = 0.6; // RAW inputs default
float mixerGainL2 = 0.7; // mixed to single side channel
float mixerGainL3 = 0.7; // Delay master gain
float delayDropOff = 0.1; // default drop off per tap.
float mixerGainL4 = 0.7; // Left and right channel master mixer

//AudioMemory
const byte audioMemoryAssignment = 64; //Amount of RAM to assign the audio library 64 = 64*256 = 16384 = 16Kb = 50% of ram.

//******** Forward Declarations of all Functions ****************
void resetMixerGains(void);
void setupEnvelopes(void);
void setupAudio(void);
//***************************************************************

void resetMixerGains(void)
{
	// Level 1 gains - 16 RAW inputs, post envelopes and filters
	mixerPadsALeft.gain(0, mixerGainL1); //pad1 left
	mixerPadsALeft.gain(1, mixerGainL1); //pad2 left
	mixerPadsALeft.gain(2, mixerGainL1); //pad3 left
	mixerPadsALeft.gain(3, mixerGainL1); //pad4 left
	mixerPadsALeft.gain(4, mixerGainL1); //pad5 left
	mixerPadsALeft.gain(5, mixerGainL1); //pad6 left
	mixerPadsALeft.gain(6, mixerGainL1); //pad7 left
	mixerPadsALeft.gain(7, mixerGainL1); //pad8 left
	mixerPadsALeft.gain(8, mixerGainL1); //pad9 left
	mixerPadsALeft.gain(9, mixerGainL1); //pad10 left
	mixerPadsALeft.gain(10, mixerGainL1); //pad11 left
	mixerPadsALeft.gain(11, mixerGainL1); //pad12 left
	mixerPadsALeft.gain(12, mixerGainL1); //pad13 left
	mixerPadsALeft.gain(13, mixerGainL1); //pad14 left
	mixerPadsALeft.gain(14, mixerGainL1); //pad15 left
	mixerPadsALeft.gain(15, mixerGainL1); //pad16 left
	mixerPadsARight.gain(0, mixerGainL1); //pad1 right
	mixerPadsARight.gain(1, mixerGainL1); //pad2 right
	mixerPadsARight.gain(2, mixerGainL1); //pad3 right
	mixerPadsARight.gain(3, mixerGainL1); //pad4 right
	mixerPadsARight.gain(4, mixerGainL1); //pad5 right
	mixerPadsARight.gain(5, mixerGainL1); //pad6 right
	mixerPadsARight.gain(6, mixerGainL1); //pad7 right
	mixerPadsARight.gain(7, mixerGainL1); //pad8 right
	mixerPadsARight.gain(8, mixerGainL1); //pad9 right
	mixerPadsARight.gain(9, mixerGainL1); //pad10 right
	mixerPadsARight.gain(10, mixerGainL1); //pad11 right
	mixerPadsARight.gain(11, mixerGainL1); //pad12 right
	mixerPadsARight.gain(12, mixerGainL1); //pad13 right
	mixerPadsARight.gain(13, mixerGainL1); //pad14 right
	mixerPadsARight.gain(14, mixerGainL1); //pad15 right
	mixerPadsARight.gain(15, mixerGainL1); //pad16 right
	
		
	// Level 3 gains, delay mixers default settings.
	mixer11.gain(0, mixerGainL3); //left delay - tap1
	mixer11.gain(1, (mixerGainL3-(1 * delayDropOff))); //left-tap2
	mixer11.gain(2, (mixerGainL3-(2 * delayDropOff))); //left-tap3
	mixer11.gain(3, (mixerGainL3-(3 * delayDropOff))); //left-tap4
	mixer12.gain(0, (mixerGainL3-(4 * delayDropOff))); //left-tap5
	mixer12.gain(1, (mixerGainL3-(5 * delayDropOff))); //left-tap6
	mixer12.gain(2, (mixerGainL3-(6 * delayDropOff))); //left-tap7
	mixer12.gain(3, (mixerGainL3-(7 * delayDropOff))); //left-tap8
	
	mixer14.gain(0, mixerGainL3); //right delay - tap1
	mixer14.gain(1, (mixerGainL3-(1 * delayDropOff))); //right-tap2
	mixer14.gain(2, (mixerGainL3-(2 * delayDropOff))); //right-tap3
	mixer14.gain(3, (mixerGainL3-(3 * delayDropOff))); //right-tap4
	mixer15.gain(0, (mixerGainL3-(4 * delayDropOff))); //right-tap5
	mixer15.gain(1, (mixerGainL3-(5 * delayDropOff))); //right-tap6
	mixer15.gain(2, (mixerGainL3-(6 * delayDropOff))); //right-tap7
	mixer15.gain(3, (mixerGainL3-(7 * delayDropOff))); //right-tap8
	
	mixer13.gain(0, mixerGainL4); // Master Left Mixer - delay taps 1-4
	mixer13.gain(1, mixerGainL4); // Master Left Mixer - delay taps 5-8
	mixer13.gain(2, mixerGainL4); // Master Left Mixer - white noise
	mixer13.gain(3, mixerGainL4); // Master Left Mixer - pink noise
	mixer16.gain(0, mixerGainL4); // Master Right Mixer - delay taps 1-4
	mixer16.gain(1, mixerGainL4); // Master Right Mixer - delay taps 5-8
	mixer16.gain(2, mixerGainL4); // Master Right Mixer - white noise
	mixer16.gain(3, mixerGainL4); // Master Right Mixer - pink noise
}

void setupEnvelopes(void)
{
	// set envelope parameters
	for (int i=0; i<16; i++) {
		Envelopes[i]->attack(9.2); //9.2
		Envelopes[i]->hold(2.1); //2.1
		Envelopes[i]->decay(31.4); //31.4
		Envelopes[i]->sustain(0.6); //0.6
		Envelopes[i]->release(845.5); //84.5
	}
	
}


void setupAudio(void)
{
	AudioMemory(audioMemoryAssignment);
	audioShield.enable();
	audioShield.volume(0.7); //Master Volume
	resetMixerGains(); //setup all the mixer gain levels to defaults.
	setupEnvelopes(); // setup pad envelopes to default
}