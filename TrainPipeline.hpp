//
//  TrainPipeline.hpp
//  ProjetoIBM
//
//  Created by joao lopes on 02/06/16.
//
//

#ifndef TrainPipeline_hpp
#define TrainPipeline_hpp

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
const string RESULT_FILENAME = "results.txt";

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


class TrainPipeline{
private:
    string dirName;
public:
    TrainPipeline(string dirName);
    void Train();
};

#endif /* TrainPipeline_hpp */
