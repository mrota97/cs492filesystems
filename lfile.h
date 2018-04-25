//
// Created by matt on 4/19/18.
//

#ifndef FILESYSTEMS_LFILE_H
#define FILESYSTEMS_LFILE_H


struct file_block {
    unsigned long start_addr;
    file_block * next;
    unsigned long free;
};

class lfile {
    public:
    file_block * head, * tail;
    unsigned long size;
    lfile();
    virtual ~lfile();
    unsigned long physical_to_block(unsigned long physical, unsigned long block_size);
    unsigned long block_to_physical(unsigned long block, unsigned long block_size);
    file_block* add_block();
    file_block* remove_block();
    int append_bytes(unsigned long bytes);
    int remove_bytes(unsigned long bytes, unsigned long block_size);
};

lfile::lfile() {}

lfile::~lfile() {}

unsigned long lfile::physical_to_block(unsigned long physical, unsigned long block_size) {}

unsigned long lfile::block_to_physical(unsigned long block, unsigned long block_size) {}

file_block* lfile::add_block() { //returns file block that is the new tail
  // call ldisk
  // increase size
  // return new tail
}

file_block* lfile::remove_block() { // returns file block that is the new tail
  // call ldisk
  // decrease size
  // return new tail
}

int lfile::append_bytes(unsigned long bytes) {
  // get tail of lfile (a file_block)
  // while(file_blocks.free < bytes)
  //   bytes -= file_blocks.free
  //   file_blocks.free = 0
  //   file_block = add_block
  // file_block.free -= bytes
  // return 0
  file_block* current_block = tail;
  while(current_block->free <= bytes){
    bytes -= current_block->free;
    current_block->free = 0;
    current_block = add_block();
  }
  current_block->free -= bytes;
  return 0;
}

int lfile::remove_bytes(unsigned long bytes, unsigned long block_size) {
  file_block* current_block = tail;
  unsigned long used = block_size - current_block->free; // used space in the block
  while(used <= bytes){
    bytes -= used;
    used = 0;
    current_block = remove_block();
  }
  current_block->free += bytes;
  return 0;
}

//    // TODO: add ldisk add arguments to addFile
//    // TODO: make sure arguments make sense
//    // LDISK == GLOBAL
//    lfile * addFile(off_t size, long block_size){
//      lfile * first = new lfile(blockToPhysical(disk->add(root, free_space, root))); //first node of new linked-list
//      lfile * last = first; // used for linking new nodes to last
//      long left = block_size;
//      while(size > 0){
//        if(left == 0){ // if on a new block
//          lfile * newlfile = new lfile(disk->add(node,free_space,root));
//          last->next = newlfile; // set prior node's next to current node
//          last = newlfile; // set most newest node to current node
//          left = block_size; //with a new node their is full space available
//        }
//        if (size >= left){ // space needed >= space left
//          size = size - left;
//          left = 0;
//        }else{ // more space than needed left in current block
//          size = 0;
//        }
//      }
//      return first;
//    }
//};
#endif //FILESYSTEMS_LFILE_H
