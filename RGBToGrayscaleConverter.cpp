//
//  RGBToGrayscaleConverter.cpp
//  ProjetoIBM
//
//  Created by joao lopes on 19/05/16.
//
//

#include "RGBToGrayscaleConverter.hpp"
#include "itkRGBToLuminanceImageFilter.h"

typedef RGBToLuminanceImageFilter<RGBImageType, GrayscaleImageType> GrayscaleFilterType;

RGBToGrayscaleConverter::RGBToGrayscaleConverter(string rgbImageName){
    this -> rgbImageName = rgbImageName;
}

GrayscaleImageType* RGBToGrayscaleConverter::GetOutput(){
    ReaderType::Pointer reader = ReaderType::New();
    reader -> SetFileName(this -> rgbImageName);
    GrayscaleFilterType::Pointer grayscaleFilter = GrayscaleFilterType::New();
    grayscaleFilter -> SetInput(reader -> GetOutput());
    WriterType::Pointer writer = WriterType::New();
    writer -> SetFileName("grayImage.tif");
    writer -> SetInput(grayscaleFilter -> GetOutput());
    writer -> Update();
    GrayscaleImageType::Pointer grayImage = GrayscaleImageType::New();
    grayImage = grayscaleFilter -> GetOutput();
    return grayImage;
}