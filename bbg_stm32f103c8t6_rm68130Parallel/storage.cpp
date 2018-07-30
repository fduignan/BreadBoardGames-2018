#include "storage.h"
#include "stm32f103.h"
#include "jingledesign.h"
#include "invaders.h"
#include <stdint.h>

extern uint32_t PERSISTENT_DATA_START;
void storage::begin()
{
    StorageLocation = (uint32_t) (&PERSISTENT_DATA_START);
    CurrentRecordAddress = FindMagicNumber();
    if (CurrentRecordAddress) // if there is a valid record then read it
    {
        readData();
    }
    else
    {
        // create initial valid record based on default values
        for (int i=0;i<STARTUP_JINGLE_LENGTH;i++)
        {            
            nvm_data.StartupJingleNotes[i] = DefaultStartupJingleTones[i];
            nvm_data.StartupJingleTimes[i] = DefaultStartupJingleTimes[i];                        
        }
        for (int i=0;i<sizeof(DefaultInvaderImage)/sizeof(uint16_t);i++)
        {
            nvm_data.InvaderSprite[i]=DefaultInvaderImage[i];
        }
        nvm_data.HighScoreInvaders=0;
        nvm_data.HighScoreBreakout=0;
        writeData();
    }
}
int  storage::writeToSector(uint32_t Address,void * values, uint16_t size)
{              
    int RValue = 0;
    uint16_t *AddressPtr;
    uint16_t *valuePtr;
    AddressPtr = (uint16_t *)Address;
    valuePtr=(uint16_t *)values;
    size = size / 2;  // incoming value is expressed in bytes, not 16 bit words
    disable_interrupts();
    while(size) {        
        // unlock the flash 
        // Key 1 : 0x45670123
        // Key 2 : 0xCDEF89AB
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xCDEF89AB;
        FLASH->CR &= ~BIT1; // ensure PER is low
        FLASH->CR |= BIT0;  // set the PG bit        
        *(AddressPtr) = *(valuePtr);
        while(FLASH->SR & BIT0); // wait while busy
        
        if (FLASH->SR & BIT2)
        {
            RValue= -1; // flash not erased to begin with
            break;
        }
        if (FLASH->SR & BIT4)
        {
            RValue= -2; // write protect error
            break;
        }
        AddressPtr++;
        valuePtr++;
        size--;
    }   
    FLASH->KEYR = 0; // lock the flash
    enable_interrupts();
    return RValue;    
}

void storage::eraseSector(uint32_t SectorStartAddress)
{
    disable_interrupts();
    // unlock the flash
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    FLASH->CR &= ~BIT0;  // Ensure PG bit is low
    FLASH->CR |= BIT1; // set the PER bit
    FLASH->AR = SectorStartAddress;
    FLASH->CR |= BIT6; // set the start bit 
    while(FLASH->SR & BIT0); // wait while busy
    FLASH->KEYR = 0; // lock the flash
    enable_interrupts();

}
void storage::readFromSector(uint32_t SectorStartAddress, void * values, uint16_t size)
{
    uint16_t *AddressPtr;
    uint16_t *valuePtr;
    AddressPtr = (uint16_t *)SectorStartAddress;
    valuePtr=(uint16_t *)values;
    size = size/2; // incoming value is expressed in bytes, not 16 bit words
    while(size)
    {
        *((uint16_t *)valuePtr)=*((uint16_t *)AddressPtr);
        valuePtr++;
        AddressPtr++;
        size--;
    }

}
uint32_t storage::FindMagicNumber()
{
    // assumes 8 byte magic number/phrase
    // make pointer to start of FLASH sector 
    unsigned char *Address=(unsigned char *)StorageLocation;
    while (Address < (unsigned char *)(StorageLocation + SectorSize))
    {
        if (Address[0]==MagicNumber[0])
            if (Address[1]==MagicNumber[1])
                if (Address[2]==MagicNumber[2])
                    if (Address[3]==MagicNumber[3])
                        if (Address[4]==MagicNumber[4])
                            if (Address[5]==MagicNumber[5])
                                if (Address[6]==MagicNumber[6])
                                    if (Address[7]==MagicNumber[7])
                                    {
                                        return (uint32_t)Address;                                        
                                    }
        Address++;
    }
    return 0; 
}
int storage::writeData()
{
    if (CurrentRecordAddress)
    {
        // Is there enough room to write a new record without a sector erase?
        uint32_t SpaceRequired = sizeof(nvm_data)+sizeof(MagicNumber); // how much space is needed?
        // Flash is written in units of 16 bits so need to round up size to nearest even number 
        if (SpaceRequired % 2) // odd? 
            SpaceRequired++;   // yes so round up to even
        
        if ( (CurrentRecordAddress+SpaceRequired) <= (StorageLocation+SectorSize) )
        {
            // there is enough space so zero out the current record
            uint16_t Zero=0;
            uint32_t Index=0;
            while(Index < SpaceRequired)
            {
                writeToSector(CurrentRecordAddress+Index,&Zero,2);
                Index+=2;
            }
            CurrentRecordAddress += SpaceRequired; // point to next available area of flash
        }
        else 
        {
            // not enough room so need to erase the sector and move back to the start of it
            eraseSector(StorageLocation);
            CurrentRecordAddress = StorageLocation;
        }
    }
    else
    {
        // no records in storage to date so point at start of sector
        CurrentRecordAddress = StorageLocation;
    }
    writeToSector(CurrentRecordAddress,(void *)MagicNumber,sizeof(MagicNumber));
    writeToSector(CurrentRecordAddress+sizeof(MagicNumber),&nvm_data,sizeof(nvm_data));
    return 0;
}
int storage::readData()
{
    if (CurrentRecordAddress)
    {
        readFromSector(CurrentRecordAddress+sizeof(MagicNumber),&nvm_data, sizeof(nvm_data));
        return 0;
    }
    else
        return -1;
}
void storage::eraseData()
{
    eraseSector(StorageLocation);
    CurrentRecordAddress = 0;
}
