/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Pete (El Supremo), el_supremo@shaw.ca
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
// Most recent version 140403 1600
/*
  This code implements the basic instructions of the
  W25Q128FV SPI flash chip which can be added to the
  Teensy 3.x Audio adapter board.
  The chip is divided into 65536 pages of 256 bytes each.
  When writing to the chip you must write a page at a time
  and that page must have previously been erased to all 1
  When erasing, pages  can be erased only in groups of
  16 (4kB sector), 128 (32kB block), 256 (64kB block) or
  the entire chip (chip erase).
  
*/
#include <SPI.h>
#include "flash_spi.h"

static const uint8_t  f_cs=6; //chip select 

#define STAT_WIP 1
#define STAT_WEL 2

#define CMD_WRITE_STATUS_REG   0x01
#define CMD_PAGE_PROGRAM       0x02
#define CMD_READ_DATA          0x03
#define CMD_WRITE_DISABLE      0x04
#define CMD_READ_STATUS_REG    0x05
#define CMD_WRITE_ENABLE       0x06
#define CMD_READ_HIGH_SPEED    0x0B
#define CMD_SECTOR_ERASE       0x20
#define CMD_BLOCK32K_ERASE     0x52
#define CMD_RESET_ENABLE       0x66
#define CMD_RESET_DEVICE       0x99
#define CMD_READ_ID            0x9F
#define CMD_RELEASE_POWER_DOWN 0xAB
#define CMD_POWER_DOWN         0xB9
#define CMD_CHIP_ERASE         0xC7
#define CMD_BLOCK64K_ERASE     0xD8


//=====================================
// flash_wait_for_write is set (non_zero) after any write
// and all functions use write_pause to check that Write-In-Progress
// is clear before proceeding.
// The EXCEPTIONS are:
//    - flash_init
//    - the flash_read_status function (which would cause a recursive call)
// Note that the hard reset function does do a write_pause() first, but it
//   does its own wait when the reset command has been issued
unsigned char flash_wait_for_write = 0;

void write_pause(void)
{
  if(flash_wait_for_write) {
  	while(flash_read_status() & STAT_WIP);
  	flash_wait_for_write = 0;
  }
}

//=====================================
void flash_init(void)
{ 
  flash_wait_for_write = 0;
  // Set up pin 10 even though we don't use it as CS
  // This has fixed up most of the problems with SPI
  // when PlayFromSdCard is run before playing from flash.
  digitalWrite(10, HIGH);
  pinMode(10,OUTPUT);

  // Set up the chip select pin (6)
  digitalWrite(f_cs, HIGH);
  pinMode(f_cs,OUTPUT);

  // Assign MOSI and SCK for the audio board
  // BEFORE doing SPI.begin()
  SPI.setMOSI(7);
  SPI.setSCK(14);
    
  // start the SPI library:
  // This sets pin 10 HIGH and then to output
  SPI.begin();

  SPI.setBitOrder(MSBFIRST);
  // MODE0 and MODE3 work with the W25Q128FV (and M25P16)  flash chip
  // I've used MODE0 because the SD library also uses it.
  SPI.setDataMode(SPI_MODE0);
  // Some of it fails with SPI_CLOCK_DIV2
  //>>>PAH use DIV2 all the time - replace DIV4
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  // This is a horrible kludge but reading the status byte
  // here SOMEHOW fixes the problem of flash locking up
  // after the uSD card has been accessed
  flash_read_status();

//  delay(10);
}

//=====================================
// convert a page number to a 24-bit address
int page_to_address(int pn)
{
  return(pn << 8);
}

//=====================================
// convert a 24-bit address to a page number
int address_to_page(int addr)
{
  return(addr >> 8);
}

//=====================================
void flash_read_id(unsigned char *idt)
{
  write_pause();
  //set control register 
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_READ_ID);
  for(int i = 0;i < 20;i++) {
    *idt++ = SPI.transfer(0x00);
  }
  digitalWrite(f_cs, HIGH);
}

//=====================================
unsigned char flash_read_status(void)
{
  unsigned char c;

// This can't do a write_pause
  digitalWrite(f_cs, LOW);  
  SPI.transfer(CMD_READ_STATUS_REG);
  c = SPI.transfer(0x00);
  digitalWrite(f_cs, HIGH);
  return(c);
}

//=====================================

void flash_hard_reset(void)
{

  // Make sure that the device is not busy before
  // doing the hard reset sequence
  // At the moment this does NOT check the
  // SUSpend status bit in Status Register 2
  // but the library does not support suspend
  // mode yet anyway
  write_pause();
  
  // Send Write Enable command
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_RESET_ENABLE);
  digitalWrite(f_cs, HIGH);
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_RESET_DEVICE);
  digitalWrite(f_cs, HIGH);
  // Wait for the hard reset to finish
  // Don't use flash_wait_for_write here
  while(flash_read_status() & STAT_WIP);
  // The spec says "the device will take
  // approximately tRST=30 microseconds
  // to reset"
  // Give it a bit more time to be sure
  delayMicroseconds(50);
}

