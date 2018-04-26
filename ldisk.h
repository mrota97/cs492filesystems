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
    disk_node  *head, *tail;
    unsigned long free_blocks, length, disk_size, block_size;
    ldisk();
    ldisk(unsigned long disk, unsigned long block);
    virtual ~ldisk();
    int get_pos(disk_node * block);
    disk_node * find_block(unsigned long block_id);
    int combine();
    int remove_block(unsigned long block_id);
    unsigned long use_block();
    friend std::ostream &operator<<(std::ostream& os, const ldisk &l){
        os << "[ ";
        disk_node * temp = l.head;
        while (temp != nullptr) {
            os << *temp << ", ";
            temp = temp->next;
        }
        return os << "\b\b" << " ]";
    }
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
    temp->next = NULL;
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

int ldisk::remove_block(unsigned long block_id) {
//    disk_node * temp = head;
//    while (temp != nullptr) {
//        if (temp->next == block) {
//            temp->next = temp->next->next;
//            length--;
//            return 0;
//        }
//        temp = temp->next;
//    }
//    return 1;
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

// Inserts a block in a free space and returns its block_id
unsigned long ldisk::use_block() {
    if (free_blocks == 0) {
        std::cerr << "Out of space." << std::endl;
        return length;
    }
    disk_node * curr = head, * next = head;
    auto * use = new disk_node;
    use->used = true;
    while (curr != nullptr) {
        if (!curr->used) {
            unsigned long block_location = curr->block_start;
            use->block_start = block_location;
            use->block_end = block_location;
            curr->block_start++;

            use->next = curr;
            head = use;

            int check = combine();
            if (check <= 0)
//                std::cerr << "Error: something went wrong in the combining of nodes." << std::endl;
            return block_location;
        } else {
            while (curr->next != nullptr) {
                next = curr->next;
                if (!next->used) {
                    unsigned long block_location = next->block_start;
                    use->block_start = block_location;
                    use->block_end = block_location;
                    next->block_start++;

                    use->next = next;
                    curr->next = use;

                    int check = combine();
                    if (check <= 0)
//                        std::cerr << "Error: something went wrong in the combining of nodes." << std::endl;
                    return block_location;
                }
                curr = curr->next;
            }
        }
    }
}


#endif //FILESYSTEMS_LDISK_H
