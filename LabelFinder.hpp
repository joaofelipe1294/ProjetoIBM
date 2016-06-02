//
//  LabelFinder.hpp
//  ProjetoIBM
//
//  Created by joao lopes on 02/06/16.
//
//

#ifndef LabelFinder_hpp
#define LabelFinder_hpp

#include <stdio.h>
#include <iostream>

using namespace std;

class LabelFinder{
private:
    string fileName;
public:
    LabelFinder(string fileName);
    int GetLabel();
};

#endif /* LabelFinder_hpp */
