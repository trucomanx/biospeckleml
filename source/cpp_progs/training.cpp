/** \example training.cpp
 *  \brief Programa para el testeo de las funciones.
    
Para compilar o código principal.cpp:
\code{.sh}
    g++ -static -o training training.cpp -lpdsmlmm -lpdsspmm -lpdsramm -Wall -Wextra
\endcode
Para executar o programa:
\code{.sh}
    ./training
\endcode
testado con: libpdsspmm-0.1.2.tar.gz
testado con: libpdsramm-0.1.2.tar.gz
testado con: libpdsmlmm-0.1.2.tar.gz 
 */
    
#include <cmath>
#include <Pds/Ra>
#include <Pds/Ca>
#include <Pds/Sp>
#include <Pds/Ml>
//#include <Pds/Nn>

#include <sys/stat.h>
#include <sys/types.h>

#include "headers/preproccesing.cpp"
#include "headers/postproccesing.cpp"
#include "headers/octaveplot.cpp"
#include "headers/model.cpp"


//inputs
std::string dirpath ="dataset/training";
std::string ext_x="Xdat";
std::string ext_y="Ydat";

// outputs
std::string outputpath ="output_training";
std::string modelsubdir="model";

std::string filename_w   ="LogisticModelW.dat";
std::string filename_mean="FeatureScalingMean.dat";
std::string filename_std ="FeatureScalingStd.dat";


#include "training.hpp"

int main(int argc, char *argv[])
{
    training_init(argc,argv,dirpath,outputpath);
    
    Pds::ClassificationMetrics Metrics; 
    FeatureBlock DS;
    ModelBlock Model;
    unsigned int M=3;
    Pds::Matrix Yeval;
    Pds::Matrix F;

    Pds::IterationConf Conf; 
    Conf.Show=true; Conf.SetMaxIter(10000); Conf.SetLambda(0.02);
        
    Pds::Ra::MakeDir(Pds::Ra::FullFile({outputpath,modelsubdir}));
    
    // Obtem DS.X y DS.Y
    DS=preproccesing::get_featureblock_compound_samples(dirpath,ext_x,ext_y,0.2);
    if(DS.X.IsEmpty())
    {
        pds_print_error_message("Problems finding samples *."+ext_x+" and *."+ext_y+" in: "+dirpath);
        return 0;
    }
    
    // Split data
    Model.Mean=Pds::Vector(DS.X.Ncol());
    Model.Std=Pds::Vector(DS.X.Ncol());
    Pds::DataSetBlock Dat=Pds::DataSet::SplitAndScaling(DS.X,DS.Y,1,1,1,Model.Mean,Model.Std);

    std::cout<<"Used training samples: "<<Dat.Ytr.Nel()<<std::endl;
    std::cout<<" Used crosval samples: "<<Dat.Ycv.Nel()<<std::endl;
    std::cout<<" Used testing samples: "<<Dat.Ytt.Nel()<<std::endl;
    std::cout<<"----------------------------------"<<std::endl;
    std::cout<<"        Total samples: "<<DS.Y.Nel()<<std::endl;

    // Ploting training data
    octaveplot::featureX3DY(outputpath,Dat.Xtr,Dat.Ytr,"dataset_training");
    
    
    // Trainig the model
    F=Pds::Kernel::Polynomial(Dat.Xtr,M);
    Pds::Perceptron Neurona(Conf,F,Dat.Ytr);
    Model.W=Neurona.GetW();
    
    Model.W.T().Print("\n Neurona:\n");
    
    // Evaluate cross-val data
    F=Pds::Kernel::Polynomial(Dat.Xcv,M);
    Yeval=Neurona.Predict(F);
    
    Metrics = Pds::ClassificationMetrics::Calculate(0.5,Yeval,Dat.Ycv);
    Metrics.Print("\nMetrics of cross-validation data:\n");

    octaveplot::featureX3DY(outputpath,Dat.Xcv,Yeval,"dataset_crossval");
    
    // Evaluate testing data
    F=Pds::Kernel::Polynomial(Dat.Xtt,M);
    Yeval=Neurona.Predict(F);
    
    Metrics = Pds::ClassificationMetrics::Calculate(0.5,Yeval,Dat.Ytt);
    Metrics.Print("\nMetrics of testing data:\n");
    
    // Saving model
    std::string modelpath=Pds::Ra::FullFile({outputpath,modelsubdir});
    model::save(Model,modelpath,filename_w,filename_mean,filename_std);
    
    return 0;
}
