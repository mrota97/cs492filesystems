//
// Created by matt on 4/17/18.
//

#ifndef FILESYSTEMS_NODE_H
#define FILESYSTEMS_NODE_H

#include <string>
#include <utility>
#include <vector>

class FileTree {
    std::string path, name;
    off_t size;
    time_t timestamp;
public:
    FileTree* parent;
    std::vector<FileTree*> children;
    FileTree(std::string, FileTree*, std::string, off_t, time_t); // Constructor: file node

    // Getters
    std::string get_path() { return path; }
    std::string get_name() { return name; }
    off_t get_size() { return size; }
    time_t get_timestamp() { return timestamp; }

    int add_child(FileTree*);
    unsigned long num_child() { return children.size(); }
    void update_timestamp(time_t new_timestamp) { timestamp = new_timestamp; }
    // remove_child()
    FileTree* get_node(std::string, std::string);
};

// Class member declarations

FileTree::FileTree(std::string path, FileTree *parent, std::string name= nullptr, off_t size=NULL, time_t timestamp=NULL) \
: path(std::move(path)), parent(parent), name(std::move(name)), size(size), timestamp(timestamp) {}

int FileTree::add_child(FileTree *Child) {
    unsigned long old_size = children.size();
    children.push_back(Child);
    if ((children.size() - old_size) == 1) {
        return 0;
    }
    return 1;
}

FileTree* FileTree::get_node(std::string p, std::string n=NULL) {
    FileTree * temp = this;
    if (n == NULL) {
        // Node returned will be a directory
        if (get_path() == p) {
            return temp;
        }
    }
}

// 

#endif //FILESYSTEMS_NODE_H
