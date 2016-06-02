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
#include "itkBinaryErodeImageFilter.h"
#include "itkGrayscaleMorphologicalOpeningImageFilter.h"
#include "itkGrayscaleMorphologicalClosingImageFilter.h"
#include <iostream>
#include <dirent.h>
#include <cstdlib>
#include <string>


#include "FileHandler.hpp"
#include "OptimalThreshold.hpp"
#include "DataCollector.hpp"
#include "KNNClassifier.hpp"
#include "LabelFinder.hpp"

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
typedef BinaryDilateImageFilter <GrayscaleImageType, GrayscaleImageType, StructuringElementType> BinaryDilateImageFilterType;
typedef BinaryErodeImageFilter <GrayscaleImageType, GrayscaleImageType, StructuringElementType> BinaryErodeImageFilterType;
typedef GrayscaleMorphologicalClosingImageFilter<GrayscaleImageType, GrayscaleImageType, StructuringElementType> OpeningMorphologyType;



/* functions */

/*
int descobreLabel(string nomeImagem){
    int posicaoUnderline;
    posicaoUnderline = nomeImagem.find("_");
    if(nomeImagem[posicaoUnderline + 1] == 49){ //49 equivale a 1 na tabela ASCII
        return 1;
    }else{
        return 0;
    }
}*/

