/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Modified to play from Serial Flash (c) Frank Bösing, 2014/12

#include "play_serialflash.h"
#include <arm_math.h>

extern "C" {
extern const int16_t ulaw_decode_table[256];
};


/*	========================================================================================
				SPIFIFO.h
	======================================================================================*/
#if SERIALFLASH_USE_SPIFIFO

#define SPICLOCK_MAX ((SPI_CTAR_PBR(0) | SPI_CTAR_BR(0) | SPI_CTAR_DBR)) // F_BUS/2 MHz


void AudioPlaySerialFlash::flashinit(void)
{
#if AUDIOBOARD == 1
	pinMode(10,OUTPUT);
	digitalWrite(10, HIGH);
	SPI.setMOSI(7);
	SPI.setMISO(12);
	SPI.setSCK(14);
#else
	SPI.setMOSI(7);
	SPI.setMISO(12);
	SPI.setSCK(13);
#endif
	SPIFIFO.begin(SERFLASH_CS, SPICLOCK_MAX);
}

inline void AudioPlaySerialFlash::readSerStart(const size_t position)
{

#if 0
	delay(1);
	SPIFIFO.write(0x9F, SPI_CONTINUE);
	SPIFIFO.write(0, SPI_CONTINUE);
	SPIFIFO.write(0, SPI_CONTINUE);
	SPIFIFO.write(0);
	SPIFIFO.read();
	Serial.printf("manufacturerID = %x\r\n",SPIFIFO.read());
	Serial.printf("memoryType = %x\r\n",SPIFIFO.read());
	Serial.printf("capacity = %x\r\n",SPIFIFO.read());
#endif
	SPIFIFO.write16((0xb00 | ((position>>16) & 0xff)), SPI_CONTINUE);//CMD_READ_HIGH_SPEED
	SPIFIFO.write16( position, SPI_CONTINUE);
	SPIFIFO.write(0,SPI_CONTINUE);
	SPIFIFO.read();
	SPIFIFO.read();
	SPIFIFO.read();
}

void AudioPlaySerialFlash::play(const unsigned int data)
{
	int temp;
	stop();
	AudioStartUsingSPI();
	__disable_irq();
	readSerStart(data);
	SPIFIFO.write16(0x00,SPI_CONTINUE);
	SPIFIFO.write16(0x00);
	prior = 0;
	next = 0;
	beginning = data + 4;
	length =__REV16(SPIFIFO.read());
	temp = SPIFIFO.read();
	length |= (temp & 0xff00) << 8;
	playing = temp & 0xff;
	__enable_irq();
	//Serial.printf("Length:0x%x Mode: 0x%x\r\n", length, playing);
}

void AudioPlaySerialFlash::update(void)
{
	audio_block_t *block;
	int16_t *out;
	uint32_t consumed;
	int16_t s0, s1, s2, s3, s4;
	uint16_t a;
	int i;
	uint8_t p;
		
	if (paused) return;
	p = playing;
	if (!p) return;

	block = allocate();
	if (block == NULL) return;

//	cyc =  ARM_DWT_CYCCNT;
	readSerStart(beginning + next);

	//FIFO 8 Bytes ahead..

	SPIFIFO.write16(0,SPI_CONTINUE);
	SPIFIFO.write16(0,SPI_CONTINUE);
	SPIFIFO.write16(0,SPI_CONTINUE);
	SPIFIFO.write16(0,SPI_CONTINUE);

	out = block->data;
	s0 = prior;

if (p<=0x03)	
	switch (p) {

	  case 0x01: // u-law encoded, 44100 Hz		//3667 cycles 6925
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i+=2) {
			a = SPIFIFO.read();
			*out++ = ulaw_decode_table[a>>8];
			*out++ = ulaw_decode_table[a & 0xff];
			if (i < AUDIO_BLOCK_SAMPLES - 10) {SPIFIFO.write16(0,SPI_CONTINUE);}
			else
			if (i < AUDIO_BLOCK_SAMPLES - 8) {SPIFIFO.write16(0);}
		}
		consumed = 128;
		break;

	  case 0x02: // u-law encoded, 22050 Hz 3730
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i += 8) {
			a = SPIFIFO.read();
			s1 = ulaw_decode_table[a>>8];
			s2 = ulaw_decode_table[a & 0xff];
			*out++ = (s0 + s1) >> 1;
			*out++ = s1;
			*out++ = (s1 + s2) >> 1;
			*out++ = s2;
			a = SPIFIFO.read();
			s3 = ulaw_decode_table[a>>8];
			s4 = ulaw_decode_table[a & 0xff];
			*out++ = (s2 + s3) >> 1;
			*out++ = s3;
			*out++ = (s3 + s4) >> 1;
			*out++ = s4;
			s0 = s4;
			if (i < AUDIO_BLOCK_SAMPLES - 8*2) {SPIFIFO.write16(0,SPI_CONTINUE);}
			if (i < AUDIO_BLOCK_SAMPLES - 8*2-8) {SPIFIFO.write16(0,SPI_CONTINUE);}
			else
			if (i < AUDIO_BLOCK_SAMPLES - 8*2) {SPIFIFO.write16(0);}
		}
		consumed = 64;
		break;

		case 0x03: // u-law encoded, 11025 Hz 2674
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i += 16) {
			a = SPIFIFO.read();
			s1 = ulaw_decode_table[a>>8];
			s2 = ulaw_decode_table[a & 0xff];
			*out++ = (s0 * 3 + s1) >> 2;
			*out++ = (s0 + s1)     >> 1;
			*out++ = (s0 + s1 * 3) >> 2;
			*out++ = s1;
			*out++ = (s1 * 3 + s2) >> 2;
			*out++ = (s1 + s2)     >> 1;
			*out++ = (s1 + s2 * 3) >> 2;
			*out++ = s2;
			a = SPIFIFO.read();
			s3 = ulaw_decode_table[a>>8];
			s4 = ulaw_decode_table[a & 0xff];
			if (i < AUDIO_BLOCK_SAMPLES - 16*2) {SPIFIFO.write16(0,SPI_CONTINUE);}
			if (i < AUDIO_BLOCK_SAMPLES - 16*2-16) {SPIFIFO.write16(0,SPI_CONTINUE);}
			else
			if (i < AUDIO_BLOCK_SAMPLES - 16*2) {SPIFIFO.write16(0);}
			*out++ = (s2 * 3 + s3) >> 2;
			*out++ = (s2 + s3)     >> 1;
			*out++ = (s2 + s3 * 3) >> 2;
			*out++ = s3;
			*out++ = (s3 * 3 + s4) >> 2;
			*out++ = (s3 + s4)     >> 1;
			*out++ = (s3 + s4 * 3) >> 2;
			*out++ = s4;
			s0 = s4;
		}
		consumed = 32;
		break;
	}
