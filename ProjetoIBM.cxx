#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include <itkThresholdImageFilter.h>
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryErodeImageFilter.h"
#include <iostream>
#include <sstream>

#include "RGBToGrayscaleConverter.hpp"

#include <itkRGBToLuminanceImageFilter.h>


using namespace std;
using namespace itk;

typedef Image<unsigned char , 2> ImageType;
//typedef ImageFileReader<ImageType> ReaderType;
typedef ThresholdImageFilter<ImageType> ThresholdFilter;
typedef ImageFileWriter<ImageType> WriterType;
typedef itk::BinaryBallStructuringElement<ImageType::PixelType,2> StructuringElementType;
typedef itk::BinaryDilateImageFilter <ImageType, ImageType, StructuringElementType> BinaryDilateImageFilterType;
typedef itk::BinaryErodeImageFilter <ImageType, ImageType, StructuringElementType> BinaryErodeImageFilterType;

/* funcoes */

int descobreLabel(string nomeImagem){
    int posicaoUnderline;
    posicaoUnderline = nomeImagem.find("_");
    if(nomeImagem[posicaoUnderline + 1] == 49){ //49 equivale a 1 na tabela ASCII
        return 1;
    }else{
        return 0;
    }
}

//Im001_1.tif

int main(int argc, char * argv[]){
    RGBToGrayscaleConverter* rgbConerter = new RGBToGrayscaleConverter("Im001_1.tif");
    rgbConerter -> GetOutput();
    
    /*
    string imageName;
    if(argc < 2){
        cout << "Arquivo : ";
        cin >> imageName;
    }else{
        imageName = argv[1];
    }
    int label = descobreLabel(imageName);
    cout << label << endl;
    ReaderType::Pointer reader = ReaderType::New();
    reader -> SetFileName(imageName);
    //ThresholdFilter::Pointer thresholdFilter = ThresholdFilter::New();
    //thresholdFilter -> SetInput(reader -> GetOutput());
    //thresholdFilter -> SetOutsideValue(255);
    //thresholdFilter -> SetUpper(34);
    //thresholdFilter -> SetLower(0);
    
    //StructuringElementType structuringElement;
    //structuringElement.SetRadius(12);
    //structuringElement.CreateStructuringElement();
    
    //BinaryDilateImageFilterType::Pointer dilateFilter = BinaryDilateImageFilterType::New();
    //dilateFilter->SetInput(thresholdFilter->GetOutput());
    //dilateFilter->SetKernel(structuringElement);
    
    
    
    //BinaryErodeImageFilterType::Pointer erodeFilter = BinaryErodeImageFilterType::New();
    //erodeFilter->SetInput(thresholdFilter->GetOutput());
    //erodeFilter->SetKernel(structuringElement);
    
    
    
    WriterType::Pointer writer = WriterType::New();
    writer -> SetFileName("saida_gray.tif");
    //writer -> SetInput(dilateFilter -> GetOutput());
    //writer -> SetInput(thresholdFilter -> GetOutput());
    //writer -> SetInput(erodeFilter -> GetOutput());
    writer -> SetInput(reader -> GetOutput());
    writer -> Update();*/
    
    return EXIT_SUCCESS;
}
