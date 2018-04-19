//
// Created by matt on 4/19/18.
//

#ifndef FILESYSTEMS_LDISK_H
#define FILESYSTEMS_LDISK_H

class ldisk {
    unsigned long block_start;
    unsigned long block_end;
    bool used;
    ldisk* next;
public:
    ldisk(unsigned long, unsigned long, bool, ldisk*);
    // add()
    // Free spot
    // reallocated
    // combine blocks
};

ldisk::ldisk(unsigned long block_start, unsigned long block_end, bool, ldisk * next) : block_start(block_start), block_end(block_end), next(next) {}

#endif //FILESYSTEMS_LDISK_H
