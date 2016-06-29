//
//  ProgressBar.cpp
//  ProjetoIBM
//
//  Created by joao lopes on 28/06/16.
//
//

#include "ProgressBar.hpp"


void ProgressBar::update(double newProgress) {
        currentProgress += newProgress;
        amountOfFiller = (int)((currentProgress / neededProgress)*(double)pBarLength);
    }

void ProgressBar::print() {
    currUpdateVal %= pBarUpdater.length();
    cout << "\r" //Bring cursor to start of line
    << firstPartOfpBar; //Print out first part of pBar
    for (int a = 0; a < amountOfFiller; a++) { //Print out current progress
        cout << pBarFiller;
    }
    cout << pBarUpdater[currUpdateVal];
    for (int b = 0; b < pBarLength - amountOfFiller; b++) { //Print out spaces
        cout << " ";
    }
    cout << lastPartOfpBar //Print out last part of progress bar
    //<< " (" << (int)(100*(currentProgress/neededProgress)) << "%)" //This just prints out the percent
    << flush;
    currUpdateVal += 1;
}
