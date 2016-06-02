//
//  LabelFinder.cpp
//  ProjetoIBM
//
//  Created by joao lopes on 02/06/16.
//
//

#include "LabelFinder.hpp"

LabelFinder::LabelFinder(string fileName){
    this -> fileName = fileName;
}

int LabelFinder::GetLabel(){
    int underlinePosition;
    underlinePosition = this -> fileName.find("_");
    if(this -> fileName[underlinePosition + 1] == 49){ //49 equivale a 1 na tabela ASCII
        return 1;
    }else{
        return 0;
    }
}
