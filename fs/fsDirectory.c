#include <fs/fs.h>

DirectoryHandle fsDirectoryCreate(struct Partition part, uint8_t* filename) {
    
    FileHandle handle = fsFileCreate(part, filename, 61);
    
    fsFileSetAttributes(part, handle, (Filename)" rwd");
    
    return handle;
}
