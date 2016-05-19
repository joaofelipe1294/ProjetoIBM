#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBToLuminanceImageFilter.h"
#include "itkImageRegionConstIterator.h"
//#include "itkDiscreteGaussianImageFilter.h"
//#include "itkCastImageFilter.h"
//#include "itkRescaleIntensityImageFilter.h"
//#include <itkThresholdImageFilter.h>
//#include "itkBinaryDilateImageFilter.h"
//#include "itkBinaryBallStructuringElement.h"
//#include "itkBinaryErodeImageFilter.h"
#include <iostream>
//#include <sstream>
//#include <itkRGBToLuminanceImageFilter.h>

using namespace std;
using namespace itk;

const int DIMENSIONS = 2;

typedef Image<unsigned char , DIMENSIONS> GrayscaleImageType;
typedef RGBPixel<unsigned char> RGBPixelType;
typedef Image<RGBPixelType , DIMENSIONS> RGBImageType;
typedef ImageFileReader<RGBImageType> RGBReaderType;
typedef RGBToLuminanceImageFilter<RGBImageType, GrayscaleImageType> GrayscaleFilterType;


//typedef Image<unsigned char , 2> ImageType;
//typedef ImageFileReader<ImageType> ReaderType;
//typedef ThresholdImageFilter<ImageType> ThresholdFilter;
typedef ImageFileWriter<GrayscaleImageType> WriterType;
//typedef itk::BinaryBallStructuringElement<ImageType::PixelType,2> StructuringElementType;
//typedef itk::BinaryDilateImageFilter <ImageType, ImageType, StructuringElementType> BinaryDilateImageFilterType;
//typedef itk::BinaryErodeImageFilter <ImageType, ImageType, StructuringElementType> BinaryErodeImageFilterType;

/* functions */

/*int descobreLabel(string nomeImagem){
    int posicaoUnderline;
    posicaoUnderline = nomeImagem.find("_");
    if(nomeImagem[posicaoUnderline + 1] == 49){ //49 equivale a 1 na tabela ASCII
        return 1;
    }else{
        return 0;
    }
}*/








//Im001_1.tif

int main(int argc, char * argv[]){
    RGBReaderType::Pointer rgbReader = RGBReaderType::New();
    rgbReader -> SetFileName("Im001_1.tif");
    GrayscaleFilterType::Pointer grayscaleFilter = GrayscaleFilterType::New();
    grayscaleFilter -> SetInput(rgbReader -> GetOutput());
    grayscaleFilter -> Update();
    GrayscaleImageType::Pointer grayScaleImage = grayscaleFilter -> GetOutput();
    
    
    
    GrayscaleImageType::RegionType region = grayScaleImage -> GetLargestPossibleRegion();
    ImageRegionConstIterator<GrayscaleImageType> imageIterator(grayScaleImage,region);

    while(!imageIterator.IsAtEnd()){
        unsigned char val = imageIterator.Get();
        val = abs(val-255);
        std::cout << (int)val << std::endl;
        ++imageIterator;
    }
    

    
    return EXIT_SUCCESS;
}

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //ImageType::Pointer image = ImageType::New();
    //ReaderType::Pointer reader = ReaderType::New();
    //reader -> SetFileName("Im001_1.tif");
    //GrayscaleFilterType::Pointer grayscaleFilter = GrayscaleFilterType::New();
    //grayscaleFilter -> SetInput(reader -> GetOutput());
    //image = grayscaleFilter -> GetOutput();
    
    
    
    
    
    
    
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

