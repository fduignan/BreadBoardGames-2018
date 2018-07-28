// persistent storage management
#include <stdint.h>
typedef struct {
    uint16_t InvaderSprite[10*16]; // User custom invader sprite
} nvm_data_type;

class storage
{
public:    
    storage(){};
    void begin();    
    int writeData();
    int readData();
    void eraseData();
    int DataPresent()
    {
        if (CurrentRecordAddress)
            return 1;
        else
            return 0;
    }
    nvm_data_type nvm_data;
private:
/* FLASH memory is used for storage.  The address of the variable StorageLocation represents the start address of the flash sector 
 * used for storage.  In an effort to lengthen the lifetime of this sector the following write algorithm will be used:
 * First write: Store magic number and data to start of sector
 * Subsequent writes : zero out current magic number and data.  
 * If there is room after this memory, write magic number and new data
 * If there is no room then erase sector and behave as if it is the first write
 * If the magic number is not present within the sector then it is safe to assume that default values can be used for non-volatile data
 */
    const char MagicNumber[8]= {'M','A','G','I','C','4','2','#'};
    const uint32_t SectorSize=1024;
    uint32_t StorageLocation;
    uint32_t CurrentRecordAddress;  
    int writeToSector(uint32_t Address,void * values, uint16_t size);
    void eraseSector(uint32_t SectorStartAddress);
    void readFromSector(uint32_t SectorStartAddress, void * values, uint16_t size);
    uint32_t FindMagicNumber();

};
