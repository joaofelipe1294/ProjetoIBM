#include <iostream>
#include "FileHandler.hpp"
#include "OptimalThreshold.hpp"
#include "DataCollector.hpp"
#include "KNNClassifier.hpp"
#include "LabelFinder.hpp"

#include "TrainPipeline.hpp"
#include "PreProcessPipeline.hpp"

using namespace std;
using namespace itk;



int main(int argc, char * argv[]){
    string imageName , dirName , input;
    int k = NULL;
    bool needToTrain = NULL;
    if(argc == 1){
        cout << "Treinar (S/N): ";
        cin >> input;
        if(input.compare("S") == 0 || input.compare("s") == 0){
            needToTrain = true;
            cout << "Pasta de treinamento : ";
            cin >> dirName;
        }else if(input.compare("N") || input.compare("n")){
            needToTrain = false;
        }
        cout << "K : ";
        cin >> k;
        cout << "Imagem a ser classificada : ";
        cin >> imageName;
    }else if(argc == 4){
        dirName = argv[1];
        k = atoi(argv[2]);
        imageName = argv[3];
    }else if(strcmp(argv[1], "-t")){
        dirName = argv[2];
        k = atoi(argv[3]);
        imageName = argv[4];
    }
    
/* -------------------------------------------------- TRAIN ---------------------------------------------- */
    
    if(needToTrain == true){
        TrainPipeline* trainPipeline = new TrainPipeline(dirName);
        trainPipeline -> Train();
    }
    
/* ------------------------------------------------------------------------------------------------------- */

/* ------------------------------------------------- PRE PROCESS ------------------------------------------ */
    
    PreProcessPipeline* preProcess = new PreProcessPipeline(imageName);
    ImageData imageData = preProcess -> Process();
    
/* --------------------------------------------------------------------------------------------------------- */
    
    //cout << "thresholdvalue : " << imageData.GetThresholdValue() << " | pxCount : " << imageData.GetPxCount()  << " | cellAvrage : " << imageData.GetCellAvrage() << endl;
    KNNClassifier* knnClassifier = new KNNClassifier(RESULT_FILENAME , k , imageData);
    cout << "+------------+" << endl;
    cout << "| Classe : " << knnClassifier -> FindLabel() << " | " <<  endl;
    cout << "+------------+" << endl;
    return EXIT_SUCCESS;
}