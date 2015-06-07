#define FLASHSIZE 16777216
#define PAGE      256
#define DIRECTORY "MADZACH1" //this is not necessary anymore
File dir;
File entry;
unsigned char id_tab[32];
unsigned pos;
unsigned page;
unsigned fsize = 0;
unsigned fcnt = 0;
unsigned char buf[PAGE];
unsigned char buf2[PAGE];

String PadFilenames[16];
char PadFilenameChar[64];
unsigned int PadFlashOffset[16];
//Current PAtches
String currentPadsetA = "MADZACH1";

//******** Forward Declarations of all Functions ****************
void initialiseSDCard(void);
void setupFileAccess(void);
void updateAllFilenames(void);
bool spiflash_verify(void);
void spiflash_flash(void);
void spiflash_erase(void);
void spiflash_transferfiles(void);

//***************************************************************

void initialiseSDCard(void)
{
	//SDCard Initialise
	SPI.setMOSI(7);
	SPI.setSCK(14);
	if (!(SD.begin(10))) {
		// stop here, but print a message repetitively
		while (1) {
			//Serial.println("Unable to access the SD card");
			delay(500);
		}
	} else
	{
		//Serial.println("SD card initialised");
	}
}


void setupFileAccess(void)
{
	initialiseSDCard(); //setup SDcard access
	updateAllFilenames();
}

void updateAllFilenames(void)
{
	PadFilenames[0] = "/" + currentPadsetA + "/P1.RAW";
	PadFilenames[1] = "/" + currentPadsetA + "/P2.RAW";
	PadFilenames[2] = "/" + currentPadsetA + "/P3.RAW";
	PadFilenames[3] = "/" + currentPadsetA + "/P4.RAW";
	PadFilenames[4] = "/" + currentPadsetA + "/P5.RAW";
	PadFilenames[5] = "/" + currentPadsetA + "/P6.RAW";
	PadFilenames[6] = "/" + currentPadsetA + "/P7.RAW";
	PadFilenames[7] = "/" + currentPadsetA + "/P8.RAW";
	PadFilenames[8] = "/" + currentPadsetA + "/P9.RAW";
	PadFilenames[9] = "/" + currentPadsetA + "/P10.RAW";
	PadFilenames[10] = "/" + currentPadsetA + "/P11.RAW";
	PadFilenames[11] = "/" + currentPadsetA + "/P12.RAW";
	PadFilenames[12] = "/" + currentPadsetA + "/P13.RAW";
	PadFilenames[13] = "/" + currentPadsetA + "/P14.RAW";
	PadFilenames[14] = "/" + currentPadsetA + "/P15.RAW";
	PadFilenames[15] = "/" + currentPadsetA + "/P16.RAW";
	
	PadFlashOffset[0] = 0x0;
	PadFlashOffset[1] = 0x6700;
	PadFlashOffset[2] = 0x6B800;
	PadFlashOffset[3] = 0x78400;
	PadFlashOffset[4] = 0xB7B00;
	PadFlashOffset[5] = 0xF4100;
	PadFlashOffset[6] = 0xFB000;
	PadFlashOffset[7] = 0x107C00;
	PadFlashOffset[8] = 0x12DD00;
	PadFlashOffset[9] = 0x18CE00;
	PadFlashOffset[10] = 0x244A00;
	PadFlashOffset[11] = 0x287300;
	PadFlashOffset[12] = 0x293F00;
	PadFlashOffset[13] = 0x2D3600;
	PadFlashOffset[14] = 0x2F9700;
	PadFlashOffset[15] = 0x384E00;
	
}


/*
   W25Q128FV Serial Flasher
   
   (c) Frank Boesing, 2014,Dec
   GNU License Version 3.
   Teensy Audio Shield (c) Paul Stoffregen 
   W25Q128FV - Library  (c) Pete (El Supremo) 
   Thank you both!!!!
      
   Reads directory "/SERFLASH" on SD and burns 
   all files to the optional serial flash.
  
   Version 20141227
      
*/


bool spiflash_verify(void)
{
    fcnt = 1;
    pos = 0; 
    page = 0;     
    Serial.println("Verify.");
    dir = SD.open(DIRECTORY);
    while(1) {
      entry = dir.openNextFile();
      if (!entry) break;
      pos = page * PAGE;
      Serial.printf("%d. Verifying \"%s\" at Position: 0x%X...", fcnt, entry.name(), pos);
      int rd =0;
      do {
        memset(buf, 0xff, PAGE);
        rd = entry.read(buf, PAGE);
        flash_read_pages(buf2, page, 1);
        int v = 0;
        for (int i=0; i<PAGE; i++) v+= buf[i]-buf2[i];
        if (v) {Serial.println("is not ok."); return false;}
        pos += rd;         
        page++;
      } while (rd==PAGE);          
      Serial.printf("ok.\r\n");
      entry.close(); 
      fcnt++;
    }
    return true;
}

void spiflash_flash(void)
{
    fcnt = 1;
    pos = 0; 
    page = 0;     
    dir = SD.open(DIRECTORY);
    while(1) {
      entry = dir.openNextFile();
      if (!entry) break;
      pos = page * PAGE;
      Serial.printf("%d. Flashing \"%s\" at Position: 0x%X...", fcnt, entry.name(), pos);
	  PadFlashOffset[fcnt]=pos;
	  int rd =0;
      do {
        memset(buf, 0xff, PAGE);          
        rd = entry.read(buf, PAGE);
        pos += rd;         
        flash_page_program(buf,page);
        page++;
      } while (rd==PAGE);          
      Serial.printf("done.\r\n");
      entry.close(); 
      fcnt++;
    }
}

void spiflash_erase(void) {
    Serial.println("Erasing chip....");
    flash_chip_erase(true);
    Serial.println("done.");
}

void spiflash_transferfiles(void) {
	dir = SD.open(DIRECTORY);
	fsize = 0;
	fcnt = 0;
	while(1)
	{
		entry = dir.openNextFile();
		if (!entry) break;
		int s = entry.size();
		if ( (s & 0xff) > 0) s |= 0xff;
		Serial.printf("%s\t\t\t%d Bytes\r\n", entry.name(), s);
		fsize += s;
		entry.close();
		fcnt++;
	}
	dir.close();
	
	Serial.printf("\t\t\t%d File(s), %d Bytes\r\n", fcnt, fsize);

	if (!fsize) goto ready;
	if (fsize < FLASHSIZE) {
		flash_init();
		int flashstatus = flash_read_status();
		flash_read_id(id_tab);
		Serial.printf("Flash Status: 0x%X, ID:0x%X,0x%X,0x%X,0x%X ", flashstatus , id_tab[0], id_tab[1], id_tab[2], id_tab[3]);
		if (id_tab[0]!=0xef || id_tab[1]!=0x40 || id_tab[2]!=0x18 || id_tab[3]!=0x00) {Serial.println(" is not ok."); goto end;}
		Serial.printf(" is ok.\r\nFile(s) fit(s) in serial flash, %d Bytes remaining.\r\n\r\n", FLASHSIZE - fsize);

		Serial.print("Check flash content: ");
		if (spiflash_verify()) { Serial.println("Nothing to do.");goto end; }

		spiflash_erase();
		spiflash_flash();
		//spiflash_verify();

		end:
		dir.close();
	}
	else Serial.println("Does not fit.");
	
	ready:
	Serial.println("Ready.");
}