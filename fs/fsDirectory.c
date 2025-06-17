#include <fs/fs.h>

DirectoryHandle fsDirectoryCreate(struct Partition part, uint8_t* filename) {
    FileHandle handle = fsFileCreate(part, filename, 30);
    
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

DirectoryHandle fsDirectoryExtentCreate(struct Partition part, uint32_t parentPtr, uint32_t nextPtr) {
    uint8_t filename[] = "EXTENT";
    FileHandle handle = fsFileCreate(part, filename, 30);
    
    // Set number of entries in this directory extent
    fsDirectorySetReferenceCount(part, handle, 0);
    // Mark as an extent 'E'
    fsFileSetAttributes(part, handle, (uint8_t*)"    ");
    
    // Set the extent chain
    fsFileSetNextAddress(part, handle, nextPtr);
    fsFileSetParentAddress(part, handle, parentPtr);
    
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
    
    // Check if the directory is full
    if (((refCount+1) * 4) >= part.sector_size) {
        // Get next extent
        DirectoryHandle extent = fsFileGetNextAddress(part, handle);
        
        // If no extent was found, generate a new one
        if (extent == 0) {
            extent = fsDirectoryExtentCreate(part, handle, extent);
            fsFileSetNextAddress(part, handle, extent);
        } else {
            
            // Check extents recursively
            while (extent != 0) {
                
                // Check extent
                uint32_t extentRefCount = fsDirectoryGetReferenceCount(part, extent);
                
                // Check if the next extent is full
                if (((extentRefCount+1) * 4) >= part.sector_size) {
                    
                    // Generate new extent if no further extents exist
                    DirectoryHandle nextExtent = fsFileGetNextAddress(part, extent);
                    if (nextExtent == 0) {
                        //DirectoryHandle parentExtent = fsFileGetParentAddress(part, extent);
                        
                        //DirectoryHandle currentExtent = extent;
                        extent = fsDirectoryExtentCreate(part, extent, 0);
                        
                        read me
                        // Somewhere in here the parent / next pointers arent being set properly
                        
                        //fsFileSetNextAddress(part, handle, extent);
                        break;
                    }
                    
                } else {
                    // Room exists in this extent
                    break;
                }
                
                // Get next extent
                extent = fsFileGetNextAddress(part, extent);
            }
            
        }
        
        // Set the target extent
        handle = extent;
        
        refCount = fsDirectoryGetReferenceCount(part, handle);
    }
    
    fsDirectorySetReferenceCount(part, handle, refCount+1);
    
    File index = fsFileOpen(part, handle);
    
    // Read directory references
    uint8_t buffer[fileSize + part.sector_size];
    fsFileRead(part, index, buffer, fileSize);
    
    // Add new reference address
    uint8_t refBytes[4];
    *((uint32_t*)&refBytes[0]) = file;
    for (uint8_t i=0; i < 4; i++) 
        buffer[(refCount * 4) + i] = refBytes[i];
    
    // Write back references
    fsFileWrite(part, index, buffer, fileSize);
    fsFileClose(index);
    
    return 1;
}

uint8_t fsDirectoryRemoveFile(struct Partition part, DirectoryHandle handle, uint32_t file) {
    
    return 0;
}

