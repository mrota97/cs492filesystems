#include <iostream>
#include <getopt.h>

static void show_usage(char* progname) {
    std::cerr << "Usage: " << progname <<
              "[-f input files storing information on files]" <<
              "[-d input files storing information on directories]" <<
              "[-s disk size]" <<
              "[-b block size]" << std::endl;
}

int main(int argc, char* argv[]) {
    int flags, opt;
    char *file_list, *dir_list;

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

    exit(EXIT_SUCCESS);
}