#include <iostream>
#include <getopt.h>
#include <fstream>
#include <fcntl.h>
#include <cstring>
#include <ctime>
#include "filetree.h"

static void show_usage(char* progname) {
    std::cerr << "Usage: " << progname <<
              "[-f input files storing information on files]" <<
              "[-d input files storing information on directories]" <<
              "[-s disk size]" <<
              "[-b block size]" << std::endl;
}

void generate_timestamp(tm * stamp, std::string time, std::string day, std::string month) {
    // Set the month correctly
    if (month == "Jan")
        stamp->tm_mon = 0;
    else if (month == "Feb")
        stamp->tm_mon = 1;
    else if (month == "Mar")
        stamp->tm_mon = 2;
    else if (month == "Apr")
        stamp->tm_mon = 3;
    else if (month == "May")
        stamp->tm_mon = 4;
    else if (month == "Jun")
        stamp->tm_mon = 5;
    else if (month == "Jul")
        stamp->tm_mon = 6;
    else if (month == "Aug")
        stamp->tm_mon = 7;
    else if (month == "Sep")
        stamp->tm_mon = 8;
    else if (month == "Oct")
        stamp->tm_mon = 9;
    else if (month == "Nov")
        stamp->tm_mon = 10;
    else if (month == "Dec")
        stamp->tm_mon = 11;

    // Day
    stamp->tm_mday = std::stoi(day);

    // Extract hour and minute
    size_t pos = time.find(':');
    stamp->tm_hour = std::stoi(time.substr(0, pos));
    stamp->tm_min = std::stoi(time.substr(pos+1, std::string::npos));

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
                block_size = strtoul(optarg, nullptr, 0);
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

    FileTree G;

    std::string line;
    std::vector<std::string> paths;
    bool init = true;
    std::ifstream dirs (dir_list);
    if (dirs.is_open()) {
        while(std::getline(dirs, line)) {
            paths = G.path_to_vector(line, "/");
            while (!paths.empty()) {
                if (paths.size() == 1) {
                    if (init) {
                        G = FileTree(paths[0], disk_size, block_size);
                        init = false;
                    } else
                        G.mkdir(paths[0]);
                    paths.clear();
                } else {
                    G.cd(paths[0]);
                    paths.erase(paths.begin());
                }
            }
            G.cd();
        }
    }

//    G.print_disk();
//
//    G.create("file.txt");
//
//    G.print_dir();
//
//    G.append("file.txt", 2001);
//
//    G.print_disk();
//
//    G.append("file.txt", 47);
//
//    G.print_disk();
//
//    G.append("file1.txt", 1);
//
//    G.print_disk();
//
//    G.append("file1.txt", 509438);
//
//    G.print_disk();

    std::string one, two, three, four, five, six;
    std::string size, month, day, time, path;
    std::ifstream files (file_list);
    if (files.is_open()) {
        while (files >> one >> two >> three >> four >> five >> six >> size >> month >> day >> time >> path) {
            paths = G.path_to_vector(path, "/");
            while (!paths.empty()) {
                if (paths.size() == 1) {
                    G.create(paths[0]);
                    struct tm stamp = {0};
                    generate_timestamp(&stamp, time, day, month);
                    time_t timestamp = mktime(&stamp);
                    std::cout << ctime(&timestamp) << std::endl;

                    G.append(paths[0], std::stoul(size));
                    paths.clear();
                } else {
                    G.cd(paths[0]);
                    paths.erase(paths.begin());
                }
            }
            G.cd();
        }
    }

    while(true){
      std::string s; // input from user goes here
      std::string delimiter = " ";
      std::string token;
      std::vector<std::string> args;
      size_t pos = 0;

      std::cout << G.cwd() << " >> ";
      std::getline(std::cin, s);

      while((pos = s.find(delimiter)) != std::string::npos){
        token = s.substr(0,pos);
        args.push_back(token);
        s.erase(0, pos + delimiter.length());
      }
      //do something with the last argument
      args.push_back(s);

      unsigned long bytes;
      std::string command = args[0];
      if (command == "exit"){
          break;
      } else if(command == "cd") {
          if (args.size() == 1) {
              G.cd();
          } else {
              G.cd(args[1]);
          }
      } else if(command == "ls") {
          G.print_dir();
      } else if(command == "mkdir") {
          G.mkdir(args[1]);
      } else if(command == "create") {
          G.create(args[1]);
      } else if(command == "append") {
        // conversion of string at args[2] to unsigned long bytes
          bytes = strtoul(args[2].c_str(), nullptr, 0);
          G.append(args[1], bytes);
      } else if(command == "remove") {
        // conversion of string at args[2] to unsigned long bytes
          bytes = strtoul(args[2].c_str(), nullptr, 0);
          G.shorten(args[1], bytes);
      } else if(command == "delete") {
          G.remove(args[1]);
      } else if(command == "dir") {

      } else if(command == "prfiles") {
          G.prfiles(G.get_root());
      } else if(command == "prdisk") {
          G.print_disk();
      } else{
          std::cerr << command << std::endl;
      }
    }
    exit(EXIT_SUCCESS);
}
