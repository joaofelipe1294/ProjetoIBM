//
//  OptimalThreshold.cpp
//  ProjetoIBM
//
//  Created by joao lopes on 22/05/16.
//
//

#include <iostream>
#include "OptimalThreshold.hpp"
#include "itkImageRegionConstIterator.h"
#include <itkThresholdImageFilter.h>
#include "itkImageFileWriter.h"

using namespace std;

typedef ThresholdImageFilter<GrayscaleImageType> ThresholdFilter;
typedef ImageFileWriter<GrayscaleImageType> WriterType;

Optimalthreshold::Optimalthreshold(GrayscaleImageType::Pointer thresholdImage){
    this -> originalImage = thresholdImage;
    this -> thresholdImage = GrayscaleImageType::New();
}

void Optimalthreshold::ApplyThreshold(int value , GrayscaleImageType::Pointer thresholdImage , GrayscaleImageType::Pointer outputImage){
    ThresholdFilter::Pointer thresholdFilter = ThresholdFilter::New();
    thresholdFilter -> SetInput(thresholdImage);
    thresholdFilter -> SetOutsideValue(255);
    thresholdFilter -> SetUpper(value); // valor aleatorio
    thresholdFilter -> SetLower(0);
    outputImage = thresholdFilter -> GetOutput();
    thresholdFilter -> Update();
    //WriterType::Pointer writer = WriterType::New();
    //writer -> SetFileName("saida.tif");
    //writer -> SetInput(outputImage);
    //writer -> Update();
}

int Optimalthreshold::GetOutput(){
    int somaMaiores = 0 , somaMenores = 0 , contadorMaiores = 0 , contadorMenores = 0 , mediaMaiores = 0 , mediaMenores = 0;
    int limiarAtual = 185 , novoLimiar = 0;
    ApplyThreshold(185, this -> originalImage , this -> thresholdImage);
    this -> originalImage -> Update();
    GrayscaleImageType::RegionType region;
    GrayscaleImageType::RegionType regionIterator = this -> originalImage -> GetLargestPossibleRegion();
    ImageRegionConstIterator<GrayscaleImageType> imageIterator(this -> originalImage , regionIterator);
    while(true){
        imageIterator.GoToBegin();
            while(!imageIterator.IsAtEnd()){
                unsigned char val = imageIterator.Get();
                int intValue = (int)val;
                //cout << "pk : " << intValue << endl;
                if(val > limiarAtual){
                    somaMaiores += intValue;
                    contadorMaiores++;
                }else{
                    somaMenores += intValue;
                    contadorMenores++;
                }
                ++imageIterator;
            }
        if(contadorMenores == 0){
            mediaMenores = 0;
        }else{
            mediaMenores = somaMenores / contadorMenores;
        }
        if(contadorMaiores == 0){
            mediaMaiores = 0;
        }else{
            mediaMaiores = somaMaiores / contadorMaiores;
        }
        //cout << "Soma maiores : " << somaMaiores << " | contador : " << contadorMaiores << endl;
        //cout << "Soma menores : " << somaMenores  << " | contador : " << contadorMenores <<  endl;
        //cout << "Media maiores : " << mediaMaiores << endl;
        //cout << "Media menores : " << mediaMenores << endl;
        novoLimiar = (mediaMaiores + mediaMenores) / 2;
        if(novoLimiar == limiarAtual){
            if(limiarAtual > 125){
                //cout << "BREAK limiar : " << limiarAtual << " valor /2 : " << limiarAtual / 2 <<  endl;
                return limiarAtual / 2;
            }else{
                //cout << "BREAK limiar : " << limiarAtual <<  endl;
                return limiarAtual;
            }
        }else{
            contadorMaiores = 0;
            contadorMenores = 0;
            somaMaiores = 0;
            somaMenores = 0;
            limiarAtual = novoLimiar;
            novoLimiar = NULL;
            ApplyThreshold(limiarAtual, this -> originalImage, this -> thresholdImage);
            //cout << "CONTINUE limiar : " << limiarAtual << endl;
        }
        //cout << "Novo limiar : " << limiarAtual << endl;
    }
    return novoLimiar;
}

