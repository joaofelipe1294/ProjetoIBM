//
//  OptimalThreshold.hpp
//  ProjetoIBM
//
//  Created by joao lopes on 22/05/16.
//
//

#ifndef OptimalThreshold_hpp
#define OptimalThreshold_hpp

#include <stdio.h>
#include "itkImage.h"

using namespace itk;

typedef Image<unsigned char , 2> GrayscaleImageType;

class Optimalthreshold{
private:
    GrayscaleImageType::Pointer originalImage;
    GrayscaleImageType::Pointer thresholdImage;
public:
    Optimalthreshold(GrayscaleImageType::Pointer);
    int GetOutput();
private:
    void ApplyThreshold(int value , GrayscaleImageType::Pointer original , GrayscaleImageType::Pointer thresholdImage);
};

#endif /* OptimalThreshold_hpp */
