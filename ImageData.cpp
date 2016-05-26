//
//  ImageData.cpp
//  ProjetoIBM
//
//  Created by joao lopes on 23/05/16.
//
//

#include "ImageData.hpp"

void ImageData::SetPxCount(int pxCount){
    this -> pxCount = pxCount;
}

void ImageData::SetCellAvrage(int cellAvrage){
    this -> cellAvrage = cellAvrage;
}

void ImageData::SetThresholdValue(int thresholdValue){
    this -> thresholdValue = thresholdValue;
}

void ImageData::SetLabel(int label){
    this -> label = label;
}

void ImageData::SetDistance(int distance){
    this -> distance = distance;
}

int ImageData::GetPxCount(){
    return this -> pxCount;
}

int ImageData::GetCellAvrage(){
    return this -> cellAvrage;
}

int ImageData::GetThresholdValue(){
    return this -> thresholdValue;
}

int ImageData::GetLabel(){
    return this -> label;
}

int ImageData::GetDistance(){
    return this -> distance;
}