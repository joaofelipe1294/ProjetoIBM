//
//  DataCollector.hpp
//  ProjetoIBM
//
//  Created by joao lopes on 23/05/16.
//
//

#ifndef DataCollector_hpp
#define DataCollector_hpp

#include "itkImage.h"
#include <stdio.h>

using namespace itk;

typedef Image<unsigned char , 2> GrayscaleImageType;

class DataCollector{
public:
    DataCollector(GrayscaleImageType::Pointer image);
private:
    GrayscaleImageType::Pointer image;
    int pxCount;
    int cellAvrage;
    int aberration;
    void GetData();
    void CalcAberration();
public:
    int GetPxCount();
    int GetCellAvrage();
    int GetAberration();
};
#endif /* DataCollector_hpp */
