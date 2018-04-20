#include <iostream>
#include <getopt.h>
#include <fstream>
#include <fcntl.h>
#include <cstring>
#include "filetree.h"

static void show_usage(char* progname) {
    std::cerr << "Usage: " << progname <<
              "[-f input files storing information on files]" <<
              "[-d input files storing information on directories]" <<
              "[-s disk size]" <<
              "[-b block size]" << std::endl;
}

int main(int argc, char* argv[]) {
    unsigned long disk_size;
    int flags = NULL, opt, block_size;
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
                disk_size = strtoul(optarg, NULL, 0);
                break;
            case 'b':
                block_size = atoi(optarg);
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

    // Scan in the directory and file lists
    char path [256];
    int check;
    FILE * dirs;
    FileTree *G = nullptr, * current = nullptr;

    dirs = fopen(dir_list, "r");
    if (dirs == NULL) perror("Error: dir_list.txt");
    else {
        while ((check = fscanf(dirs, "%s", path)) != -1) {
            if (strcmp(path, "./")) {
                G->set_values(path, NULL, 0, 0, NULL);
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
    if (files == NULL) perror("Error: file_list.txt");
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
