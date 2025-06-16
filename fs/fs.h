#ifndef _BASIC_FILE_SYSTEM__
#define _BASIC_FILE_SYSTEM__

#include <stdint.h>
#include <stdlib.h>

#define SECTOR_FREE    '?'
#define SECTOR_HEADER  'U'
#define SECTOR_USED    'D'

#define DEVICE_OFFSET_TYPE        11 // uint8      Device type specifier
#define DEVICE_OFFSET_CAPACITY    12 // uint32     Max capacity of the device in bytes
#define DEVICE_OFFSET_ROOT        16 // uint32     Pointer to the root directory
#define DEVICE_OFFSET_SECT_SZ     20 // uint32     Size of a sector

#define FILE_OFFSET_NAME          1
#define FILE_OFFSET_SIZE          11
#define FILE_OFFSET_ATTRIBUTES    15
#define FILE_OFFSET_REF_COUNT     19
#define FILE_OFFSET_FLAG          23
#define FILE_OFFSET_PARENT        24
#define FILE_OFFSET_NEXT          28

typedef uint32_t FileHandle;
typedef uint32_t DirectoryHandle;
typedef uint8_t* Filename;

struct Partition {
    uint32_t block_address;
    uint32_t block_size;
    
    uint32_t sector_count;
    uint32_t sector_size;
};


void fsInit(void);

void fs_write_byte(uint32_t address, uint8_t data);
void fs_read_byte(uint32_t address, uint8_t* data);

// Partition

struct Partition fsDeviceOpen(uint32_t deviceAddress);
uint32_t fsDeviceGetSize(struct Partition part);
uint32_t fsDeviceGetSectorSize(struct Partition part);

void fsDeviceFormat(struct Partition part, uint32_t begin, uint32_t end, uint32_t sectorSize);

// Allocation

FileHandle fsFileCreate(struct Partition part, uint8_t* filename, uint32_t size);
FileHandle fsFileDelete(struct Partition part, FileHandle handle);

uint32_t fsAllocate(struct Partition part, uint32_t size);
void fsFree(struct Partition part, uint32_t address);

// Directories

DirectoryHandle fsDirectoryCreate(struct Partition part, uint8_t* filename);

// File IO

void fsFileSetName(struct Partition part, FileHandle handle, uint8_t* name);
void fsFileGetName(struct Partition part, FileHandle handle, uint8_t* name);

void fsFileSetReferenceCount(struct Partition part, FileHandle handle, uint32_t count);
uint32_t fsFileGetReferenceCount(struct Partition part, FileHandle handle);

uint32_t fsFileGetSize(struct Partition part, FileHandle handle);

void fsFileSetAttributes(struct Partition part, FileHandle handle, uint8_t* attributes);
void fsFileGetAttributes(struct Partition part, FileHandle handle, uint8_t* attributes);

void fsFileSetFlag(struct Partition part, FileHandle handle, uint8_t index, uint8_t state);
uint8_t fsFileGetFlag(struct Partition part, FileHandle handle, uint8_t index);

#endif
