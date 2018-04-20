#include <iostream>
#include <getopt.h>
#include <fstream>
#include <fcntl.h>
#include <cstring>
#include "filetree.h"
#include "ldisk.h"
#include "globals.h"


static void show_usage(char* progname) {
    std::cerr << "Usage: " << progname <<
              "[-f input files storing information on files]" <<
              "[-d input files storing information on directories]" <<
              "[-s disk size]" <<
              "[-b block size]" << std::endl;
}

int main(int argc, char* argv[]) {
    unsigned long disk_size, block_size;
    int flags = NULL, opt;
    char *file_list = nullptr, *dir_list = nullptr;

    while ((opt = getopt(argc, argv, "f:d:s:b:")) != -1) {
        switch (opt) {
            case 'f':
                // Open the file list text file
                file_list = optarg;
                break;
            case 'd':
                // Open the directory list text file
                dir_list = optarg;
                break;
            case 's':
                disk_size = strtoul(optarg, nullptr, 0);
                break;
            case 'b':
                block_size = static_cast<unsigned long>(atoi(optarg));
                break;
            default:
                show_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind > argc) {
        std::cerr << "Expected argument after options" << std::endl;
        exit(EXIT_FAILURE);
    }

    unsigned long free_space = disk_size / block_size;

    // Scan in the directory and file lists
    char path [256];
    int check;
    FILE * dirs;
    FileTree *G = nullptr, * current = nullptr;

    dirs = fopen(dir_list, "r");
    if (dirs == nullptr) perror("Error: dir_list.txt");
    else {
        while ((check = fscanf(dirs, "%s", path)) != -1) {
            if (strcmp(path, "./") != 0) {
                G->set_values(path, nullptr, 0, 0, nullptr);
                G = current;
            }
            else {
                //
            }
        }
    }

    char size [256], month [256], day[256], time[256], name [256];
    FILE * files;

    files = fopen(file_list, "r");
    if (files == nullptr) perror("Error: file_list.txt");
    else {
        while ((check = fscanf(files, "%*s %*s %*s %*s %*s %*s %s %s %s %s %s", size, month, day, time, name)) != -1) {

        }
        fclose(files);
    }


//    FileTree root ("./", nullptr);
    // cd no argument = ..
    // dir print out file tree
    // ls list current directory contents
    // mkdir make a directory
    // create takes a name makes a file
    // append filename bytes
    // remove filename bytes
    // delete file or directory
    // exit
    exit(EXIT_SUCCESS);
}
