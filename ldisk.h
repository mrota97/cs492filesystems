//
// Created by matt on 4/19/18.
//

#ifndef FILESYSTEMS_LDISK_H
#define FILESYSTEMS_LDISK_H

#include <iostream>;
#include "globals.h";
#include "lfile.h";

// need global free space in main
// need global for root node
class ldisk {
    unsigned long block_start;
    unsigned long block_end;
    bool used{};
    ldisk* next;
public:
    ldisk(unsigned long, unsigned long, bool, ldisk*);

    ldisk* findParent(ldisk * node, ldisk * target){
      if(node->next == target){
        return node;
      }else{
        if(node->next == nullptr){
           std::cerr << "Error: target not found" << std::endl;
        }
        return findParent(node->next, target);
      }
    }

    void combine(ldisk * node){ // combines from node given forwards
      if(node->next == nullptr){ // end of list
        return; // done combining
      }
      if(node->used == node->next->used){ // need to be combined
        node->block_end = node->next->block_end;
        auto * temp = node->next->next;
        delete node->next;
        node->next = temp;
        return combine(node->next);
      }else{
        return combine(node->next);
      }
    }

    unsigned long add(ldisk * node, ldisk * root){
      if(free_space < 1){
        std::cerr << "Error: disk full" << std::endl;
      }
      if(node->used){
        return add(node->next, root);
      }else{
        auto * newNode = new ldisk(node->block_start+1,node->block_start+2,true,node->next);
        node->next = newNode;
        node->block_end = node->block_end+2;
        combine(root);
        free_space--;
        return node->block_start+1;
      }
    }

    void remove(ldisk * node, unsigned long location, ldisk * root){
      if(!node->used && node->next == nullptr){
        std::cerr << "Error: node not found" << std::endl;
      }
      if(!node->used){
        return remove(node->next, location, root);
      }
      if(node->block_start <= location && node->block_end >= location){
        if(location == node->block_start){ // beginning
          auto * nodeBefore = new ldisk(node->block_start,node->block_start+1,false,node);
          node->block_start = node->block_start+1;
          findParent(root, node)->next = nodeBefore;
        }else if(location == node->block_end){ // end
          auto * nodeAfter = new ldisk(node->block_end,node->block_end+1,false,node->next);
          node->block_end = node->block_end-1;
          node->next = nodeAfter;
        }
        else{ //middle
          auto * nodeBefore = new ldisk(node->block_start,location,true,node);
          auto * nodeAfter = new ldisk(location+1,node->block_end,true,node->next);
          node->block_start = location;
          node->block_end = location+1;
          node->next = nodeAfter;
        }
        combine(root);
        return;
      }
    }
};

ldisk::ldisk(unsigned long block_start, unsigned long block_end, bool, ldisk * next) : block_start(block_start), block_end(block_end), next(next) {}

#endif //FILESYSTEMS_LDISK_H
