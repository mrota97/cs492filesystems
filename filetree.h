//
// Created by matt on 4/17/18.
//

#ifndef FILESYSTEMS_NODE_H
#define FILESYSTEMS_NODE_H

#include <string>
#include <utility>
#include <vector>

class FileTree {
public:
    std::string path, name;
    off_t size;
    time_t timestamp;
    FileTree* parent;
    std::vector<FileTree*> children;

    // Getters
    std::string get_path() { return path; }
    std::string get_name() { return name; }
    off_t get_size() { return size; }
    time_t get_timestamp() { return timestamp; }

    // setters
    void set_values(std::string, std::string, off_t, time_t, FileTree *);
    int add_child(FileTree*);
    unsigned long num_child() { return children.size(); }
    void update_timestamp(time_t new_timestamp) { timestamp = new_timestamp; }
    // remove_child()
    FileTree* get_node(std::string, std::string);
};

// Class member declarations

void FileTree::set_values(std::string n, std::string p, off_t s, time_t t, FileTree * root) {
    name = std::move(n);
    path = std::move(p);
    size = s;
    timestamp = t;
    parent = root;
}

int FileTree::add_child(FileTree *Child) {
    unsigned long old_size = children.size();
    children.push_back(Child);
    if ((children.size() - old_size) == 1) {
        return 0;
    }
    return 1;
}

FileTree* FileTree::get_node(std::string p, std::string n) {
    FileTree * temp = this;
    return this;
}

// 

#endif //FILESYSTEMS_NODE_H
