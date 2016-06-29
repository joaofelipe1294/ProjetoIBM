//
//  ProgressBar.hpp
//  ProjetoIBM
//
//  Created by joao lopes on 28/06/16.
//
//

#ifndef ProgressBar_hpp
#define ProgressBar_hpp

#include <stdio.h>
#include <iostream>
using namespace std;

class ProgressBar {
public:
    void update(double newProgress);
    void print();
private:
    std::string firstPartOfpBar = "[", //Change these at will (that is why I made them public)
    lastPartOfpBar = "]",
    pBarFiller = "█",
    pBarUpdater = "/-\\|";
    int amountOfFiller,
    pBarLength = 50, //I would recommend NOT changing this
    currUpdateVal = 0; //Do not change
    double currentProgress = 0, //Do not change
    neededProgress = 100; //I would recommend NOT changing this
};


#endif /* ProgressBar_hpp */
