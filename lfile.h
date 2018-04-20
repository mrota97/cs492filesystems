//
// Created by matt on 4/19/18.
//

#ifndef FILESYSTEMS_LFILE_H
#define FILESYSTEMS_LFILE_H

#include "globals.h";
#include "ldisk.h";


class lfile {
    unsigned long start_addr;
    lfile* next;
public:

    lfile lfile() {}

    unsigned long physicalToBlock(unsigned long physical,unsigned long block_size){
      return physical / block_size;
    }

    unsigned long blockToPhysical(unsigned long block, unsigned long block_size){
      return block * block_size;
    }



    // TODO: add ldisk add arguments to addFile
    // TODO: make sure arguments make sense
    // LDISK == GLOBAL
    lfile * addFile(off_t size, long block_size){
      lfile * first = new lfile(blockToPhysical(disk->add(root, free_space, root))); //first node of new linked-list
      lfile * last = first; // used for linking new nodes to last
      long left = block_size;
      while(size > 0){
        if(left == 0){ // if on a new block
          lfile * newlfile = new lfile(disk->add(node,free_space,root));
          last->next = newlfile; // set prior node's next to current node
          last = newlfile; // set most newest node to current node
          left = block_size; //with a new node their is full space available
        }
        if (size >= left){ // space needed >= space left
          size = size - left;
          left = 0;
        }else{ // more space than needed left in current block
          size = 0;
        }
      }
      return first;
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
