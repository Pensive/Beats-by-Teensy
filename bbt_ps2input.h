const int DataPin = 28;
const int IRQpin =  27;
PS2MiniPac MiniPac;
const byte Pad1_Char = 98;
const byte Pad2_Char = 108;
const byte Pad3_Char = 109;
const byte Pad4_Char = 99;
const byte Pad5_Char = 97;
const byte Pad6_Char = 106;
const byte Pad7_Char = 110;
const byte Pad8_Char = 52;
const byte Pad9_Char = 103;
const byte Pad10_Char = 111;
const byte Pad11_Char = 104;
const byte Pad12_Char = 51;
const byte Pad13_Char = 121;
const byte Pad14_Char = 105;
const byte Pad15_Char = 49;
const byte Pad16_Char = 100;
const byte Button_Quantize_Char = 117;
const byte Button_16Levels_Char = 101;
const byte Button_Undo_Char = 112;
const byte Button_Erase_Char = 115;
const byte Button_NoteRepeat_Char = 119;
const byte Button_Tempo_Char = 122;
const byte Button_Shift_Back_Char = 120;
const byte Button_Rec_Char = 107;
const byte Button_Overdub_Char = 53;
const byte Button_Stop_Char = 118;
const byte Button_Play_Char = 102;
const byte Button_PlayStart_Char = 116;
const byte Button_A_Char = 113;
const byte Button_B_Char = 114;
const byte Button_C_Char = 48;
const byte Button_D_Char = 50;





//******** Forward Declarations of all Functions ****************
void setupMiniPac(void);
void loopMiniPac(void);
//***************************************************************

void setupMiniPac(void)
{
	MiniPac.begin(DataPin, IRQpin);
	for (int i=0; i<32; i++) {
		keyPressed[i] = false;
	}
	keyPressedThisCycle = false;
}

void loopMiniPac(void)
{
	keyPressedThisCycle = false;
	while (MiniPac.available()) {
			keyPressedThisCycle = true;
		// read the next key
		char c = MiniPac.read();
		
		Serial.println(c); 
		// check for some of the special keys
		if (c == Pad1_Char) {
			if (MiniPac.keyup()) {releasePad(0); keyPressed[0]=false;} else {pressPad(0); keyPressed[0]=true;}
			} else if (c == Pad2_Char) {
			if (MiniPac.keyup()) {releasePad(1); keyPressed[1]=false;} else {pressPad(1); keyPressed[1]=true;}
			} else if (c == Pad3_Char) {
			if (MiniPac.keyup()) {releasePad(2); keyPressed[2]=false;} else {pressPad(2); keyPressed[2]=true;}
			} else if (c == Pad4_Char) {
			if (MiniPac.keyup()) {releasePad(3); keyPressed[3]=false;} else {pressPad(3); keyPressed[3]=true;}
			} else if (c == Pad5_Char) {
			if (MiniPac.keyup()) {releasePad(4); keyPressed[4]=false;} else {pressPad(4); keyPressed[4]=true;}
			} else if (c == Pad6_Char) {
			if (MiniPac.keyup()) {releasePad(5); keyPressed[5]=false;} else {pressPad(5); keyPressed[5]=true;}
			} else if (c == Pad7_Char) {
			if (MiniPac.keyup()) {releasePad(6); keyPressed[6]=false;} else {pressPad(6); keyPressed[6]=true;}
			} else if (c == Pad8_Char) {
			if (MiniPac.keyup()) {releasePad(7); keyPressed[7]=false;} else {pressPad(7); keyPressed[7]=true;}
			} else if (c == Pad9_Char) {
			if (MiniPac.keyup()) {releasePad(8); keyPressed[8]=false;} else {pressPad(8); keyPressed[8]=true;}
			} else if (c == Pad10_Char) {
			if (MiniPac.keyup()) {releasePad(9); keyPressed[9]=false;} else {pressPad(9); keyPressed[9]=true;}
			} else if (c == Pad11_Char) {
			if (MiniPac.keyup()) {releasePad(10); keyPressed[10]=false;} else {pressPad(10); keyPressed[10]=true;}
			} else if (c == Pad12_Char) {
			if (MiniPac.keyup()) {releasePad(11); keyPressed[11]=false;} else {pressPad(11); keyPressed[11]=true;}
			} else if (c == Pad13_Char) {
			if (MiniPac.keyup()) {releasePad(12); keyPressed[12]=false;} else {pressPad(12); keyPressed[12]=true;}
			} else if (c == Pad14_Char) {
			if (MiniPac.keyup()) {releasePad(13); keyPressed[13]=false;} else {pressPad(13); keyPressed[13]=true;}
			} else if (c == Pad15_Char) {
			if (MiniPac.keyup()) {releasePad(14); keyPressed[14]=false;} else {pressPad(14); keyPressed[14]=true;}
			} else if (c == Pad16_Char) {
			if (MiniPac.keyup()) {releasePad(15); keyPressed[15]=false;} else {pressPad(15); keyPressed[15]=true;}
			} else if (c == Button_Stop_Char) {
			//something
			} else if (c == Button_Play_Char) {
			if (MiniPac.keyup()) {} else {spiflash_transferfiles();}
			} else if (c == Button_PlayStart_Char) {
			//something
			} else if (c == Button_Rec_Char) {
			//something
			} else if (c == Button_Overdub_Char) {
			//something
			} else if (c == Button_Quantize_Char) {
			if (! MiniPac.keyup()) {fadeBacklightRGB(BacklightWhite);}
			} else if (c == Button_16Levels_Char) {
			if (! MiniPac.keyup()) {fadeBacklightRGB(BacklightRed);}
			} else if (c == Button_Undo_Char) {
			if (! MiniPac.keyup()) {fadeBacklightRGB(BacklightGreen);}
			} else if (c == Button_Erase_Char) {
			if (! MiniPac.keyup()) {fadeBacklightRGB(BacklightBlue);}
			} else if (c == Button_A_Char) {
			if (! MiniPac.keyup()) {fadeBacklightRGB(BacklightCyan); padMode=0;}
			} else if (c == Button_B_Char) {
			if (! MiniPac.keyup()) {fadeBacklightRGB(BacklightYellow); padMode=1;}
			} else if (c == Button_C_Char) {
			if (! MiniPac.keyup()) {fadeBacklightRGB(BacklightMagenta); padMode=2;}
			} else if (c == Button_D_Char) {
			if (! MiniPac.keyup()) {fadeBacklightRGB(BacklightBlue); padMode=3;}
			} else {
			// otherwise, just print all normal characters
			Serial.print(c); //nothing should pop up here unless there are PS/2 problems.
		}
		if (keyPressedThisCycle) { loopLCD(); //this is now on  30fps timer.
		}
	}
}

