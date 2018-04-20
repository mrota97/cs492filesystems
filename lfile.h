//
// Created by matt on 4/19/18.
//

#include "ldisk.h";

#ifndef FILESYSTEMS_LFILE_H
#define FILESYSTEMS_LFILE_H

class lfile {
    unsigned long start_addr;
    lfile* next;
public:
    physicalToBlock(unsigned long physical,unsigned long blockSize){
      return b / block_size;
    }
    blockToPhysical(unsigned long block, unsigned long block_size){
      return b * block_size;
    }

    // TODO: add ldisk add arguments to addFile
    // TODO: make sure arguments make sense
    addFile(off_t size, long block_size){
      lfile first = new first(ldisk.add(node,free,root)); //first node of new linked-list
      lfile* last = first; // used for linking new nodes to last
      long left = block_size;
      while(off_t > 0){
        if(left == 0){ // if on a new block
          lfile newlfile = new lfile(ldisk.add(node,free,root));
          last.next = *newlfile; // set prior node's next to current node
          last = *newlfile; // set most newest node to current node
          left = block_size; //with a new node their is full space available
        }
        if (off_t >= left){ // space needed >= space left
          off_t = off_t - left;
          left = 0;
        }else{ // more space than needed left in current block
          off_t = 0;
        }
      }
      return start
    }


    //deleteFile(){
    //
    //}
    //appendFile(){
    //
    //}
    //shortenFile(){
    //
    //}

};

#endif //FILESYSTEMS_LFILE_H
