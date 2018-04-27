//
// Created by matt on 4/17/18.
//

#ifndef FILESYSTEMS_NODE_H
#define FILESYSTEMS_NODE_H

#include <string>
#include <utility>
#include <vector>

#include "ldisk.h"
#include "lfile.h"

struct tree_node {
    std::string path, name;
    int level;
    bool is_dir;
    off_t size;
    time_t timestamp;
    tree_node * parent;
    std::vector<tree_node*> children;
    lfile file_info;
    friend std::ostream& operator<<(std::ostream &os, const tree_node &n) {
        return os << n.path;
    }
};

class FileTree {
public:
    tree_node * root, * current_dir;
    ldisk disk;
    // Constructors & Destructors
    FileTree();
    FileTree(std::string root_dir, unsigned long disk, unsigned long block);
    virtual ~FileTree();

    //// Helpers
    std::vector<std::string> path_to_vector(std::string path, const std::string &delimiter);
    bool is_empty(std::string name);
    void absoulute_add(std::string path, bool type);
    // Getters
    tree_node* get_node(std::string name);
    tree_node* get_root();
    // Setters
    void set_timestamp(std::string name, time_t time);


    // Creation
    void mkdir(std::string name);
    void create(std::string name);

    // Deletion
    int remove(std::string name);

    // Manip
    void append(std::string name, unsigned long bytes);
    void shorten(std::string name, unsigned long bytes);

    // Directory management
    std::string cwd();
    int cd(std::string path="");

    // Printing
    void prfiles(tree_node * node);
    void footprint();
    void print_dir();
    void print_disk();
    void print_file_info(std::string name);
    friend std::ostream& operator<<(std::ostream &strm, const FileTree &f) {

    }
};

FileTree::FileTree() {
    root = nullptr;
    current_dir = nullptr;
}

FileTree::FileTree(std::string root_dir, unsigned long disk_size, unsigned long block_size) {
    std::string dir = root_dir;
    unsigned long block_s = block_size, disk_s = disk_size;
    tree_node * temp = new tree_node;
    disk = ldisk(disk_s, block_s);
    temp->name = dir;
    temp->path = dir;
    temp->level = 0;
    temp->parent = NULL;
    temp->is_dir = true;
    temp->size = 0;
    temp->timestamp = std::time(nullptr);
    root = temp;
    current_dir = temp;
}

FileTree::~FileTree() {}

// Makes a directory with the specified name in the current directory
void FileTree::mkdir(std::string name) {
    tree_node * dir = current_dir;
    tree_node * temp = new tree_node;
    temp->name = name;
    temp->path = dir->path + name + "/";
    temp->parent = dir;
    temp->level = dir->level + 1;
    temp->is_dir = true;
    temp->size = 0;
    temp->timestamp = std::time(nullptr);
    dir->children.push_back(temp);
}

// Makes a file with the specified name in the current directory
void FileTree::create(std::string name) {
    tree_node * dir = current_dir;
    tree_node * temp = new tree_node;
    temp->name = name;
    temp->path = dir->path + name;
    temp->parent = dir;
    temp->level = dir->level + 1;
    temp->is_dir = false;
    temp->size = 0;
    temp->timestamp = std::time(nullptr);
    dir->children.push_back(temp);
}

// Removes a file or empty directory from the tree
int FileTree::remove(std::string name) {
    tree_node * dir = current_dir;
    int pos = 0;
    for (tree_node* n: dir->children) {
        if (strcmp(name.c_str(), n->name.c_str()) == 0) {
            if (n->is_dir && n->children.size() > 0) {
                std::cerr << "Directory is not empty" << std::endl;
                return 1;
            } else if (n->is_dir && n->children.empty()) {
                dir->children.erase(dir->children.begin()+pos);
                return 0;
            } else {
                // free disk blocks allocated to the file
                // deallocate the file's linked list lFile
                dir->children.erase(dir->children.begin()+pos);
                dir->timestamp = std::time(nullptr);
                return 0;
            }
        }
        pos++;
    }
    std::cerr << "Error: file or directory does not exist!" << std::endl;
    return 1;
}

