// Demonstrate a resonant state-variable filter whose filter frequency
// is controlled by an LFO and envelope. A second encelope is used
// to control the dynamics of each note. This example is a simple,
// monophonic synth.
//
// Accepts MIDI in over USB. Set USB type to MIDI in the Tools menu.
//
// This example code is in the public domain.

byte synthAPadNotes[16];
byte synthBPadNotes[16];


void setupSynth(void)
{
	//************* Synth A ***********
	//set up a basic subtractive synth patch
	// modulation
	synthASine1.frequency(5);        // 5Hz LFO
	synthASine1.amplitude(0.7);     // controls modulation depth
	synthASine2.frequency(23);        // 23Hz second LFO
	synthASine1.amplitude(0.9);     // controls modulation depth
	synthAEnvelope1.attack(140);  // fairly slow attack and decay
	synthAEnvelope1.hold(180);
	synthAEnvelope1.sustain(0.3);
	synthAEnvelope1.decay(140);
	synthAEnvelope1.release(40);
	// saw oscillator
	synthAWaveform.begin(0.2, 220, WAVEFORM_SAWTOOTH);  // 220Hz saw wave oscillator
	// quiet level of 0.4 as resonant filter adds gain
	// resonant filter, low-pass mode (output 0 of filter is LP)
	synthAFilter.resonance(18);
	synthAFilter.octaveControl(1.5);  // modulation signal shifts resonant frequency by +/- 1.5 octaves
	synthAFilter.frequency(200);    // start below the resonant peak
	//envelope for note -on and -off dynamics
	synthAEnvelope2.attack(10);
	synthAEnvelope2.decay(20);
	synthAEnvelope2.release(80);
	
	synthAPadNotes[0] = 60;
	synthAPadNotes[1] = 62;
	synthAPadNotes[2] = 63;
	synthAPadNotes[3] = 64;
	synthAPadNotes[4] = 67;
	synthAPadNotes[5] = 69;
	synthAPadNotes[6] = 72;
	synthAPadNotes[7] = 74;
	synthAPadNotes[8] = 75;
	synthAPadNotes[9] = 76;
	synthAPadNotes[10] = 79;
	synthAPadNotes[11] = 81;
	synthAPadNotes[12] = 84;
	synthAPadNotes[13] = 86;
	synthAPadNotes[14] = 87;
	synthAPadNotes[15] = 88;
	
	//************* Synth B - TeensyMoog ***********
	synthBPadNotes[0] = 60;
	synthBPadNotes[1] = 62;
	synthBPadNotes[2] = 63;
	synthBPadNotes[3] = 64;
	synthBPadNotes[4] = 67;
	synthBPadNotes[5] = 69;
	synthBPadNotes[6] = 72;
	synthBPadNotes[7] = 74;
	synthBPadNotes[8] = 75;
	synthBPadNotes[9] = 76;
	synthBPadNotes[10] = 79;
	synthBPadNotes[11] = 81;
	synthBPadNotes[12] = 84;
	synthBPadNotes[13] = 86;
	synthBPadNotes[14] = 87;
	synthBPadNotes[15] = 88;
	
	// now respond to MIDI over USB. In this example, only note-on and note-off used and velocity ignored
	//usbMIDI.setHandleNoteOff(OnNoteOff);
	//usbMIDI.setHandleNoteOn(OnNoteOn);
}


void synthANoteOn(byte channel, byte note, byte velocity) {
	// accept input on any channel, and ignore velocity
	
	// check for a currently held note and brutally kill it
	// this sounds bad so only play one note at once, retro style
	if (synthACurrentNote <= 127) {
		synthAEnvelope2.release(2);
		synthAEnvelope2.noteOff();
		delay(3);  // briefest period for killed note to stop sounding
		synthAEnvelope2.release(80);
	}
	synthACurrentNote = note;
	// MIDI note 69 == A4 == 440Hz
	float freq = ((float)note - 69) / 12.0;  // semitones above or below A4
	freq = 440.0 * powf(2.0, freq);
	synthASine1.phase(0);          // reset LFOs phases
	synthASine2.phase(0);
	synthAEnvelope1.noteOn();  // start modulation
	synthAWaveform.frequency(freq);
	synthAWaveform.phase(0);
	synthAFilter.frequency(1.2 * freq);  // resonance above played note
	synthAEnvelope2.noteOn();
}

void synthANoteOff(byte channel, byte note, byte velocity) {
	// accept any channel, ignore off-velocity as most things do
	// we only care about the currently playing note
	if (note == synthACurrentNote) {
		synthAEnvelope2.noteOff();
		synthAEnvelope1.noteOff();
		synthACurrentNote = 255;
	}
}

void loopSynthA()
{
	synthASine1.frequency(Pot1);        // 5Hz LFO
	//synthASine1.amplitude(float(Pot2/255));     // controls modulation depth
	synthASine2.frequency(Pot2);        // 23Hz second LFO
	//synthASine1.amplitude(float(Pot2/255));	
	synthAFilter.frequency(Pot3);
	synthAFilter.resonance(Pot4);
}