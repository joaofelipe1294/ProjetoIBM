//
//  PreProcessPipeline.cpp
//  ProjetoIBM
//
//  Created by joao lopes on 02/06/16.
//
//

#include "PreProcessPipeline.hpp"

PreProcessPipeline::PreProcessPipeline(string imageName){
    this -> imageName = imageName;
}

void PreProcessPipeline::Process(){
        int limitValue = 0;
    
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
    //stringstream imageOutOpen;
    //imageOutOpen << "open_" << imageName;
    //cout  << "imageName : " << imageOutOpen.str() << endl;
    //WriterType::Pointer writerOpen = WriterType::New();
    //writerOpen -> SetFileName(imageOutOpen.str());
    //writerOpen -> SetInput(openingFilter -> GetOutput());
    //writerOpen -> Update();
    
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
    
/* ------------------------------------------ RETURN COMPUTED VALUES ------------------------------------- */
    
    //ImageData imageData;
    //imageData.SetPxCount(dataCollector -> GetPxCount());
    //imageData.SetCellAvrage(dataCollector -> GetCellAvrage());
    //imageData.SetThresholdValue(limitValue);
    
    FILE* resultFile;
    resultFile = fopen("image.txt", "w");
    if(dataCollector -> GetPxCount() == 0){
        fprintf(resultFile,"%d,%d,%d\n",limitValue * 2 , dataCollector -> GetCellAvrage() , dataCollector -> GetAberration());
    }else{
        fprintf(resultFile,"%d,%d,%d\n",limitValue , dataCollector -> GetCellAvrage() , dataCollector -> GetAberration());
    }
    //fprintf(resultFile, "%d,%d,%d" , imageData.get)
    
    
    //return imageData;
    
/* ------------------------------------------------------------------------------------------------------- */
    
}