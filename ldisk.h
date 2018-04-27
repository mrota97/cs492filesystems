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
    friend std::ostream& operator<<(std::ostream& os, const disk_node &d){
        return os << "block(" << d.block_start << "->" << d.block_end << ", "<< d.used <<")";
    }
};

class ldisk {
public:
    disk_node  *head;
    unsigned long free_blocks, length, disk_size, block_size;
    ldisk();
    ldisk(unsigned long disk, unsigned long block);
    virtual ~ldisk();
    void print_footprint();
    int get_pos(disk_node * block);
    disk_node * find_block(unsigned long block_id);
    void combine();
    int remove_block(unsigned long addr);
    unsigned long use_block();
    friend std::ostream &operator<<(std::ostream& os, const ldisk &l){
        os << "[ ";
        disk_node * temp = l.head;
        while (temp != nullptr) {
            os << *temp;
            if (temp->next != nullptr)
                os << ", ";
            temp = temp->next;
        }
        return os << " ] free_blocks: " << l.free_blocks;
    }
};

ldisk::ldisk() {
    head = nullptr;
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
    free_blocks = disk / block;
    disk_size = disk;
    block_size = block;
    length = 1;
}

ldisk::~ldisk() = default;

void ldisk::print_footprint() {
    disk_node * temp = head;
    while (temp != nullptr) {
        if (!temp->used) {
            std::cout << "Free: ";
        } else {
            std::cout << "In use: ";
        }
        if (temp->block_end == temp->block_start) {
            std::cout << temp->block_start << std::endl;
        } else {
            std::cout << temp->block_start << "-" << temp->block_end << std::endl;
        }
        temp = temp->next;
    }
}

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

int ldisk::remove_block(unsigned long addr) {
    disk_node * temp = head;
    unsigned long block_id = addr / block_size;
    while (temp != nullptr) {
        if (block_id > temp->block_start && block_id < temp->block_end) { // Between the two blocks but not one of them
            std::cout << "do stuff" << std::endl;
        } else if (block_id == temp->block_start && block_id == temp->block_end) { // Deletion of a single block
            std::cout << "do stuff" << std::endl;
        } else if (block_id == temp->block_start && block_id < temp->block_end) { // Block is at the beginning of a contiguous set
            std::cout << "do stuff" << std::endl;
        } else if (block_id > temp->block_start && block_id == temp->block_end) { // Block is at the end of a contiguous set
            std::cout << "do stuff" << std::endl;
        }
    }
    return 0;
}

void ldisk::combine() {
    disk_node * temp = head;
    if (length > 1) {
        while (temp->next != nullptr) {
            disk_node * next_node = temp->next;
            if (next_node == nullptr)
                return;
            else if (temp->used == next_node->used) {
                // Combine here
                temp->block_end = next_node->block_end;
                temp->next = next_node->next;
            } else {
                temp = temp->next;
            }
        }
    }
}

// Inserts a block in a free space and returns its block_id
unsigned long ldisk::use_block() {
    disk_node *current = head;
    while (current != nullptr) {
        if (!current->used) {
            if (current->block_start == current->block_end) { // There's one lonely block that's free
                current->used = true;
                free_blocks--;
                length++;
                combine();
                return current->block_start;
            } else { // There's more than one free blocks that can be used here
                auto *use = new disk_node;

                use->block_start = current->block_start + 1;
                use->block_end = current->block_end;
                use->used = false;
                use->next = current->next;

                current->block_end = current->block_start;
                current->used = true;
                current->next = use;
                free_blocks--;
                length++;
                combine();
                return current->block_start;
            }

        }
        current = current->next;
    }
}

#endif //FILESYSTEMS_LDISK_H