// Append the specified amount of bytes to the given file
void FileTree::append(std::string name, unsigned long bytes) {
    tree_node * dir = get_node(name);
    lfile file_info = lfile();
    dir->file_info.append_bytes(bytes, &disk);
    dir->size += bytes;
}

// Shorten the given file by the specified amount of bytes
void FileTree::shorten(std::string name, unsigned long bytes) {
    tree_node * dir = get_node(name);
    dir->file_info.remove_bytes(bytes, &disk);
    dir->size -= bytes;
}

// changes the working direcotry 
int FileTree::cd(std::string name) {
    if (name.empty()) {
        current_dir = root;
        return 0;
    } else if (strcmp(name.c_str(), "..") == 0) {
        if (current_dir->parent != nullptr)
            current_dir = current_dir->parent;
        return 0;
    } else if (strcmp(name.c_str(), current_dir->name.c_str()) == 0) {
        // Do nothing
    } else {
        tree_node *dir = get_node(name);
        if (dir->is_dir) {
            if (dir == current_dir) {
                std::cerr << "Error: dir " << name << " was not found!" << std::endl;
                return 1;
            }
            current_dir = dir;
            return 0;
        } else {
            std::cerr << "Error: not a directory!" << std::endl;
            return 1;
        }
    }
}

// Returns a pointer to the node with the given name
tree_node* FileTree::get_node(std::string name) {
    tree_node * dir = current_dir;
    for (tree_node *e: dir->children) {
        if (strcmp(name.c_str(), e->name.c_str()) == 0) {
            return e;
        }
    }
    return dir;
}

tree_node* FileTree::get_root() {
    return root;
}

void FileTree::set_timestamp(std::string name, time_t timestamp) {
    tree_node * node = get_node(name);
    node->timestamp = timestamp;
}

// Returns the current directory
std::string FileTree::cwd() {
    return current_dir->path;
}

bool FileTree::is_empty(std::string name) {
    tree_node * dir = get_node(name);
    return (dir->children.size() == 0);
}

// Given a path, traverse the tree and add the node at that path
void FileTree::absoulute_add(std::string path, bool type) {
    std::vector<std::string> path_names = path_to_vector(path, "/");
    for (std::string &name: path_names) {
        std::cout << name << "->";
    }
    std::cout << "\b\b" << std::endl;
}

// Returns a vector of names in the path given the path string
std::vector<std::string> FileTree::path_to_vector(std::string p, const std::string &delimiter) {
    std::vector<std::string> path_names;
    std::string name;
    size_t pos = 0;

    // Extract the root dir from the path
    name = p.substr(0, 2);
    path_names.push_back(name);
    p.erase(0, 2);

    // Then get the other names
    while ((pos = p.find(delimiter)) != std::string::npos) {
        name = p.substr(0, pos);
        path_names.push_back(name);
        p.erase(0, pos + delimiter.length());
    }

    if (!p.empty())
        path_names.push_back(p);

    return path_names;
}

void FileTree::prfiles(tree_node * node) {
    if (node != nullptr) {
        std::cout << node->path
                  << ", " << node->size
                  << ", " << node->timestamp
                  << ", " << node->file_info << std::endl;
        for (tree_node *n: node->children)
            prfiles(n);
    }
}

void FileTree::print_dir() {
    tree_node * dir = current_dir;
    for (tree_node *n: dir->children) {
        std::cout << n->name << std::endl;
    }
}

void FileTree::print_disk() {
    disk.combine();
    std::cout << disk << std::endl;
}

void FileTree::print_file_info(std::string name) {
    tree_node * node = get_node(name);
    if (!node->is_dir) {
        std::cout << node->file_info << std::endl;
    }
}

void FileTree::footprint() {
    disk.print_footprint();
}

// 

#endif //FILESYSTEMS_NODE_H
