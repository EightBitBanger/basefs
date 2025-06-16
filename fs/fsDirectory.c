#include <fs/fs.h>

DirectoryHandle fsDirectoryCreate(struct Partition part, uint8_t* filename) {
    
    FileHandle handle = fsFileCreate(part, filename, 61);
    
    // Set number of entries in this directory
    fsDirectorySetReferenceCount(part, handle, 0);
    // Mark as a directory 'd'
    fsFileSetAttributes(part, handle, (uint8_t*)" rwd");
    
    return handle;
}

DirectoryHandle fsDirectoryDelete(struct Partition part, DirectoryHandle handle) {
    
    // TODO Purge any files / directories currently in this directory
    
    fsFileDelete(part, handle);
    
    return handle;
}


void fsDirectorySetReferenceCount(struct Partition part, DirectoryHandle handle, uint32_t count) {
    uint8_t sizeBytes[4];
    *((uint32_t*)&sizeBytes[0]) = count;
    for (uint8_t i=0; i < 4; i++) 
        fs_write_byte(part.block_address + handle + i + FILE_OFFSET_REF_COUNT, sizeBytes[i]);
    return;
}

uint32_t fsDirectoryGetReferenceCount(struct Partition part, DirectoryHandle handle) {
    uint8_t sizeBytes[4];
    for (uint8_t i=0; i < 4; i++) 
        fs_read_byte(part.block_address + handle + i + FILE_OFFSET_REF_COUNT, &sizeBytes[i]);
    return *((uint32_t*)&sizeBytes[0]);
}


uint8_t fsDirectoryAddFile(struct Partition part, DirectoryHandle handle, uint32_t file) {
    uint32_t fileSize = fsFileGetSize(part, handle);
    uint32_t refCount = fsDirectoryGetReferenceCount(part, handle);
    fsDirectorySetReferenceCount(part, handle, refCount+1);
    
    File index = fsFileOpen(part, handle);
    
    // Read directory references
    uint8_t buffer[fileSize + 32];
    fsFileRead(part, index, buffer, fileSize);
    
    // Add new reference address
    uint8_t refBytes[4];
    *((uint32_t*)&refBytes[0]) = file;
    for (uint8_t i=0; i < 4; i++) 
        buffer[((refCount) * 4) + i] = refBytes[i];
    
    // Write back references
    fsFileWrite(part, index, buffer, fileSize);
    fsFileClose(index);
    
    return 1;
}

uint8_t fsDirectoryRemoveFile(struct Partition part, DirectoryHandle handle, uint32_t file) {
    
    
    /*
    uint32_t currentAddress = directoryAddress;
    
    for (uint32_t z = 0; z < FS_DIRECTORY_LISTING_MAX; z++) {
        
        uint32_t numberOfFiles = fsDirectoryGetNumberOfFiles(currentAddress);
        
        for (uint8_t index = 0; index < numberOfFiles; index++) {
            
            uint32_t currentFileAddress = fsDirectoryGetFile(currentAddress, index);
            
            if (currentFileAddress != fileAddress) 
                continue;
            
            // Free the file first
            fsFree(fileAddress);
            
            // Check if the directory block can be freed
            if ((numberOfFiles - 1) == 0) {
                
                // Reroute the next/parent pointers
                uint32_t nextAddress = fsDirectoryGetNext(currentAddress);
                uint32_t parentAddress = fsDirectoryGetParent(currentAddress);
                
                if (parentAddress != 0) {
                    
                    if (nextAddress == 0) {
                        
                        fsDirectorySetNext(parentAddress, 0);
                        
                    } else {
                        
                        fsDirectorySetParent(nextAddress, parentAddress);
                        fsDirectorySetNext(parentAddress, nextAddress);
                    }
                    
                    // Free the directory block
                    // TODO fix Will delete the directory file its self and should not
                    //fsFree(currentAddress);
                }
                
            } else {
                
                // Move the last file reference to the current index
                uint32_t lastFileRef = fsDirectoryGetFile(currentAddress, numberOfFiles - 1);
                fsDirectorySetFile(currentAddress, index, lastFileRef);
                
                // Clear the last file reference
                fsDirectorySetFile(currentAddress, numberOfFiles - 1, 0);
            }
            
            // Decrement the file counter
            numberOfFiles--;
            fsDirectorySetNumberOfFiles(currentAddress, numberOfFiles);
            
            return 1;
        }
        
        // Get the pointer to the next directory block
        currentAddress = fsDirectoryGetNext(currentAddress);
        
        // Check if it's the last block
        if (currentAddress == 0) 
            return 0;
    }
    */
    
    return 0;
}