else 
	switch(p) {

		case 0x81: // 16 bit PCM, 44100 Hz	//13310 21608
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
			a = __REV16(SPIFIFO.read());
			*out++ = a;
			if (i < AUDIO_BLOCK_SAMPLES - 5) {SPIFIFO.write16(0,SPI_CONTINUE);}
			else
			if (i < AUDIO_BLOCK_SAMPLES - 4) {SPIFIFO.write16(0);} 
		}
		consumed = 256;
		break;

		case 0x82: // 16 bits PCM, 22050 Hz		6930
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i += 4) {
			s1 = __REV16(SPIFIFO.read());
			s2 = __REV16(SPIFIFO.read());
			*out++ = (s0 + s1) >> 1;
			*out++ = s1;
			*out++ = (s1 + s2) >> 1;
			*out++ = s2;
			s0 = s2;
			if (i < AUDIO_BLOCK_SAMPLES - 4*2) {SPIFIFO.write16(0,SPI_CONTINUE);}
			if (i < AUDIO_BLOCK_SAMPLES - 4*2-4) {SPIFIFO.write16(0,SPI_CONTINUE);}
			else
			if (i < AUDIO_BLOCK_SAMPLES - 4*2) {SPIFIFO.write16(0);}
		}
		consumed = 128;
		break;

		case 0x83: // 16 bit PCM, 11025 Hz	3740
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i += 8) {
			s1 = __REV16(SPIFIFO.read());
			s2 = __REV16(SPIFIFO.read());
			*out++ = (s0 * 3 + s1) >> 2;
			*out++ = (s0 + s1)     >> 1;
			*out++ = (s0 + s1 * 3) >> 2;
			*out++ = s1;
			*out++ = (s1 * 3 + s2) >> 2;
			*out++ = (s1 + s2)     >> 1;
			*out++ = (s1 + s2 * 3) >> 2;
			*out++ = s2;
			s0 = s2;
			if (i < AUDIO_BLOCK_SAMPLES - 8*2) {SPIFIFO.write16(0,SPI_CONTINUE);}
			if (i < AUDIO_BLOCK_SAMPLES - 8*2-8) {SPIFIFO.write16(0,SPI_CONTINUE);}
			else
			if (i < AUDIO_BLOCK_SAMPLES - 8*2) {SPIFIFO.write16(0);}
		}
		consumed = 64;
		break;

	  default:
		release(block);
		stop();
		return;
	}

	transmit(block);
	release(block);

	prior = s0;
	next += consumed;

//	cyc = ARM_DWT_CYCCNT - cyc;

	if (next >= length) stop();

}
/*	========================================================================================
				SPI.h
	======================================================================================*/
