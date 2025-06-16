#include <fs/fs.h>

FileHandle fsFileCreate(struct Partition part, uint8_t* filename, uint32_t size) {
    FileHandle handle = fsAllocate(part, size);
    if (handle == 0) 
        return 0;
    
    fsFileSetName(part, handle, filename);
    
    uint8_t attributes[] = " rw ";
    fsFileSetAttributes(part, handle, attributes);
    
    fsFileSetReferenceCount(part, handle, 0);
    // Set default flag
    fs_write_byte(part.block_address + handle + FILE_OFFSET_FLAG, 0x00);
    return handle;
}

FileHandle fsFileDelete(struct Partition part, FileHandle handle) {
    
    fsFree(part, handle);
    
    return handle;
}


void fsFileGetName(struct Partition part, FileHandle handle, uint8_t* name) {
    for (uint32_t i=0; i < 10; i++) {
        if (name[i] == '\0') break;
        fs_read_byte(part.block_address + handle + i + FILE_OFFSET_NAME, &name[i]);
    }
    return;
}

void fsFileSetName(struct Partition part, FileHandle handle, uint8_t* name) {
    // Clean up the file name
    uint8_t filename[10];
    for (uint32_t i=0; i < 10; i++) 
        filename[i] = '\0';
    for (uint32_t i=0; i < 10; i++) {
        if (name[i] == '\0') 
            break;
        filename[i] = name[i];
    }
    for (uint32_t i=0; i < 10; i++) 
        fs_write_byte(part.block_address + handle + i + FILE_OFFSET_NAME, filename[i]);
    return;
}

uint32_t fsFileGetSize(struct Partition part, FileHandle handle) {
    uint8_t sizeBytes[4];
    for (uint8_t i=0; i < 4; i++) 
        fs_read_byte(part.block_address + handle + i + FILE_OFFSET_SIZE, &sizeBytes[i]);
    return *((uint32_t*)&sizeBytes[0]);
}


void fsFileSetReferenceCount(struct Partition part, FileHandle handle, uint32_t count) {
    uint8_t sizeBytes[4];
    *((uint32_t*)&sizeBytes[0]) = count;
    for (uint8_t i=0; i < 4; i++) 
        fs_write_byte(part.block_address + handle + i + FILE_OFFSET_REF_COUNT, sizeBytes[i]);
    return;
}

uint32_t fsFileGetReferenceCount(struct Partition part, FileHandle handle) {
    uint8_t sizeBytes[4];
    for (uint8_t i=0; i < 4; i++) 
        fs_read_byte(part.block_address + handle + i + FILE_OFFSET_REF_COUNT, &sizeBytes[i]);
    return *((uint32_t*)&sizeBytes[0]);
}


void fsFileSetAttributes(struct Partition part, FileHandle handle, uint8_t* attributes) {
    for (uint32_t i=0; i < 10; i++) 
        fs_write_byte(part.block_address + handle + i + FILE_OFFSET_ATTRIBUTES, attributes[i]);
    return;
}

void fsFileGetAttributes(struct Partition part, FileHandle handle, uint8_t* attributes) {
    for (uint32_t i=0; i < 10; i++) 
         fs_read_byte(part.block_address + handle + i + FILE_OFFSET_ATTRIBUTES, &attributes[i]);
    return;
}


void fsFileSetFlag(struct Partition part, FileHandle handle, uint8_t index, uint8_t state) {
    uint8_t flag;
    fs_read_byte(part.block_address + handle + FILE_OFFSET_FLAG, &flag);
    if (state == 1) {
        flag |= (1 << index);
    } else {
        flag &= ~(1 << index);
    }
    fs_write_byte(part.block_address + handle + FILE_OFFSET_FLAG, flag);
    return;
}

uint8_t fsFileGetFlag(struct Partition part, FileHandle handle, uint8_t index) {
    uint8_t flag;
    fs_read_byte(part.block_address + handle + FILE_OFFSET_FLAG, &flag);
    return (flag >> index) & 1;
}

void fsFileSetParentAddress(struct Partition part, FileHandle handle, uint32_t parent) {
    uint8_t ptrBytes[4];
    *((uint32_t*)&ptrBytes[0]) = parent;
    for (uint8_t i=0; i < 4; i++) 
        fs_write_byte(part.block_address + handle + i + FILE_OFFSET_PARENT, ptrBytes[i]);
    return;
}

uint32_t fsFileGetParentAddress(struct Partition part, FileHandle handle) {
    uint8_t ptrBytes[4];
    for (uint8_t i=0; i < 4; i++) 
        fs_read_byte(part.block_address + handle + i + FILE_OFFSET_PARENT, &ptrBytes[i]);
    return *((uint32_t*)&ptrBytes[0]);
}


void fsFileSetNextAddress(struct Partition part, FileHandle handle, uint32_t parent) {
    uint8_t ptrBytes[4];
    *((uint32_t*)&ptrBytes[0]) = parent;
    for (uint8_t i=0; i < 4; i++) 
        fs_write_byte(part.block_address + handle + i + FILE_OFFSET_NEXT, ptrBytes[i]);
    return;
}

uint32_t fsFileGetNextAddress(struct Partition part, FileHandle handle) {
    uint8_t ptrBytes[4];
    for (uint8_t i=0; i < 4; i++) 
        fs_read_byte(part.block_address + handle + i + FILE_OFFSET_NEXT, &ptrBytes[i]);
    return *((uint32_t*)&ptrBytes[0]);
}
