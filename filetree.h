//
// Created by matt on 4/17/18.
//

#ifndef FILESYSTEMS_NODE_H
#define FILESYSTEMS_NODE_H

#include <string>
#include <utility>
#include <vector>
#include "globals.h"
#include "lfile.h"


class FileTree {
public:
    std::string path, name;
    off_t size;
    time_t timestamp;
    FileTree* parent;
    lfile ** file_info;
    std::vector<FileTree*> children;

    // getters
    std::string get_path() { return this->path; }
    std::string get_name() { return this->name; }
    off_t get_size() { return this->size; }
    time_t get_timestamp() { return this->timestamp; }
    unsigned long get_num_child() { return this->children.size(); }

    // setters
    void set_values(std::string, std::string, off_t, time_t, FileTree *);
    void update_timestamp(time_t timestamp) { this->timestamp = timestamp; }

    // methods
    int add_child(FileTree*);
    int remove_child(const std::string &);
    FileTree* get_node(std::string, const std::string &);

    // Helpers
    std::vector<std::string> path_to_vector(std::string, const std::string &);
};

// Class member declarations

void FileTree::set_values(std::string n, std::string p, off_t s, time_t t, FileTree * root) {
    this->name = std::move(n);
    this->path = std::move(p);
    this->size = s;
    this->timestamp = t;
    this->parent = root;
}

int FileTree::add_child(FileTree *child) {
    unsigned long old_size = this->children.size();
    this->children.push_back(child);
    if ((this->children.size() - old_size) == 1) {
        return 0;
    }
    return 1;
}

int FileTree::remove_child(const std::string &path, const std::string &name) {
    FileTree * to_remove = this->get_node(path, name);
}

// Takes in a path string and returns a vector of names
std::vector<std::string> FileTree::path_to_vector(std::string p, const std::string &delimiter) {
    std::vector<std::string> path_names;
    std::string name;
    size_t pos = 0;

    while ((pos = p.find(delimiter)) != std::string::npos) {
        name = p.substr(0, pos);
        path_names.push_back(name);
        p.erase(0, pos + delimiter.length());
    }

    return path_names;
}

// Return a pointer to a node in the tree given its name
FileTree* FileTree::get_node(std::string p, const std::string &n) {
    FileTree * temp = this;
    std::vector<std::string> path_names = temp->path_to_vector(std::move(p), "/");

    for (auto names = path_names.begin(); names != path_names.end(); names++) {
        for (auto children = temp->children.begin(); children != temp->children.end(); children++) {
            if (*names == (*children)->name) {
                // We've found something
                temp = this;
                if (names + 1 == path_names.end()) {    // The node we're on is the
                    return temp;
                } else {                                //
                    break;
                }
            }
        }
    }

    if (temp == this) { // Nothing
        std::cerr << "File " << path_names.back() << " was not found in the file system." << std::endl;
    }

    return this;
}



// 

#endif //FILESYSTEMS_NODE_H