//=====================================
// Tbe Typ=13sec  Max=40sec
void flash_chip_erase(boolean wait)
{
  write_pause();
  // Send Write Enable command
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_WRITE_ENABLE);
  digitalWrite(f_cs, HIGH);
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_CHIP_ERASE);
  digitalWrite(f_cs, HIGH);
  flash_wait_for_write = 1;
  if(wait)write_pause();
}

//=====================================
// Tse Typ=0.6sec Max=3sec
// measured 549.024ms
// Erase the sector which contains the specified
// page number.
// The smallest unit of memory which can be erased
// is the 4kB sector (which is 16 pages)
void flash_erase_pages_sector(int pn)
{
  int address;

  write_pause(); 
  // Send Write Enable command
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_WRITE_ENABLE);
  digitalWrite(f_cs, HIGH);
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_SECTOR_ERASE);
  // Send the 3 byte address
  address = page_to_address(pn);
  SPI.transfer((address >> 16) & 0xff);
  SPI.transfer((address >> 8) & 0xff);
  SPI.transfer(address & 0xff);
  digitalWrite(f_cs, HIGH);
  // Indicate that next I/O must wait for this write to finish
  flash_wait_for_write = 1;
}

//=====================================
// Erase the 32kb block which contains the specified
// page number.
void flash_erase_pages_block32k(int pn)
{
  int address;

  write_pause();
  // Send Write Enable command
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_WRITE_ENABLE);
  digitalWrite(f_cs, HIGH);
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_BLOCK32K_ERASE);
  // Send the 3 byte address
  address = page_to_address(pn);
  SPI.transfer((address >> 16) & 0xff);
  SPI.transfer((address >> 8) & 0xff);
  SPI.transfer(address & 0xff);
  digitalWrite(f_cs, HIGH);
  // Indicate that next I/O must wait for this write to finish
  flash_wait_for_write = 1;
}

//=====================================
// Erase the 64kb block which contains the specified
// page number.
void flash_erase_pages_block64k(int pn)
{
  int address;
  
  write_pause();
  // Send Write Enable command
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_WRITE_ENABLE);
  digitalWrite(f_cs, HIGH);
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_BLOCK64K_ERASE);
  // Send the 3 byte address
  address = page_to_address(pn);
  SPI.transfer((address >> 16) & 0xff);
  SPI.transfer((address >> 8) & 0xff);
  SPI.transfer(address & 0xff);
  digitalWrite(f_cs, HIGH);
  // Indicate that next I/O must wait for this write to finish
  flash_wait_for_write = 1;
}

//=====================================
// Tpp Typ=0.64ms Max=5ms
// measured 1667us
void flash_page_program(unsigned char *wp,int pn)
{
  int address;

  write_pause(); 
  // Send Write Enable command
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_WRITE_ENABLE);
  digitalWrite(f_cs, HIGH);
  
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_PAGE_PROGRAM);
  // Send the 3 byte address
  address = page_to_address(pn);
  SPI.transfer((address >> 16) & 0xff);
  SPI.transfer((address >> 8) & 0xff);
  SPI.transfer(address & 0xff);
  // Now write 256 bytes to the page
  for(int i = 0;i < 256;i++) {
    SPI.transfer(*wp++);
  }
  digitalWrite(f_cs, HIGH);
  // Indicate that next I/O must wait for this write to finish
  flash_wait_for_write = 1;
}

//=====================================
// measured = 664us
void flash_read_pages(unsigned char *p,int pn,const int n_pages)
{
  int address;
  unsigned char *rp = p;
  
  write_pause();
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_READ_DATA);
  // Send the 3 byte address
  address = page_to_address(pn);
  SPI.transfer((address >> 16) & 0xff);
  SPI.transfer((address >> 8) & 0xff);
  SPI.transfer(address & 0xff);
  // Now read the page's data bytes
  for(int i = 0;i < n_pages * 256;i++) {
    *rp++ = SPI.transfer(0);
  }
  digitalWrite(f_cs, HIGH);
}

//=====================================
// measured = 442us for one page
// Read specified number of pages starting with pn
void flash_fast_read_pages(unsigned char *p,int pn,const int n_pages)
{
  int address;
  unsigned char *rp = p;
  
  write_pause();
// The chip doesn't run at the higher clock speed until
// after the command and address have been sent
  digitalWrite(f_cs, LOW);
  SPI.transfer(CMD_READ_HIGH_SPEED);
  // Send the 3 byte address
  address = page_to_address(pn);
  SPI.transfer((address >> 16) & 0xff);
  SPI.transfer((address >> 8) & 0xff);
  SPI.transfer(address & 0xff);
  // send dummy byte
  SPI.transfer(0);
  // Double the clock speed
//>>>PAH use DIV2 all the time so don't need this
//  SPI.setClockDivider(SPI_CLOCK_DIV2);
  // Now read the number of pages required
  for(int i = 0;i < n_pages * 256;i++) {
    *rp++ = SPI.transfer(0);
  }
  digitalWrite(f_cs, HIGH);
  // reset the clock speed
//>>>PAH use DIV2 all the time so don't need this
//  SPI.setClockDivider(SPI_CLOCK_DIV4);
}
