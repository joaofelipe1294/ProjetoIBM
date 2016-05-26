//
//  ImageData.hpp
//  ProjetoIBM
//
//  Created by joao lopes on 23/05/16.
//
//

#ifndef ImageData_hpp
#define ImageData_hpp

#include <stdio.h>

class ImageData{
private:
    int thresholdValue;
    int pxCount;
    int cellAvrage;
    int label;
    int distance;
public:
    void SetPxCount(int pxCount);
    void SetCellAvrage(int cellAvrage);
    void SetThresholdValue(int thresholdValue);
    void SetLabel(int label);
    void SetDistance(int distance);
    int GetPxCount();
    int GetCellAvrage();
    int GetThresholdValue();
    int GetLabel();
    int GetDistance();
};

#endif /* ImageData_hpp */