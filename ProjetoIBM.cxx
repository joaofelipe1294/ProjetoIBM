#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBToLuminanceImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include <itkThresholdImageFilter.h>
#include "itkNeighborhoodConnectedImageFilter.h"
#include <itkOrImageFilter.h>
#include <itkBinaryDilateImageFilter.h>
#include "itkBinaryBallStructuringElement.h"
#include <iostream>
#include <dirent.h>
#include <cstdlib>
#include <string>

#include "FileHandler.hpp"
//#include "itkDiscreteGaussianImageFilter.h"
//#include "itkCastImageFilter.h"
//#include "itkRescaleIntensityImageFilter.h"
//#include "itkBinaryDilateImageFilter.h"

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
typedef NeighborhoodConnectedImageFilter<GrayscaleImageType, GrayscaleImageType> NeighborhoodConnectedFilterType;
typedef OrImageFilter<GrayscaleImageType> OrFilterType;
typedef ImageFileWriter<GrayscaleImageType> WriterType;
typedef BinaryBallStructuringElement<GrayscaleImageType::PixelType , 2> StructuringElementType;
typedef itk::BinaryDilateImageFilter <GrayscaleImageType, GrayscaleImageType, StructuringElementType> BinaryDilateImageFilterType;
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

int main(int argc, char * argv[]){
    string imageName , dirName = "img/";
    FileHandler* fileHandler = new FileHandler(dirName);
    //int numFiles = countFiles(dirName);
    //string* files = new string[numFiles];
    
    /*if(argc == 2){
        imageName = argv[1];
    }else if(argc == 3){*/
        //listaArquivos(dirName , files);
        //cout << numFiles << endl;
    /*}else{
        cout << "Nome da imagem : ";
        cin >> imageName;
    }*/
    for (int cont = 0; cont < fileHandler -> GetNumberOfFiles(); cont++) {
        /* converte imagem */
        RGBReaderType::Pointer rgbReader = RGBReaderType::New();
        rgbReader -> SetFileName(fileHandler -> GetFiles()[cont]); // nome da imagem que sera processada !
        //rgbReader -> SetFileName("img/Im022_1.tif"); // nome da imagem que sera processada !
        GrayscaleFilterType::Pointer grayscaleFilter = GrayscaleFilterType::New();
        grayscaleFilter -> SetInput(rgbReader -> GetOutput());
        GrayscaleImageType::Pointer grayScaleImage = grayscaleFilter -> GetOutput();
        grayscaleFilter -> Update();
        
        
        /* region growing */
        NeighborhoodConnectedFilterType::Pointer regionGrow = NeighborhoodConnectedFilterType::New();
        float lower = 0;
        float upper = 97;
        regionGrow->SetLower(lower);
        regionGrow->SetUpper(upper);
        regionGrow->SetReplaceValue(255);
        regionGrow->SetInput(grayscaleFilter -> GetOutput());
        
        for (int x = 5; x < 250; x += 5) {
            GrayscaleImageType::IndexType seed1;
            seed1[0] = x;
            seed1[1] = 0;
            regionGrow->AddSeed(seed1);
            GrayscaleImageType::IndexType seed2;
            seed2[0] = x;
            seed2[1] = 255;
            regionGrow->AddSeed(seed2);
        }
        for (int y = 0; y < 250; y += 5) {
            GrayscaleImageType::IndexType seed1;
            seed1[0] = 255;
            seed1[1] = y;
            regionGrow->AddSeed(seed1);
            GrayscaleImageType::IndexType seed2;
            seed2[0] = 0;
            seed2[1] = y;
            regionGrow->AddSeed(seed2);
        }
        
        
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
        
        
        /* orImage filter */
        
        OrFilterType::Pointer orFilter = OrFilterType::New();
        orFilter -> SetInput(0, thresholdImage);
        orFilter -> SetInput(1, regionGrow -> GetOutput());
        orFilter -> Update();
        
        
        /* fechamento */
        StructuringElementType structuringElement;
        structuringElement.SetRadius(4);
        structuringElement.CreateStructuringElement();
        // BinaryDilateImageFilterType::Pointer dilatateFilter = BinaryDilateImageFilterType::New();
        
        
        BinaryDilateImageFilterType::Pointer dilateFilter = BinaryDilateImageFilterType::New();
        dilateFilter->SetInput(orFilter -> GetOutput());
        dilateFilter->SetKernel(structuringElement);
        
         /* gera resultado */
        stringstream imageOut;
        imageOut << "out/" << fileHandler -> GetFiles()[cont].substr(dirName.length() , (fileHandler -> GetFiles()[cont].length() - dirName.length()));
        //imageOut << "out/" << "saida.tif";
        cout  << "imageName : " << imageOut.str() << endl;
        
        WriterType::Pointer writer = WriterType::New();
        writer -> SetFileName(imageOut.str());
        //writer -> SetInput(grayscaleImage);
        //writer -> SetInput(thresholdImage);//(thresholdFilter -> GetOutput());
        //writer -> SetInput(grayscaleFilter -> GetOutput());
        //writer -> SetInput(regionGrow -> GetOutput());
        writer -> SetInput(dilateFilter -> GetOutput());
        writer -> Update();
    }
    
    
    
    return EXIT_SUCCESS;
}