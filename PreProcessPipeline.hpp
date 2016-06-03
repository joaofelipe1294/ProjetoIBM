//
//  PreProcessPipeline.hpp
//  ProjetoIBM
//
//  Created by joao lopes on 02/06/16.
//
//

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

const int DIMENSIONS_PRE_PROCESS = 2;

typedef Image<unsigned char , DIMENSIONS_PRE_PROCESS> GrayscaleImageType;
typedef RGBPixel<unsigned char> RGBPixelType;
typedef Image<RGBPixelType , DIMENSIONS_PRE_PROCESS> RGBImageType;
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

#ifndef PreProcessPipeline_hpp
#define PreProcessPipeline_hpp

#include <stdio.h>

class PreProcessPipeline{
private:
    string imageName;
public:
    PreProcessPipeline(string imageName);
    ImageData Process();
};

#endif /* PreProcessPipeline_hpp */
