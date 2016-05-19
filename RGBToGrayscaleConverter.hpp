//
//  RGBToGrayscaleConverter.hpp
//  ProjetoIBM
//
//  Created by joao lopes on 19/05/16.
//
//

#ifndef RGBToGrayscaleConverter_hpp
#define RGBToGrayscaleConverter_hpp


#include <stdio.h>
#include <iostream>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"


using namespace std;
using namespace itk;

typedef RGBPixel<unsigned char> RGBPixelType;
typedef Image<RGBPixelType , 2> RGBImageType;
typedef ImageFileReader<RGBImageType> ReaderType;
typedef Image<unsigned char> GrayscaleImageType;
typedef ImageFileWriter<GrayscaleImageType> WriterType;

typedef Image<unsigned char , 2> ImageType;


class RGBToGrayscaleConverter{
private:
    string rgbImageName;
    
public:
    RGBToGrayscaleConverter(string rgbImageName);
    GrayscaleImageType* GetOutput();
};

#endif /* RGBToGrayscaleConverter_hpp */
