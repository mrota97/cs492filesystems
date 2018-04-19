//
// Created by matt on 4/19/18.
//

#ifndef FILESYSTEMS_LDISK_H
#define FILESYSTEMS_LDISK_H

#include "lfile.h";

// need global free space in main
// need global for root node
class ldisk {
    unsigned long block_start;
    unsigned long block_end;
    bool used;
    ldisk* next;
public:
    ldisk(unsigned long, unsigned long, bool, ldisk*);

    findParent(ldisk * node, ldisk * target){
      if(node.next == target){
        return node;
      }else{
        if(node.next == null){
          // throw error target not found
        }
        return findParent(node.next, target)
      }
    }

    combine(ldisk node){ // combines from node given forwards
      if(node.next == nullptr){ // end of list
        return; // done combining
      }
      if(node.used == node.next.used){ // need to be combined
        node.block_end = node.next.block_end;
        ldisk* temp = node.next.next;
        delete node.next;
        node.next = temp;
        return combine(node.next);
      }else{
        return combine(node.next);
      }
    }

    add(ldisk node, unsigned long free, ldisk root){
      if(free < 1){
        // throw error disk full
      }
      if(node.used){
        return add(node.next, free, root);
      }else{
        ldisk newNode = new ldisk(node.block_start+1,node.block_start+2,true,node.next);
        node.next = *newNode;
        node.block_end = node.block_end+2
        combine(root);
        free--;
        return node.block_start+1;
      }
    }

    remove(ldisk node, unsigned long block, ldisk root){
      if(!node.used && node.next == nullptr){
        // throw error node not found
      }
      if(!node.used){
        return remove(node.next,block);
      }
      if(block.block_start <= location && block.block_end >= location){
        if(location == block.block_start){ // beginning
          ldisk nodeBefore = new ldisk(node.block_start,node.block_start+1,false,node);
          node.block_start = node.block_start+1;
          findParent(root, node).next = *nodeBefore;
        }else if(location == block.block_end){ // end
          ldisk nodeAfter = new ldisk(node.block_end,node.block_end+1,false,node.next);
          node.block_end = node.block_end-1;
          node.next = *nodeAfter;
        }
        else{ //middle
          ldisk nodeBefore = new ldisk(node.block_start,location,true,node);
          ldisk nodeAfter = new ldisk(location+1,node.block_end,true,node.next);
          node.block_start = location;
          node.block_end = location+1;
          node.next = *nodeAfter;
        }
        combine(root);
      }
    }
};

ldisk::ldisk(unsigned long block_start, unsigned long block_end, bool, ldisk * next) : block_start(block_start), block_end(block_end), next(next) {}

#endif //FILESYSTEMS_LDISK_H