/*
 
 size[0] = 257;
 size[1] = 257;
 region.SetSize(size);
 thresholdImage->SetRegions(region);
 thresholdImage->Allocate();
 WriterType::Pointer writer = WriterType::New();
 writer -> SetFileName("saida_th.tif");
 writer -> SetInput(this -> thresholdImage);
 writer -> Update();
 
 
 
 GrayscaleImageType::RegionType region = thresholdImage -> GetLargestPossibleRegion();
 ImageRegionConstIterator<GrayscaleImageType> imageIterator(thresholdImage , region);
 int somaMaiores = 0 , somaMenores = 0 , contadorMaiores = 0 , contadorMenores = 0 , mediaMaiores = 0 , mediaMenores = 0;
 int limiarAtual = 185 , novoLimiar = 0;
 while(true){
 imageIterator.GoToBegin();
 while(!imageIterator.IsAtEnd()){
 unsigned char val = imageIterator.Get();
 int intValue = (int)val;
 if(val > limiarAtual){
 somaMaiores += intValue;
 contadorMaiores++;
 }else{
 somaMenores += intValue;
 contadorMenores++;
 }
 ++imageIterator;
 }
 if(contadorMenores == 0){
 mediaMenores = 0;
 }else{
 mediaMenores = somaMenores / contadorMenores;
 }
 if(contadorMaiores == 0){
 mediaMaiores = 0;
 }else{
 mediaMaiores = somaMaiores / contadorMaiores;
 }
 //cout << "Soma maiores : " << somaMaiores << " | contador : " << contadorMaiores << endl;
 //cout << "Soma menores : " << somaMenores  << " | contador : " << contadorMenores <<  endl;
 //cout << "Media maiores : " << mediaMaiores << endl;
 //cout << "Media menores : " << mediaMenores << endl;
 novoLimiar = (mediaMaiores + mediaMenores) / 2;
 if(novoLimiar == limiarAtual){
 thresholdFilter -> SetUpper(limiarAtual);
 thresholdFilter -> SetInput(thresholdImage);
 thresholdFilter -> Update();
 thresholdImage = thresholdFilter -> GetOutput();
 ThresholdFilter::Pointer thresholdFilter = ThresholdFilter::New();
 thresholdFilter -> SetInput(grayscaleFilter -> GetOutput());
 thresholdFilter -> SetOutsideValue(255);
 if(limiarAtual > 125){
 thresholdFilter -> SetUpper(limiarAtual / 2);
 //cout << "BREAK limiar : " << limiarAtual << " valor /2 : " << limiarAtual / 2 <<  endl;
 }else{
 thresholdFilter -> SetUpper(limiarAtual);
 //cout << "BREAK limiar : " << limiarAtual / 2 <<  endl;
 }
 thresholdFilter -> SetLower(0);
 thresholdFilter -> Update();
 thresholdImage = thresholdFilter -> GetOutput();
 thresholdImage -> Update();
 break;
 }else{
 contadorMaiores = 0;
 contadorMenores = 0;
 somaMaiores = 0;
 somaMenores = 0;
 limiarAtual = novoLimiar;
 novoLimiar = NULL;
 ThresholdFilter::Pointer thresholdFilter = ThresholdFilter::New();
 thresholdFilter -> SetInput(grayscaleFilter -> GetOutput());
 thresholdFilter -> SetOutsideValue(255);
 thresholdFilter -> SetUpper(limiarAtual);
 thresholdFilter -> SetLower(0);
 thresholdFilter -> Update();
 thresholdImage = thresholdFilter -> GetOutput();
 thresholdImage -> Update();
 thresholdFilter -> SetUpper(limiarAtual);
 thresholdFilter -> SetInput(thresholdImage);
 thresholdFilter -> Update();
 thresholdImage = thresholdFilter -> GetOutput();
 //cout << "CONTINUE limiar : " << limiarAtual << endl;
 }
 //cout << "Novo limiar : " << limiarAtual << endl;
 }
 
 */