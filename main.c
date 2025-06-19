#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <fs/fs.h>
#include <stdio.h>

extern uint8_t* block;

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { 640, 480};
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    
    fsInit();
    
    struct Partition partA = fsDeviceOpen(0x00000000);
    fsDeviceFormat(partA, 0x000, 0x800, 32);
    
    struct Partition part = fsDeviceOpen(0x00000000);
    
    if (part.block_size == 0) {
        printf("Device not ready\n\n");
        return -304;
    }
    
    DirectoryHandle rootHandle = fsDeviceGetRootDirectory(part);
    
    
    for (uint8_t i=0; i < 4; i++) {
        uint8_t filename[] = "file( )";
        filename[5] = 'A' + i;
        FileHandle fileHandle = fsFileCreate(part, filename, 20);
        
        fsDirectoryAddFile(part, rootHandle, fileHandle);
    }
    
    uint8_t filename[] = "file(E)";
    uint32_t fileHandle = fsDirectoryFindByName(part, rootHandle, filename);
    
    if (fileHandle != 0) {
        
        printf("File found!\n");
        
    }
    
    //fsDirectoryRemoveFile(part, handle, 0);
    
    
    
    vfsList(part, rootHandle);
    
    
    // Print the byte layout
    char character[ ] = {' ', '\0'};
    uint8_t horzCount = 0;
    for (unsigned int i=0; i < 2048; i++) {
        character[0] = block[i];
        if (character[0] < 0x20) character[0] += 0xb0;
        printf(character);
        if (horzCount == 127) {
            character[0] = '\n';
            printf(character);
            
            horzCount=0;
        } else {
            horzCount++;
        }
    }
    horzCount = 0;
    printf("\n");
    
    // Print sector headers
    for (unsigned int i=1; i < 512; i++) {
        character[0] = block[ i * part.sector_size ];
        if (character[0] < 0x20) character[0] = 0x20;
        printf(character);
        if (horzCount == 100) {
            character[0] = '\n';
            printf(character);
            
            horzCount=0;
        } else {
            horzCount++;
        }
    }
    
    free(block);
    
    printf("\n\n");
    return 0;
}

