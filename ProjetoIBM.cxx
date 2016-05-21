#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBToLuminanceImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include <itkThresholdImageFilter.h>
#include <iostream>
//#include "itkDiscreteGaussianImageFilter.h"
//#include "itkCastImageFilter.h"
//#include "itkRescaleIntensityImageFilter.h"
//#include "itkBinaryDilateImageFilter.h"
//#include "itkBinaryBallStructuringElement.h"
//#include "itkBinaryErodeImageFilter.h"
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
typedef ThresholdImageFilter<GrayscaleImageType> ThresholdFilter;

//typedef Image<unsigned char , 2> ImageType;
//typedef ImageFileReader<ImageType> ReaderType;

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
    
                           /* converte imagem */
    RGBReaderType::Pointer rgbReader = RGBReaderType::New();
    rgbReader -> SetFileName("Im184_0.tif");
    GrayscaleFilterType::Pointer grayscaleFilter = GrayscaleFilterType::New();
    grayscaleFilter -> SetInput(rgbReader -> GetOutput());
    GrayscaleImageType::Pointer grayScaleImage = grayscaleFilter -> GetOutput();
    grayscaleFilter -> Update();
    
                            /* primeiro threshold */
    ThresholdFilter::Pointer thresholdFilter = ThresholdFilter::New();
    thresholdFilter -> SetInput(grayscaleFilter -> GetOutput());
    thresholdFilter -> SetOutsideValue(255);
    thresholdFilter -> SetUpper(185); // valor aleatorio
    thresholdFilter -> SetLower(0);
    thresholdFilter -> Update();
    GrayscaleImageType::Pointer thresholdImage = GrayscaleImageType::New();
    thresholdImage = thresholdFilter -> GetOutput();
    thresholdImage -> Update();
    
    
                           /* descobre o valor atual do threshold */
    GrayscaleImageType::RegionType region = thresholdImage -> GetLargestPossibleRegion();
    ImageRegionConstIterator<GrayscaleImageType> imageIterator(thresholdImage , region);
    int somaMaiores = 0 , somaMenores = 0 , contadorMaiores = 0 , contadorMenores = 0 , mediaMaiores = 0 , mediaMenores = 0;
    int limiarAtual = 185 , novoLimiar = 0;
    while(true){
        imageIterator.GoToBegin();
        while(!imageIterator.IsAtEnd()){
            unsigned char val = imageIterator.Get();
            int intValue = (int)val;
            //cout << "px : " << intValue << endl;
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
            cout << "BREAK limiar : " << limiarAtual << endl;
            ThresholdFilter::Pointer thresholdFilter = ThresholdFilter::New();
            thresholdFilter -> SetInput(grayscaleFilter -> GetOutput());
            thresholdFilter -> SetOutsideValue(255);
            thresholdFilter -> SetUpper(limiarAtual);
            thresholdFilter -> SetLower(0);
            thresholdFilter -> Update();
            //GrayscaleImageType::Pointer thresholdImage = GrayscaleImageType::New();
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
            //GrayscaleImageType::Pointer thresholdImage = GrayscaleImageType::New();
            thresholdImage = thresholdFilter -> GetOutput();
            thresholdImage -> Update();
            thresholdFilter -> SetUpper(limiarAtual);
            thresholdFilter -> SetInput(thresholdImage);
            thresholdFilter -> Update();
            thresholdImage = thresholdFilter -> GetOutput();
            cout << "CONTINUE limiar : " << limiarAtual << endl;
        }
        cout << "Novo limiar : " << limiarAtual << endl;
    }
    
    
    
    
    WriterType::Pointer writer = WriterType::New();
    writer -> SetFileName("imagem_limiar.tif");
    writer -> SetInput(thresholdImage);//(thresholdFilter -> GetOutput());
    writer -> Update();
    
    
    
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

