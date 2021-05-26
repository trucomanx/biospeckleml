/** \example training.cpp
 *  \brief Programa para el testeo de las funciones.
    
Para compilar o código principal.cpp:
\code{.sh}
    g++ -static -o training training.cpp -lpdsnnmm -lpdsmlmm -lpdsspmm  -lpdscamm -lpdsramm
\endcode
Para executar o programa:
\code{.sh}
    ./training
\endcode  
 */
    
#include <cmath>
#include <Pds/Ra>
#include <Pds/Ca>
#include <Pds/Sp>
#include <Pds/Ml>
//#include <Pds/Nn>

#include "headers/extra_preproccesing.cpp"
#include "headers/extra_postproccesing.cpp"


//inputs
//std::string training_path ="/mnt/boveda/DATASETs/BIOSPECKLE/cafe-biospeckle/training";
std::string dirpath ="/mnt/boveda/DATASETs/BIOSPECKLE/cafe-biospeckle/training/sem1";
std::string pattern ="*.bmp";
std::string imglabel="/mnt/boveda/DATASETs/BIOSPECKLE/cafe-biospeckle/training/sem1.bmp";

// outputs
std::string outputpath ="output";
std::string modelpath  ="model";
std::string filename_w="LogisticModelW.dat";
std::string filename_mean="FeatureScalingMean.dat";
std::string filename_std="FeatureScalingStd.dat";

int main(void)
{
    DatStruct DS;
    // Obtem DS.X y DS.Y
    DS=pre_proccesing(dirpath,pattern,imglabel);
    
    ////////////////////////////////////////////////////////////////////////////
    // ETAPA 1
    
    // Split data
    Pds::DataSetBlock Dat=Pds::DataSet::Split(DS.X,DS.Y,1,1,48);	

    // Ploting training data
    std::cout<<"Training samples: "<<Dat.Ytr.Size()<<std::endl;
    
    Pds::Octave::XLabel="Mean";
    Pds::Octave::YLabel="Std";
    Pds::Octave::ZLabel="AVD";
    Pds::Octave::Plot::ScatterX3DY( Dat.Xtr,Dat.Ytr,
                                    outputpath+Pds::Ra::FileSep+"dataset_training.m",
                                    outputpath+Pds::Ra::FileSep+"dataset_training.png");
    
    // Feature scaling !!!!!!!!!!!!!!!!!!!!!!!!!!!
    Pds::Vector Mean(Dat.Xtr.Ncol());
    Pds::Vector Std(Dat.Xtr.Ncol());
    
    Dat.Xtr.NormalizeCols(Mean,Std);
    Dat.Xcv.NormalizeColsWith(Mean,Std);
    Dat.Xtt.NormalizeColsWith(Mean,Std);
    DS.X.NormalizeColsWith(Mean,Std);
    ////////////////////////////////////////////////////////////////////////////
    // ETAPA 2
    
    Pds::Matrix Yeval;
    unsigned int M=3;
    Pds::Matrix F;

    Pds::IterationConf Conf; 
    Conf.Show=true; Conf.SetMinError(1e-6);Conf.SetMaxIter(10000);Conf.SetLambda(0.02);
    
    F=Pds::Kernel::Polynomial(Dat.Xtr,M);

    Pds::Perceptron Neurona(Conf,F,Dat.Ytr);
    Neurona.GetW().Save(modelpath+Pds::Ra::FileSep+filename_w);
    
    Neurona.Print("\nNeurona:\n");
    
    // Evaluate training data
    F=Pds::Kernel::Polynomial(Dat.Xcv,M);
    Yeval=Neurona.Evaluate(F);
    
    Pds::Octave::Plot::ScatterX3DY( Dat.Xcv,Yeval,
                                    outputpath+Pds::Ra::FileSep+"dataset_crossval.m",
                                    outputpath+Pds::Ra::FileSep+"dataset_crossval.png");	
    
    // Metrics of training
    Pds::ClassificationMetrics Metrics; 
    Metrics = Pds::ClassificationMetrics::Calculate(0.5,Yeval,Dat.Ycv);
    Metrics.Print("\n");

    ////////////////////////////////////////////////////////////////////////////
    // ETAPA 3

    // Evaluate training data
    F=Pds::Kernel::Polynomial(DS.X,M);
    Yeval=Neurona.Evaluate(F);
    
    // Convierto un vector columna en una matriz (imagen)
    Yeval.Reshape(DS.Nlin,DS.Ncol); 
    
    // Post procesado (plot) de la matriz Yeval.
    post_proccesing1(outputpath,Yeval,"gray");

    return 0;
}