#else
#define SPICLOCK_MAX 30000000

void AudioPlaySerialFlash::flashinit(void)
{
#if AUDIOBOARD == 1
	pinMode(10,OUTPUT);
	digitalWrite(10, HIGH);
	pinMode(SERFLASH_CS,OUTPUT);
	digitalWrite(SERFLASH_CS, HIGH);
	SPI.setMOSI(7);
	SPI.setMISO(12);
	SPI.setSCK(14);
#else
	pinMode(SERFLASH_CS,OUTPUT);
	digitalWrite(SERFLASH_CS, HIGH);
	SPI.setMOSI(11);
	SPI.setMISO(12);
	SPI.setSCK(13);
#endif
	SPI.begin();
	spisettings = SPISettings(SPICLOCK_MAX , MSBFIRST, SPI_MODE0);
}

inline void AudioPlaySerialFlash::readSerStart(const size_t position)
{
	SPI.beginTransaction(spisettings);
	digitalWriteFast(SERFLASH_CS, LOW);
	SPI.transfer(0x0b);//CMD_READ_HIGH_SPEED
	SPI.transfer((position >> 16) & 0xff);
	SPI.transfer((position >> 8) & 0xff);
	SPI.transfer(position & 0xff);
	SPI.transfer(0);
}

inline void AudioPlaySerialFlash::readSerDone(void)
{
	digitalWriteFast(SERFLASH_CS, HIGH);
	SPI.endTransaction();
}

void AudioPlaySerialFlash::play(const unsigned int data)
{
	int temp;
	stop();
	AudioStartUsingSPI();
	readSerStart(data);
	length = SPI.transfer(0);
	length |= (uint16_t) SPI.transfer(0) <<8;
	length |= (uint32_t) SPI.transfer(0) <<16;
	temp = SPI.transfer(0);
	readSerDone();
	prior = 0;
	next = 0;
	beginning = data + 4;
	__disable_irq();
	playing = temp;
	__enable_irq();
}
#endif

void AudioPlaySerialFlash::stop(void)
{
	__disable_irq();
	paused = false;
	if (playing) {
		playing = 0;
		//Serial.printf("Next:%d",next);
		__enable_irq();		
		AudioStopUsingSPI();
	} else
	__enable_irq();
}

#if !SERIALFLASH_USE_SPIFIFO
void AudioPlaySerialFlash::update(void)
{
	audio_block_t *block;
	int16_t *out;
	uint32_t consumed;
	int16_t s0, s1, s2, s3, s4;
	uint16_t a,b;
	int i;

	if (paused) return;
	if (!playing) return;
	block = allocate();
	if (block == NULL) return;

//	__disable_irq();
//	cyc =  ARM_DWT_CYCCNT;

	out = block->data;
	s0 = prior;

	readSerStart(beginning + next);

	switch (playing) {

	  case 0x01: // u-law encoded, 44100 Hz
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
			*out++ = ulaw_decode_table[SPI.transfer(0)];
		}
		consumed = 128;
		break;

	  case 0x02: // u-law encoded, 22050 Hz
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i += 8) {
			s1 = ulaw_decode_table[SPI.transfer(0)];
			s2 = ulaw_decode_table[SPI.transfer(0)];
			s3 = ulaw_decode_table[SPI.transfer(0)];
			s4 = ulaw_decode_table[SPI.transfer(0)];
			*out++ = (s0 + s1) >> 1;
			*out++ = s1;
			*out++ = (s1 + s2) >> 1;
			*out++ = s2;
			*out++ = (s2 + s3) >> 1;
			*out++ = s3;
			*out++ = (s3 + s4) >> 1;
			*out++ = s4;
			s0 = s4;
		}
		consumed = 64;
		break;

		case 0x03: // u-law encoded, 11025 Hz
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i += 16) {
			s1 = ulaw_decode_table[SPI.transfer(0)];
			s2 = ulaw_decode_table[SPI.transfer(0)];
			s3 = ulaw_decode_table[SPI.transfer(0)];
			s4 = ulaw_decode_table[SPI.transfer(0)];
			*out++ = (s0 * 3 + s1) >> 2;
			*out++ = (s0 + s1)     >> 1;
			*out++ = (s0 + s1 * 3) >> 2;
			*out++ = s1;
			*out++ = (s1 * 3 + s2) >> 2;
			*out++ = (s1 + s2)     >> 1;
			*out++ = (s1 + s2 * 3) >> 2;
			*out++ = s2;
			*out++ = (s2 * 3 + s3) >> 2;
			*out++ = (s2 + s3)     >> 1;
			*out++ = (s2 + s3 * 3) >> 2;
			*out++ = s3;
			*out++ = (s3 * 3 + s4) >> 2;
			*out++ = (s3 + s4)     >> 1;
			*out++ = (s3 + s4 * 3) >> 2;
			*out++ = s4;
			s0 = s4;
		}
		consumed = 32;
		break;

		case 0x81: // 16 bit PCM, 44100 Hz
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
			a = SPI.transfer(0);
			b = SPI.transfer(0);
			*out++ = a | b<<8;
		}
		consumed = 256;
		break;

		case 0x82: // 16 bits PCM, 22050 Hz
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i += 4) {
			a = SPI.transfer(0);
			b = SPI.transfer(0);
			s1 = a | b<<8;
			a = SPI.transfer(0);
			b = SPI.transfer(0);
			s2 = a | b<<8;
			*out++ = (s0 + s1) >> 1;
			*out++ = s1;
			*out++ = (s1 + s2) >> 1;
			*out++ = s2;
			s0 = s2;
		}
		consumed = 128;
		break;

		case 0x83: // 16 bit PCM, 11025 Hz
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i += 8) {
			a = SPI.transfer(0);
			b = SPI.transfer(0);
			s1 = a | b<<8;
			a = SPI.transfer(0);
			b = SPI.transfer(0);
			s2 = a | b<<8;
			*out++ = (s0 * 3 + s1) >> 2;
			*out++ = (s0 + s1)     >> 1;
			*out++ = (s0 + s1 * 3) >> 2;
			*out++ = s1;
			*out++ = (s1 * 3 + s2) >> 2;
			*out++ = (s1 + s2)     >> 1;
			*out++ = (s1 + s2 * 3) >> 2;
			*out++ = s2;
			s0 = s2;
		}
		consumed = 64;
		break;

	  default:
		release(block);
		stop();
		return;
	}

	readSerDone();

	transmit(block);
	release(block);

	prior = s0;
	next += consumed;

