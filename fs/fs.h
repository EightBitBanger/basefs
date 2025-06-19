#ifndef _BASIC_FILE_SYSTEM__
#define _BASIC_FILE_SYSTEM__

#include <stdint.h>
#include <stdlib.h>

#define MAX_OPEN_FILES  16

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

#define FS_FILE_NOT_FOUND   (-2)
#define FS_FILE_INVALID     (-1)
#define FS_FILE_SUCCESS     (0)

typedef uint32_t FileHandle;
typedef uint32_t DirectoryHandle;
typedef int32_t  File;

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
DirectoryHandle fsDeviceGetRootDirectory(struct Partition part);

void fsDeviceFormat(struct Partition part, uint32_t begin, uint32_t end, uint32_t sectorSize);

// Allocation

FileHandle fsFileCreate(struct Partition part, uint8_t* filename, uint32_t size);
FileHandle fsFileDelete(struct Partition part, FileHandle handle);

FileHandle fsFileExtentCreate(struct Partition part, uint32_t size, uint32_t parentPtr, uint32_t nextPtr);

uint32_t fsAllocate(struct Partition part, uint32_t size);
void fsFree(struct Partition part, uint32_t address);

// Directories

DirectoryHandle fsDirectoryCreate(struct Partition part, uint8_t* filename);
DirectoryHandle fsDirectoryDelete(struct Partition part, DirectoryHandle handle);

DirectoryHandle fsDirectoryExtentCreate(struct Partition part, uint32_t parentPtr, uint32_t nextPtr);

void fsDirectorySetReferenceCount(struct Partition part, DirectoryHandle handle, uint32_t count);
uint32_t fsDirectoryGetReferenceCount(struct Partition part, DirectoryHandle handle);

uint8_t fsDirectoryAddFile(struct Partition part, DirectoryHandle handle, uint32_t file);
uint8_t fsDirectoryRemoveFile(struct Partition part, DirectoryHandle handle, uint32_t file);

uint32_t fsDirectoryFindByName(struct Partition part, DirectoryHandle handle, uint8_t* filename);

// File IO

File fsFileOpen(struct Partition part, FileHandle handle);
uint8_t fsFileClose(File index);
uint8_t fsFileWrite(struct Partition part, File index, uint8_t* buffer, uint32_t size);
uint8_t fsFileRead(struct Partition part, File index, uint8_t* buffer, uint32_t size);

void fsFileSetName(struct Partition part, FileHandle handle, uint8_t* name);
void fsFileGetName(struct Partition part, FileHandle handle, uint8_t* name);

uint32_t fsFileGetSize(struct Partition part, FileHandle handle);

void fsFileSetAttributes(struct Partition part, FileHandle handle, uint8_t* attributes);
void fsFileGetAttributes(struct Partition part, FileHandle handle, uint8_t* attributes);

void fsFileSetFlag(struct Partition part, FileHandle handle, uint8_t index, uint8_t state);
uint8_t fsFileGetFlag(struct Partition part, FileHandle handle, uint8_t index);

void fsFileSetParentAddress(struct Partition part, FileHandle handle, uint32_t parent);
uint32_t fsFileGetParentAddress(struct Partition part, FileHandle handle);

void fsFileSetNextAddress(struct Partition part, FileHandle handle, uint32_t next);
uint32_t fsFileGetNextAddress(struct Partition part, FileHandle handle);

// Virtual file system

uint8_t vfsList(struct Partition part, DirectoryHandle handle);

#endif
