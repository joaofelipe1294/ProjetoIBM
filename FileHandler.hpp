//
//  FileHandler.hpp
//  ProjetoIBM
//
//  Created by joao lopes on 22/05/16.
//
//


#ifndef FileHandler_hpp
#define FileHandler_hpp

#include <iostream>
#include <stdio.h>

using namespace std;

class FileHandler{
private:
    string dirName;
    int numberOfFiles;
    string* files;
public:
    FileHandler(string dirName);
    string* GetFiles();
    int GetNumberOfFiles();
protected:
    void SetNumberofFiles();
    void SetFiles();
};

#endif /* FileHandler_hpp */
