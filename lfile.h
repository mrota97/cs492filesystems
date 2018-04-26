//
// Created by matt on 4/19/18.
//

#ifndef FILESYSTEMS_LFILE_H
#define FILESYSTEMS_LFILE_H

struct file_node {
    unsigned long start_addr;
    file_node * next;
    unsigned long free;
    friend std::ostream& operator<<(std::ostream &os, const file_node &n) {
      return os << "file(start addr: " << n.start_addr << ")";
    }
};

class lfile {
    public:
    file_node * head, * tail;
    unsigned long size;
    lfile();
    virtual ~lfile();
    file_node* add_block(ldisk * disk);
    int remove_block(file_node * block, ldisk * disk);
    int append_bytes(unsigned long bytes, ldisk * disk);
    int remove_bytes(unsigned long bytes, ldisk * disk);
    unsigned long wasted_space();

    friend std::ostream &operator<<(std::ostream& os, const lfile &l){
      os << "[ ";
      file_node * temp = l.head;
      while (temp != nullptr) {
        os << *temp << ", ";
        temp = temp->next;
      }
      return os << "\b\b" << "]";
    }
};

lfile::lfile() {
  head = nullptr;
  tail = nullptr;
  size = 0;
}

lfile::~lfile() {}

file_node* lfile::add_block(ldisk * disk) { //returns file block that is the new tail
  // call ldisk
  // increase size
  // return new tail
  unsigned long block_id = disk->use_block();
  if (block_id == disk->block_size) // This means that the disk is out of space.
    return nullptr;
  file_node * block = new file_node;
  block->start_addr = block_id * disk->block_size;
  block->next = NULL;
  block->free = disk->block_size;
  return block;
}

int lfile::remove_block(file_node * block, ldisk * disk) { // returns file block that is the new tail
  // call ldisk
  // decrease size
  // return new tail
  int check = disk->remove_block(block->start_addr);
}

int lfile::append_bytes(unsigned long bytes, ldisk * disk) {
  // get tail of lfile (a file_node)
  // while(file_nodes.free < bytes)
  //   bytes -= file_nodes.free
  //   file_nodes.free = 0
  //   file_node = add_block
  // file_node.free -= bytes
  // return 0
  file_node * current_block = tail, * temp;
  if (tail == nullptr) {
    current_block = add_block(disk);
    tail = head = current_block;
  } else {
    while(current_block->free < bytes){
      bytes -= current_block->free;
      current_block->free = 0;
      temp = add_block(disk);
      if (temp == nullptr) {
        return 1;
      }
      current_block->next = temp;
      current_block = current_block->next;
    }
  }
  current_block->free -= bytes;
  return 0;
}

int lfile::remove_bytes(unsigned long bytes, ldisk * disk) {
  file_node* current_block = tail;
  if (tail == nullptr) {
    return 1;
  }
  else {
    unsigned long used = disk->block_size - current_block->free; // used space in the block
    while(used <= bytes){
      bytes -= used;
      used = 0;
      int check = remove_block(current_block, disk);
      if (check == 0) {
        // it was successful

      }
    }
    current_block->free += bytes;
  }
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
