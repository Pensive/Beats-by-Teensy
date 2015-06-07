
Encoder masterKnob(24, 8);
Bounce masterKnobButton = Bounce(25, 5);  // 5 ms debounce






void encoderReset() {
	masterKnob.write(0);
}

void setupInput() {
	pinMode(25, INPUT_PULLUP);
}



void loopInput() {
	//Analog
	Pot1 = analogRead(10) / 4;
	Pot2 = analogRead(11) / 4;
	Pot3 = analogRead(12) / 4;
	Pot4 = analogRead(13);
	audioShield.volume((float)Pot4 / 1023);
	
	//MasterKnob
	long newposMasterKnob;
	newposMasterKnob = masterKnob.read();
	//
	if (newposMasterKnob != posMasterKnob) {
		sequencerBPM += (newposMasterKnob-posMasterKnob);
		posMasterKnob = newposMasterKnob;
		updateBPM(sequencerBPM);
	}
	//
	if (masterKnobButton.update()) {
		if (masterKnobButton.fallingEdge()) {
			masterKnobPressed = true;
			masterKnobReleased = false;
		}
		else {
			masterKnobPressed = false;
			masterKnobReleased = true;
		}
	}
		
		
}