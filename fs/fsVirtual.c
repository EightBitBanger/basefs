#include <fs/fs.h>
#include <stdio.h>

uint8_t vfsList(struct Partition part, DirectoryHandle handle) {
    uint8_t buffer[1024];
    
    while (handle != 0) {
        uint32_t refCount = fsDirectoryGetReferenceCount(part, handle);
        
        File index = fsFileOpen(part, handle);
        uint32_t directorySize = fsFileGetSize(part, handle);
        
        fsFileRead(part, index, buffer, directorySize);
        
        // Run file list
        for (uint32_t i=0; i < refCount; i++) {
            uint8_t ptrBytes[4];
            for (uint8_t a=0; a < 4; a++) 
                ptrBytes[a] = buffer[ (i * 4) + a ];
            
            uint32_t fileHandle = *((uint32_t*)&ptrBytes[0]);
            uint32_t fileSize = fsFileGetSize(part, fileHandle);
            
            // Print attributes
            uint8_t fileAttr[] = "    ";
            fsFileGetAttributes(part, fileHandle, fileAttr);
            fileAttr[3] = '\0';
            
            printf((char*)fileAttr);
            printf(" ");
            
            // Print file name
            uint8_t filename[] = "          ";
            fsFileGetName(part, fileHandle, filename);
            printf((char*)filename);
            
            // Print file size
            printf(" %u", fileSize);
            
            printf("\n");
        }
        fsFileClose(index);
        
        handle = fsFileGetNextAddress(part, handle);
    }
    return 0;
}
