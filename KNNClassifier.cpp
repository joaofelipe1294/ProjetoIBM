//
//  KNNClassifier.cpp
//  ProjetoIBM
//
//  Created by joao lopes on 23/05/16.
//
//

#include "KNNClassifier.hpp"
#include "ImageData.hpp"
#include <stdio.h>
#include <string>
#include <fstream>

using namespace std;

KNNClassifier::KNNClassifier(string trainFileName , int k , ImageData compareObject){
    this -> trainFileName = trainFileName;
    this -> kNearest = k;
    this -> nearest = new ImageData[this -> kNearest];
    this -> compareObject = compareObject;
    GetData();
    CompareObjects();
    
}

void KNNClassifier::GetData(){
    //this -> filePointer = fopen("results.txt", "r");
    this -> filePointer = fopen(this -> trainFileName.c_str(), "r");
    int thresholdValue , pxCount , cellAvrg , label , linesNumber , aberration;
    fscanf(this -> filePointer, "%d" , &linesNumber);
    this -> objectsNumber = linesNumber;
    this -> dataObjects = new ImageData[this -> objectsNumber];
    int line = 0;
    while (feof(this -> filePointer) == false) {
        //fscanf(this -> filePointer, "%d , %d , %d , %d" , &thresholdValue , &pxCount , &cellAvrg , &label);
        fscanf(this -> filePointer, "%d , %d , %d , %d" , &thresholdValue , &cellAvrg , &aberration , &label);
        ImageData imageData;
        imageData.SetThresholdValue(thresholdValue);
        imageData.SetPxCount(pxCount);
        imageData.SetCellAvrage(cellAvrg);
        imageData.SetAberration(aberration);
        imageData.SetLabel(label);
        this -> dataObjects[line] = imageData;
        //cout << "limiar : " << thresholdValue << " | contagem : " << pxCount << " | cellAvrg : " << cellAvrg << endl;
        line++;
    }
    fclose(this -> filePointer);
}

void KNNClassifier::CompareObjects(){
    for(int cont = 0 ; cont < this -> objectsNumber ; cont++){
        //int pxCountDiference = this -> compareObject.GetPxCount() - this -> dataObjects[cont].GetPxCount();
        int thresholdDiference = this -> compareObject.GetThresholdValue() - this -> dataObjects[cont].GetThresholdValue();
        int cellAvrageDiference = this -> compareObject.GetCellAvrage() - this -> dataObjects[cont].GetCellAvrage();
        int aberrationDistance = this -> compareObject.GetAberration() - this -> dataObjects[cont].GetAberration();
        //int distance = abs(pxCountDiference) + abs(thresholdDiference) + abs(cellAvrageDiference);
        int distance = abs(thresholdDiference) + abs(cellAvrageDiference) + abs(aberrationDistance);
        if(cont < this -> kNearest){
            //cout << "ADICIONADO - " << cont << " - distance : " << distance << endl;
            this -> dataObjects[cont].SetDistance(distance);
            this -> nearest[cont] = this -> dataObjects[cont];
        }else{
            int biggerPosition = -1;
            int biggerDistance = -1;
            for(int i = 0 ; i < this -> kNearest ; i++){
                if(this -> nearest[i].GetDistance() > distance){
                    biggerDistance = this -> nearest[i].GetDistance();
                    biggerPosition = i;
                }
            }
            if(biggerPosition != -1 && biggerDistance != -1){
                //cout << "TROCADO - " << cont << " - distance : " << distance << endl;
                this -> dataObjects[cont].SetDistance(distance);
                this -> nearest[biggerPosition] = this -> dataObjects[cont];
            }
        }
    }
}

int KNNClassifier::FindLabel(){
    int num0 = 0 , num1 = 0;
    for(int i = 0 ; i < this -> kNearest ; i++){
        if(this -> nearest[i].GetLabel() == 0){
            num0 += 1;
        }else{
            num1 += 1;
        }
    }
    if(num1 > num0){
        return 1;
    }else{
        return 0;
    }
}













