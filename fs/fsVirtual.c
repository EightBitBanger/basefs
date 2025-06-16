#include <fs/fs.h>
#include <stdio.h>

uint8_t vfsList(struct Partition part, DirectoryHandle handle) {
    uint32_t refCount = fsDirectoryGetReferenceCount(part, handle);
    
    File index = fsFileOpen(part, handle);
    uint32_t fileSize = fsFileGetSize(part, handle);
    uint8_t buffer[1024];
    
    fsFileRead(part, index, buffer, fileSize);
    
    // Run file list
    for (uint32_t i=0; i < refCount; i++) {
        uint8_t ptrBytes[4];
        for (uint8_t a=0; a < 4; a++) {
            ptrBytes[a] = buffer[ (i * 4) + a ];
            
            //uint8_t character[] = {ptrBytes[a], '\0'};
            //printf((char*)character);
        }
        
        uint32_t fileHandle = *((uint32_t*)&ptrBytes[0]);
        
        // Print file name
        uint8_t filenameTest[] = "          ";
        fsFileGetName(part, fileHandle, filenameTest);
        printf(filenameTest);
        
        /*
        uint8_t filename[10];
        fsFileGetName(part, handle, filename);
        //fsFileGetName(part, fileHandle, filename);
        for (uint8_t a=0; a < 10; a++) {
            uint8_t character[] = {filename[a], '\0'};
            
            printf((char*)character);
        }
        */
        printf("\n");
    }
    
    fsFileClose(index);
    return 0;
}
