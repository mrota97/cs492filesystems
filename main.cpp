#include <iostream>
#include <getopt.h>
#include "filetree.h"

static void show_usage(char* progname) {
    std::cerr << "Usage: " << progname <<
              "[-f input files storing information on files]" <<
              "[-d input files storing information on directories]" <<
              "[-s disk size]" <<
              "[-b block size]" << std::endl;
}

int main(int argc, char* argv[]) {
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
                break;
            case 'b':
                break;
            default:
                show_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        std::cerr << "Expected argument after options" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout <<
              "Flags = " << flags <<
              "file_list = " << file_list <<
              "dir_list = " << dir_list <<
              std::endl;

    FileTree root ("./", nullptr);
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
