//
//  KNNClassifier.hpp
//  ProjetoIBM
//
//  Created by joao lopes on 23/05/16.
//
//

#ifndef KNNClassifier_hpp
#define KNNClassifier_hpp

#include <stdio.h>

class KNNClassifier{
private:
    FILE* filePointer;
public:
    KNNClassifier(FILE* filePointer);
};

#endif /* KNNClassifier_hpp */
