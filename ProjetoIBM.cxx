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
    string imageName , dirName , input , resultFileName;
    bool needToTrain = NULL;
    if(argc == 1){
        /*cout << "Treinar (S/N): ";
        cin >> input;
        if(input.compare("S") == 0 || input.compare("s") == 0){
            needToTrain = true;
            cout << "Pasta de treinamento : ";
            cin >> dirName;
        }else if(input.compare("N") || input.compare("n")){
            needToTrain = false;
        }*/
        //cout << "Imagem a ser classificada : ";
        //cin >> imageName;
        cout << "Diretorio treino : ";
        cin >> dirName;
        cout << "Arquivo de saida : ";
        cin >> resultFileName;
    //}else if (argc == 2){
    //    imageName = argv[1];
    //    needToTrain = false;
    }else if(argc ==  3){
        //needToTrain = true;
        dirName = argv[1];
        //imageName = argv[2];
        resultFileName = argv[2];
    }else{
        return EXIT_FAILURE;
    }
    
/* -------------------------------------------------- TRAIN ---------------------------------------------- */
    
    //if(needToTrain == true){
        TrainPipeline* trainPipeline = new TrainPipeline(dirName , resultFileName);
        trainPipeline -> Train();
    //}
    
/* ------------------------------------------------------------------------------------------------------- */

/* ------------------------------------------------- PRE PROCESS ------------------------------------------ *
    
    PreProcessPipeline* preProcess = new PreProcessPipeline(imageName);
    preProcess -> Process();
    
/* --------------------------------------------------------------------------------------------------------- */
    
    //cout << "thresholdvalue : " << imageData.GetThresholdValue() << " | pxCount : " << imageData.GetPxCount()  << " | cellAvrage : " << imageData.GetCellAvrage() << endl;
    //KNNClassifier* knnClassifier = new KNNClassifier(RESULT_FILENAME , k , imageData);
    //cout << "+------------+" << endl;
    //cout << "| Classe : " << knnClassifier -> FindLabel() << " | " <<  endl;
    //cout << "+------------+" << endl;
    return EXIT_SUCCESS;
}