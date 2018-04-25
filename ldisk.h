//
// Created by matt on 4/19/18.
//

#ifndef FILESYSTEMS_LDISK_H
#define FILESYSTEMS_LDISK_H

#include <iostream>

struct disk_node {
    unsigned long block_start;
    unsigned long block_end;
    bool used;
    disk_node * next;
    friend std::ostream& operator<<(std::ostream& os, const disk_node& d){
        return os << "block(" << d.block_start << "->" << d.block_end << ", "<< d.used <<")";
    }
};

class ldisk {
public:
    disk_node  *head, *tail;
    unsigned long free_blocks, length, disk_size, block_size;
    ldisk();
    ldisk(unsigned long disk, unsigned long block);
    virtual ~ldisk();
    int get_pos(disk_node * block);
    disk_node * find_block(unsigned long block_id);
    int combine();
    int remove_block(disk_node * block);
    int use_block(unsigned long length);
    friend std::ostream &operator<<(std::ostream& os, const ldisk &l){
        os << "[ ";
        for (disk_node * temp = l.head; temp != nullptr; temp=temp->next)
            os << *temp << ", ";
        os << "\b\b" << " ]";
        return os;
    }


/*
    ldisk * find_block(ldisk * target) {
        ldisk * temp = this;
        while (temp != nullptr) {
            if (temp == target)
                return target;
            temp = temp->next;
        }
        std::cerr << "Error: target not found" << std::endl;
        return nullptr;
    }

    ldisk* findParent(ldisk * node, ldisk * target) {
      if(node->next == target){
        return node;
      }else{
        if(node->next == nullptr){
           std::cerr << "Error: target not found" << std::endl;
        }
        return findParent(node->next, target);
      }
    }

    void combine_edit() {
        ldisk * temp = this;
        if (temp->next == nullptr)
            return; // Nothing to do
        while (temp->next != nullptr) {

            temp = temp->next;
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
    }*/
};

ldisk::ldisk() {
    head = nullptr;
    tail = nullptr;
    free_blocks = 0;
    length = 0;
    disk_size = 0;
    block_size = 0;
}

ldisk::ldisk(unsigned long disk, unsigned long block) {
    auto * temp = new disk_node;
    temp->block_start = 0;
    temp->block_end = (disk / block) - 1;
    temp->next = nullptr;
    head = temp;
    tail = temp;
    free_blocks = disk / block;
    disk_size = disk;
    block_size = block;
    length = 1;
}

ldisk::~ldisk() = default;

int ldisk::get_pos(disk_node * block) {
    disk_node * temp = head;
    int pos = 0;
    while (temp != nullptr) {
        if (temp == block) {
            return pos;
        }
        pos++;
        temp = temp->next;
    }
    return -1;
}

disk_node * ldisk::find_block(unsigned long block_id) {
    disk_node * temp = head;
    while (temp != nullptr) {
        if (block_id >= temp->block_start && block_id <= temp->block_end) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

int ldisk::remove_block(disk_node * block) {
    disk_node * temp = head;
    while (temp != nullptr) {
        if (temp->next == block) {
            temp->next = temp->next->next;
            length--;
            return 0;
        }
        temp = temp->next;
    }
    return 1;
}

int ldisk::combine() {
    int changed = 0;
    if (length == 0 || length == 1 || head == nullptr) {
        return changed; // Nothing to do
    }
    disk_node * temp = head;
    while (temp->next != nullptr) {
        disk_node * next = temp->next;
        if (temp->used == next->used) {
            // Combine here
            temp->block_end = next->block_end;
            temp->next = next->next;
            changed++;
        }
        temp = temp->next;
    }
    return changed;
}

int ldisk::use_block(unsigned long length) {
    disk_node * temp = head;

}


#endif //FILESYSTEMS_LDISK_H
