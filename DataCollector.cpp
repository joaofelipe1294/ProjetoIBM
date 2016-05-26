//
//  DataCollector.cpp
//  ProjetoIBM
//
//  Created by joao lopes on 23/05/16.
//
//

#include "DataCollector.hpp"
#include "itkImageRegionConstIterator.h"
#include <iostream>

using namespace std;

DataCollector::DataCollector(GrayscaleImageType::Pointer image){
    this -> pxCount = 0;
    this -> cellAvrage = 0;
    this -> image = image;
    GetData();
}

void DataCollector::GetData(){
    this -> image -> Update();
    GrayscaleImageType::RegionType region = this -> image -> GetLargestPossibleRegion();
    ImageRegionConstIterator<GrayscaleImageType> imageIterator(this -> image , region);
    imageIterator.GoToBegin();
    while(!imageIterator.IsAtEnd()){
        unsigned char val = imageIterator.Get();
        int intValue = (int)val;
        if(intValue < 255){
            this -> pxCount += 1;
            this -> cellAvrage += intValue;
        }
        //cout << "pk : " << intValue << endl;
        ++imageIterator;
    }
    if(this -> pxCount > 0){
        this -> cellAvrage /= this -> pxCount;
    }
    //cout << "Media : " << this -> cellAvrage << endl;
    //cout << "Px count : " << this -> pxCount << endl;
}

int DataCollector::GetPxCount(){
    return  this -> pxCount;
}

int DataCollector::GetCellAvrage(){
    if(this -> cellAvrage == 0){
        return 180;
    }else{
        return this -> cellAvrage;
    }
    
}