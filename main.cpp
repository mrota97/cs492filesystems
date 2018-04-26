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
    unsigned long disk_size = 0, block_size = 0;
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

    FileTree G = FileTree("./", disk_size, block_size);
    std::cout << *G.cwd() << std::endl;

    G.mkdir("dir1");
    G.mkdir("dir2");
    G.mkdir("dir3");

    G.print_dir();

    int check = G.cd("dir2");
    std::cout << ((check==0) ? "success" : "failure") << std::endl;
    std::cout << *G.cwd() << std::endl;

    G.create("file1.txt");
    G.create("file2.cpp");
    G.create("file3.h");

    G.print_dir();
    fflush(stdout);
    G.print_disk();
    fflush(stdout);

    G.append("file1.txt", 10);

    G.print_disk();
    fflush(stdout);
    G.print_file_info("file1.txt");
    fflush(stdout);

    G.append("file1.txt", 10);

    G.print_disk();
    fflush(stdout);
    G.print_file_info("file1.txt");
    fflush(stdout);

    G.footprint();

    bool running = true;
    while(running){
      std::string s; // input from user goes here
      std::string delimiter = " ";
      std::string token;
      std::string command;
      std::vector<std::string> args;
      size_t pos = 0;
      bool isCommand = true;

      std::cin >> s;

      while((pos = s.find(delimiter)) != std::string::npos){
        token = s.substr(0,pos);
        // do something with token
        if(isCommand){
          command = token;
          isCommand = false;
        } else{
          args.push_back(token);
        }
        s.erase(0, pos + delimiter.length());
      }
      //do something with the last argument
      args.push_back(s);

      unsigned long bytes;
      if(command == "exit"){
        running = false;
      } else if(command == "cd"){
        G.cd(args[0]);
      } else if(command == "cd .."){
        G.cd("..");
      } else if(command == "ls"){
        G.print_dir();
      } else if(command == "mkdir"){
        G.mkdir(args[0]);
      } else if(command == "create"){
        G.create(args[0]);
      } else if(command == "append"){
        // conversion of string at args[1] to unsigned long bytes
          bytes = strtoul(args[1].c_str(), nullptr, 0);
        G.append(args[0], bytes);
      } else if(command == "remove"){
        // conversion of string at args[1] to unsigned long bytes
          bytes = strtoul(args[1].c_str(), nullptr, 0);
        G.shorten(args[0], bytes);
      } else if(command == "delete"){
        G.remove(args[0]);
      } else if(command == "dir"){

      } else if(command == "prfiles"){
        G.print_disk();
      } else if(command == "prdisk"){

      } else{
          std::cerr << command << std::endl;
      }


    }
    // Scan in the directory and file lists
//    char path [256];
//    int check;
//    FILE * dirs;
//    FileTree G;
//
//    dirs = fopen(dir_list, "r");
//    if (dirs == nullptr) perror("Error: dir_list.txt");
//    else {
//        while ((check = fscanf(dirs, "%s", path)) != -1) {
//            std::string s((const char*)path);
//            if (strcmp(path, "./") == 0)
//                G = FileTree(s);
//            else
//                G.mkdir(s);
//        }
//    }

//
//    char size [256], month [256], day[256], time[256], name [256];
//    FILE * files;
//
//    files = fopen(file_list, "r");
//    if (files == nullptr) perror("Error: file_list.txt");
//    else {
//        while ((check = fscanf(files, "%*s %*s %*s %*s %*s %*s %s %s %s %s %s", size, month, day, time, name)) != -1) {
//
//        }
//        fclose(files);
//    }


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
