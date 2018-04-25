//
// Created by matt on 4/17/18.
//

#ifndef FILESYSTEMS_NODE_H
#define FILESYSTEMS_NODE_H

#include <string>
#include <utility>
#include <vector>
#include <ctime>
#include "ldisk.h"
#include "lfile.h"

struct file_node {
    std::string path, name;
    bool is_dir;
    off_t size;
    time_t timestamp;
    file_node * parent;
    std::vector<file_node*> children;
    lfile file_info;
    friend std::ostream& operator<<(std::ostream &os, const file_node &n) {
        return os << n.path;
    }
};

class FileTree {
    file_node * root, * current_dir;
public:
    // Constructors & Destructors
    FileTree();
    FileTree(std::string root_dir);
    virtual ~FileTree();

    // Helpers
    std::vector<std::string> path_to_vector(std::string path, const std::string &delimiter);
    file_node* get_node(std::string path);

    // Creation
    void mkdir(std::string name);
    void create(std::string name);

    // Deletion
    int remove(std::string name);

    // Manip
    void append(std::string name, unsigned long bytes);
    void shorten(std::string name, unsigned long bytes);

    // Directory management
    file_node* cwd();
    void cd(std::string path);

    // Printing
    void print_dir();
    friend std::ostream& operator<<(std::ostream &strm, const FileTree &f) {

    }
};

FileTree::FileTree() {
    root = nullptr;
    current_dir = nullptr;
}

FileTree::FileTree(std::string root_dir) {
    file_node * temp = new file_node;
    temp->name = root_dir;
    temp->path = root_dir;
    temp->is_dir = true;
    temp->size = 0;
    temp->timestamp = 0;
    temp = root;
    temp = current_dir;
}

FileTree::~FileTree() {}

// Makes a directory with the specified name in the current directory
void FileTree::mkdir(std::string name) {
    file_node * dir = current_dir;
    file_node * temp = new file_node;
    temp->name = name;
    temp->path = dir->path + name + "/";
    temp->is_dir = true;
    temp->size = 0;
    temp->timestamp = 0;
    dir->children.push_back(temp);
}

// Makes a file with the specified name in the current directory
void FileTree::create(std::string name) {
    file_node * dir = current_dir;
    file_node * temp = new file_node;
    temp->name = name;
    temp->path = dir->path + name + "/";
    temp->is_dir = false;
    temp->size = 0;
    temp->timestamp = std::time(nullptr);
    dir->children.push_back(temp);
}

// Removes a file or empty directory from the tree
int FileTree::remove(std::string name) {
    file_node * dir = current_dir;
    int pos = 0;
    for (file_node* n: dir->children) {
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
void FileTree::append(std::string name, unsigned long bytes) {}

// Shorten the given file by the specified amount of bytes
void FileTree::shorten(std::string name, unsigned long bytes) {}

// changes the working direcotry 
void FileTree::cd(std::string path = "") {
    if (path.empty()) {
        current_dir = root;
    } else if (strcmp(path.c_str(), "..") == 0) {
        current_dir = current_dir->parent;
    } else {
        file_node * dir = get_node(path);
        if (dir->is_dir) {
            current_dir = dir;
        } else {
            std::cerr << "Error: not a directory!" << std::endl;
        }
    }
}

// Returns a pointer to the node with the given path
file_node* FileTree::get_node(std::string path) {
    file_node * temp = root;
    std::vector<std::string> path_names = path_to_vector(path, "/");
    while (temp != nullptr) {
        for (file_node *e: temp->children) {
            if (strcmp(path_names.front().c_str(), e->name.c_str()) == 0) {
                if (path_names.size() == 1) {
                    return e;
                }
                temp = e;
            }
        }
    }
}

// Returns the current directory
file_node* FileTree::cwd() {
    return current_dir;
}

// Returns a vector of names in the path given the path string
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

//void FileTree::print_dir() {
//    file_node * dir = current_dir;
//    for (file_node *n: dir->children) {
//        std::cout <<
//    }
//}


//void FileTree::debug_print() {
//    std::cout << "node(" <<
//              this->name << ", " <<
//              this->path << ", " <<
//              this->size << " Blocks, time{" <<
//              this->timestamp << "}" << std::endl;
//    std::cout << this->name << "'s file_info: " << this->file_info << std::endl;
//};

// 

#endif //FILESYSTEMS_NODE_H
