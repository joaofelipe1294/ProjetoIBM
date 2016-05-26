//
//  KNNClassifier.hpp
//  ProjetoIBM
//
//  Created by joao lopes on 23/05/16.
//
//

#ifndef KNNClassifier_hpp
#define KNNClassifier_hpp

#include <stdio.h>
#include "ImageData.hpp"
#include <iostream>

using namespace std;

class KNNClassifier{
private:
    string trainFileName;
    FILE* filePointer;
    int objectsNumber;
    int kNearest;
    ImageData* dataObjects;
    ImageData* nearest;
    ImageData compareObject;
    void GetData();
    void CompareObjects();
public:
    KNNClassifier(string trainFileName , int k , ImageData compareObject);
    int FindLabel();
};

#endif /* KNNClassifier_hpp */