int main(int argc, char * argv[]){
    string imageName , dirName;// = "img/";
    int k;
    if(argc == 1){
        cout << "Pasta de treinamento : ";
        cin >> dirName;
        cout << "K : ";
        cin >> k;
        cout << "Imagem a ser classificada : ";
        cin >> imageName;
    }else{
        dirName = argv[1];
        k = atoi(argv[2]);
        imageName = argv[3];
    }
    
    int limitValue = 0;
    int label = NULL;
    FILE* resultFile;
    string resultFileName = "results.txt";
    resultFile = fopen(resultFileName.c_str(), "w");
    FileHandler* fileHandler = new FileHandler(dirName);
    fprintf(resultFile, "%d\n" , fileHandler -> GetNumberOfFiles());
    for (int cont = 0; cont < fileHandler -> GetNumberOfFiles(); cont++) {
    
/* --------------------------------------- IMAGE CONVERTING --------------------------------------------- */

        RGBReaderType::Pointer rgbReader = RGBReaderType::New();
        rgbReader -> SetFileName(fileHandler -> GetFiles()[cont]);
        //rgbReader -> SetFileName("img/Im148_0.tif"); // teste uma imagem
        //label = descobreLabel("img/Im148_0.tif");
        LabelFinder* labelFinder = new LabelFinder(fileHandler -> GetFiles()[cont]);
        label = labelFinder -> GetLabel();
        //label = descobreLabel(fileHandler -> GetFiles()[cont]);
        rgbReader -> SetFileName(fileHandler -> GetFiles()[cont]);
        GrayscaleFilterType::Pointer grayscaleFilter = GrayscaleFilterType::New();
        grayscaleFilter -> SetInput(rgbReader -> GetOutput());
        GrayscaleImageType::Pointer grayScaleImage = grayscaleFilter -> GetOutput();
        grayscaleFilter -> Update();
        
/* ------------------------------------------------------------------------------------------------------ */
    
/* ------------------------------------------- FIND THRESHOLD VALUE -------------------------------------- */
        
        Optimalthreshold* optimalThreshold = new Optimalthreshold(grayscaleFilter -> GetOutput());
        limitValue = optimalThreshold -> GetOutput();
        //cout << "Valor Limiar : " << limitValue << endl;
        ThresholdFilter::Pointer thresholdFilter = ThresholdFilter::New();
        thresholdFilter -> SetInput(grayscaleFilter -> GetOutput());
        thresholdFilter -> SetOutsideValue(255);
        thresholdFilter -> SetUpper(optimalThreshold -> GetOutput()); // valor aleatorio
        thresholdFilter -> SetLower(0);
        thresholdFilter -> Update();
        GrayscaleImageType::Pointer thresholdImage = GrayscaleImageType::New();
        thresholdImage = thresholdFilter -> GetOutput();
        thresholdImage -> Update();
    
/* ------------------------------------------------------------------------------------------------------ */
        
/* ---------------------------------------- REGION GROWING ---------------------------------------------- */
        
        NeighborhoodConnectedFilterType::Pointer regionGrow = NeighborhoodConnectedFilterType::New();
        float lower = 0;
        float upper = optimalThreshold -> GetOutput();
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
        
/* ------------------------------------------------------------------------------------------------------ */
    
/* --------------------------------------------- OR IMAGEFILTER ----------------------------------------- */
    
        OrFilterType::Pointer orFilter = OrFilterType::New();
        orFilter -> SetInput(0, thresholdImage);
        orFilter -> SetInput(1, regionGrow -> GetOutput());
        orFilter -> Update();
        
/* ------------------------------------------------------------------------------------------------------- */
    
/* --------------------------------------------- MORFOLOGY ------------------------------------------------ */
        
        StructuringElementType structuringElement;
        structuringElement.SetRadius(4);
        structuringElement.CreateStructuringElement();
        BinaryDilateImageFilterType::Pointer dilateFilter = BinaryDilateImageFilterType::New();
        dilateFilter -> SetInput(orFilter -> GetOutput());
        dilateFilter -> SetKernel(structuringElement);
        dilateFilter -> Update();
    
/* ------------------------------------------------------------------------------------------------------- */

/* --------------------------------------------- DATA COLLECT -------------------------------------------- */
        
        DataCollector* dataCollector = new DataCollector(dilateFilter -> GetOutput());
        
/* ------------------------------------------------------------------------------------------------------- */
        
/* ------------------------------------------ GENERATE RESULTS ------------------------------------------- */
 
        //stringstream imageOut;
        //imageOut << "out/" << fileHandler -> GetFiles()[cont].substr(dirName.length() , (fileHandler -> GetFiles()[cont].length() - dirName.length()));
        //cout  << "imageName : " << imageOut.str() << endl;
        //cout << "==============================================================================" << endl << endl;
        //WriterType::Pointer writer = WriterType::New();
        //writer -> SetFileName(imageOut.str());
        //writer -> SetInput(dilateFilter -> GetOutput());
        //writer -> Update();

/* ------------------------------------------------------------------------------------------------------- */
        if(dataCollector -> GetPxCount() == 0){
            //fprintf(resultFile,"%d , %d , %d , %d\n",limitValue * 2 , dataCollector -> GetPxCount() , dataCollector -> GetCellAvrage() , label);
            fprintf(resultFile,"%d , %d , %d\n",limitValue * 2 , dataCollector -> GetCellAvrage() , label);
        }else{
            //fprintf(resultFile,"%d , %d , %d , %d\n",limitValue , dataCollector -> GetPxCount() / 100 , dataCollector -> GetCellAvrage() , label);
            fprintf(resultFile,"%d , %d , %d\n",limitValue , dataCollector -> GetCellAvrage() , label);
        }
        
        cout << "Processado  " << cont + 1 << " de " << fileHandler -> GetNumberOfFiles() << endl;
    }
    fclose(resultFile);

/* -------------------------------------------------------------------------------------------------------- */
    
    
    cout << "TREINAMENTO CONCLUIDO" << endl;

    
    
    
/* ================================================ CLASSIFY IMAGE ======================================== */
    
    
    
    
    
    /* --------------------------------------- IMAGE CONVERTING --------------------------------------------- */
    
    RGBReaderType::Pointer rgbReader = RGBReaderType::New();
    rgbReader -> SetFileName(imageName); // teste uma imagem
    GrayscaleFilterType::Pointer grayscaleFilter = GrayscaleFilterType::New();
    grayscaleFilter -> SetInput(rgbReader -> GetOutput());
    GrayscaleImageType::Pointer grayScaleImage = grayscaleFilter -> GetOutput();
    grayscaleFilter -> Update();
    
    /* ------------------------------------------------------------------------------------------------------ */
    
    /* ------------------------------------------- OPENING MORPHOLOGY -------------------------------------------- */
    
    StructuringElementType structuringElementOpen;
    structuringElementOpen.SetRadius(5);
    structuringElementOpen.CreateStructuringElement();
    OpeningMorphologyType::Pointer openingFilter = OpeningMorphologyType::New();
    openingFilter -> SetInput(grayscaleFilter -> GetOutput());
    openingFilter -> SetKernel(structuringElementOpen);
    stringstream imageOutOpen;
    imageOutOpen << "open_" << imageName;
    cout  << "imageName : " << imageOutOpen.str() << endl;
    WriterType::Pointer writerOpen = WriterType::New();
    writerOpen -> SetFileName(imageOutOpen.str());
    writerOpen -> SetInput(openingFilter -> GetOutput());
    writerOpen -> Update();
    
    
    /* ------------------------------------------------------------------------------------------------------ */
    
    
    
    /* ------------------------------------------- FIND THRESHOLD VALUE -------------------------------------- */
    
    Optimalthreshold* optimalThreshold = new Optimalthreshold(openingFilter -> GetOutput());
    //Optimalthreshold* optimalThreshold = new Optimalthreshold(grayscaleFilter -> GetOutput());
    limitValue = optimalThreshold -> GetOutput();
    //cout << "Valor Limiar : " << limitValue << endl;
    ThresholdFilter::Pointer thresholdFilter = ThresholdFilter::New();
    thresholdFilter -> SetInput(grayscaleFilter -> GetOutput());
    thresholdFilter -> SetOutsideValue(255);
    thresholdFilter -> SetUpper(optimalThreshold -> GetOutput()); // valor aleatorio
    thresholdFilter -> SetLower(0);
    thresholdFilter -> Update();
    GrayscaleImageType::Pointer thresholdImage = GrayscaleImageType::New();
    thresholdImage = thresholdFilter -> GetOutput();
    thresholdImage -> Update();
    
    /* ------------------------------------------------------------------------------------------------------ */
    
    /* ---------------------------------------- REGION GROWING ---------------------------------------------- */
    
    NeighborhoodConnectedFilterType::Pointer regionGrow = NeighborhoodConnectedFilterType::New();
    float lower = 0;
    float upper = optimalThreshold -> GetOutput();
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
    
    /* ------------------------------------------------------------------------------------------------------ */
    
    /* --------------------------------------------- OR IMAGEFILTER ----------------------------------------- */
    
    OrFilterType::Pointer orFilter = OrFilterType::New();
    orFilter -> SetInput(0, thresholdImage);
    orFilter -> SetInput(1, regionGrow -> GetOutput());
    orFilter -> Update();
    
    /* ------------------------------------------------------------------------------------------------------- */
    
    /* --------------------------------------------- MORFOLOGY ------------------------------------------------ *
    
    StructuringElementType structuringElement;
    structuringElement.SetRadius(4);
    structuringElement.CreateStructuringElement();
    BinaryDilateImageFilterType::Pointer dilateFilter = BinaryDilateImageFilterType::New();
    dilateFilter -> SetInput(orFilter -> GetOutput());
    dilateFilter -> SetKernel(structuringElement);
    dilateFilter -> Update();
    
    //BinaryErodeImageFilterType::Pointer erodeFilter = BinaryErodeImageFilterType::New();
    //erodeFilter -> SetInput(dilateFilter -> GetOutput());
    //erodeFilter -> SetKernel(structuringElement);
    //erodeFilter -> Update();
    
    /* ------------------------------------------------------------------------------------------------------- */
    
    /* --------------------------------------------- DATA COLLECT -------------------------------------------- */
    
    //DataCollector* dataCollector = new DataCollector(dilateFilter -> GetOutput());
    DataCollector* dataCollector = new DataCollector(orFilter -> GetOutput());
    
    /* ------------------------------------------------------------------------------------------------------- */
    
    /* ------------------------------------------ GENERATE RESULTS ------------------------------------------- */
    
    
    stringstream imageOut;
    imageOut << "gray_" << imageName;
    cout  << "imageName : " << imageOut.str() << endl;
    WriterType::Pointer writer = WriterType::New();
    writer -> SetFileName(imageOut.str());
    writer -> SetInput(orFilter -> GetOutput());
    writer -> Update();
    
    /* ------------------------------------------------------------------------------------------------------- */

    
    
    
    
    ImageData imageData;
    imageData.SetPxCount(dataCollector -> GetPxCount());
    imageData.SetCellAvrage(dataCollector -> GetCellAvrage());
    imageData.SetThresholdValue(limitValue);
    cout << "thresholdvalue : " << imageData.GetThresholdValue() << " | pxCount : " << imageData.GetPxCount()  << " | cellAvrage : " << imageData.GetCellAvrage() << endl;
    KNNClassifier* knnClassifier = new KNNClassifier(resultFileName , k , imageData);
    cout << "Classe : " << knnClassifier -> FindLabel() << endl;
    
    return EXIT_SUCCESS;
}