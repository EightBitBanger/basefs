#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <fs/fs.h>
#include <stdio.h>
#include <unistd.h>
#include <conio.h>
#include <string.h>


extern uint8_t* block;

void DrawConsoleOutput(struct Partition part);



int main() {
    
    block = (uint8_t*)malloc(32768 * 10);
    for (uint32_t i=0; i < 32768 * 10; i++) 
        block[i] = ' ';
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { 640, 480};
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    
    fsInit();
    
    struct Partition partA = fsDeviceOpen(0x00000000);
    fsDeviceFormat(&partA, 0, 32 * 20, 32, 0x00, (uint8_t*)"ssd0");
    struct Partition partB = fsDeviceOpen(0x00000400);
    fsDeviceFormat(&partB, 0, 32 * 20, 32, 0x00, (uint8_t*)"ssd1");
    
    DirectoryHandle rootHandleA = fsDeviceGetRootDirectory(partA);
    //DirectoryHandle rootHandleB = fsDeviceGetRootDirectory(partB);
    
    {
    uint32_t subDirectory = fsDirectoryCreate(partA, (uint8_t*)"sub");
    fsDirectoryAddFile(partA, rootHandleA, subDirectory);
    }
    {
    FileHandle fileHandle = fsFileCreate(partA, (uint8_t*)"file", 20);
    fsDirectoryAddFile(partA, rootHandleA, fileHandle);
    }
    
    
    uint32_t subDirectory = fsDirectoryCreate(partB, (uint8_t*)"dir_test");
    fsDirectoryAddFile(partA, rootHandleA, subDirectory);
    
    
    
    
    uint32_t mountedSubDirectory = fsDirectoryMountCreate(partA, partB, subDirectory);
    fsDirectoryAddFile(partA, rootHandleA, mountedSubDirectory);
    
    
    
    
    
    DrawConsoleOutput(partA);
    //vfsList(partA, rootHandle);
    
    //uint32_t dirSz = fsDirectoryGetTotalSize(partA, rootHandleA);
    //printf("\n%u", dirSz);
    
    printf("\n\n");
    
    free(block);
    while(1){}
    return 0;
}







void DrawConsoleOutput(struct Partition part) {
    system("cls");
    
    //vfsList(part, rootHandle);
    
    // Print the byte layout
    char character[ ] = {' ', '\0'};
    uint8_t horzCount = 0;
    for (unsigned int i=0; i < 3000; i++) {
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
    /*
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
    */
    return;
}


