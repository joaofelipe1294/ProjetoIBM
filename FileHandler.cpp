//
//  FileHandler.cpp
//  ProjetoIBM
//
//  Created by joao lopes on 22/05/16.
//
//

#include "FileHandler.hpp"
#include <iostream>
#include <dirent.h>
#include <cstdlib>
#include <string>
#include <sstream>

FileHandler::FileHandler(string dirName){
    this -> dirName = dirName;
    this -> SetNumberofFiles();
    this -> files = new string[this -> numberOfFiles];
    this -> SetFiles();
}

void FileHandler::SetNumberofFiles(){
    DIR *dir = 0;
    struct dirent *input = 0;
    unsigned char isFile = 0x8;
    dir = opendir (this -> dirName.c_str());
    if (dir == 0) {
        std::cerr << "Nao foi possivel abrir diretorio." << std::endl;
        exit (1);
    }
    while (input = readdir (dir)){
        if (input -> d_type == isFile){
            if(input -> d_name[0] != '.' && input -> d_name[1] != '.'){
                this -> numberOfFiles++;
            }
        }
    }
    closedir (dir);
}

void FileHandler::SetFiles(){
    DIR *dir = 0;
    struct dirent *input = 0;
    unsigned char isFile = 0x8;
    dir = opendir (dirName.c_str());
    if (dir == 0) {
        std::cerr << "Nao foi possivel abrir diretorio." << std::endl;
        exit (1);
    }
    int iterator = 0;
    while (input = readdir (dir)){
        if (input -> d_type == isFile){
            if(input -> d_name[0] != '.' && input -> d_name[1] != '.'){
                stringstream fileName;
                fileName << dirName << input -> d_name;
                this -> files[iterator] = fileName.str();
                iterator++;
            }
        }
    }
    closedir (dir);
}

string* FileHandler::GetFiles(){
    return this -> files;
}

int FileHandler::GetNumberOfFiles(){
    return this -> numberOfFiles;
}