//	cyc = ARM_DWT_CYCCNT - cyc;
//	__enable_irq();

	if (next >= length) stop();

}
#endif

#define B2M_88200 (uint32_t)((double)4294967296000.0 / AUDIO_SAMPLE_RATE_EXACT * 1.0)
#define B2M_44100 (uint32_t)((double)4294967296000.0 / AUDIO_SAMPLE_RATE_EXACT * 2.0)
#define B2M_22050 (uint32_t)((double)4294967296000.0 / AUDIO_SAMPLE_RATE_EXACT * 4.0)
#define B2M_11025 (uint32_t)((double)4294967296000.0 / AUDIO_SAMPLE_RATE_EXACT * 8.0)


int AudioPlaySerialFlash::BytesConsumedPerUpdate(void)
{
	__disable_irq();
	int p = playing;
	__enable_irq();
	int n = 32 << (3-(p & 0x03));
	if (p & 0x80) n*=2;	
	return n;
}

uint32_t AudioPlaySerialFlash::b2m(void)
{
  uint32_t b;
  __disable_irq();
  uint8_t p= playing;
  __enable_irq();  
  switch (p) {
	  case 0x81: // 16 bit PCM, 44100 Hz
		b = B2M_88200;  break;
	  case 0x01: // u-law encoded, 44100 Hz
	  case 0x82: // 16 bits PCM, 22050 Hz
		b = B2M_44100;  break;
	  case 0x02: // u-law encoded, 22050 Hz
	  case 0x83: // 16 bit PCM, 11025 Hz
		b = B2M_22050;  break;
	  case 0x03: // u-law encoded, 11025 Hz
		b = B2M_11025;  break;
	  default:
	    b = 0; break;
	}
	return b;
}

uint32_t AudioPlaySerialFlash::calcMillis(uint32_t position)
{
	return ((uint64_t)position * b2m()) >> 32;
}

//returns millisconds since start
uint32_t AudioPlaySerialFlash::positionMillis(void)
{
	__disable_irq();
	uint32_t n = next;
	__enable_irq();
	return calcMillis(n - beginning);
}

//returns length of file in milliseconds
uint32_t AudioPlaySerialFlash::lengthMillis(void)
{
	return calcMillis(length);
}

//returns true when playing
bool AudioPlaySerialFlash::isPlaying(void)
{
	return playing > 0;
}

//pauses playing
bool AudioPlaySerialFlash::pause(bool _paused)
{
	__disable_irq();
	bool p = (playing > 0)?_paused:false;
	paused = p;
	__enable_irq();
	return p;
}

//set position in milliseconds in 2.9ms steps.
//
void AudioPlaySerialFlash::setPositionMillis(const unsigned int millis)
{
	uint32_t n = ((uint64_t)millis<<32) / b2m();
	int u = BytesConsumedPerUpdate();
	n = n & ~(u-1);
	if (n >= length -  u ) stop();
	else {
		__disable_irq();
		next = n;
		__enable_irq();
	}
	Serial.printf("pu:%d",n);
}