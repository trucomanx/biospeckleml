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

#include <sys/stat.h>
#include <sys/types.h>

#include "headers/preproccesing.cpp"
#include "headers/postproccesing.cpp"
#include "headers/octaveplot.cpp"

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


int main(void)
{
    Pds::ClassificationMetrics Metrics; 
    FeatureBlock DS;

    Pds::Matrix Yeval;
    unsigned int M=3;
    Pds::Matrix F;

    Pds::IterationConf Conf; 
    Conf.Show=true; 
    Conf.SetMinError(1e-6);
    Conf.SetMaxIter(10000);
    Conf.SetLambda(0.02);
        
    Pds::Ra::MakeDir(Pds::Ra::FullFile({outputpath,modelsubdir}));
    Pds::Ra::MakeDir(outputpath);
 
    // Obtem DS.X y DS.Y
    DS=preproccesing::get_featureblock_compound_samples(dirpath,ext_x,ext_y,0.2);
    if(DS.X.IsEmpty())
    {
        pds_print_error_message("Problems finding samples *."+ext_x+" and *."+ext_y+" in: "+dirpath);
        return 0;
    }
    
    // Split data
    Pds::DataSetBlock Dat=Pds::DataSet::Split(DS.X,DS.Y,1,1,23);	

    std::cout<<"Used training samples: "<<Dat.Ytr.Nel()<<std::endl;
    std::cout<<" Used crosval samples: "<<Dat.Ycv.Nel()<<std::endl;
    std::cout<<" Used testing samples: "<<Dat.Ytt.Nel()<<std::endl;
    std::cout<<"----------------------------------"<<std::endl;
    std::cout<<"        Total samples: "<<DS.Y.Nel()<<std::endl;

    // Ploting training data
    octaveplot::featureX3DY(outputpath,Dat.Xtr,Dat.Ytr,"dataset_training");
    
    // Feature scaling !!!!!!!!!!!!!!!!!!!!!!!!!!!
    Pds::Vector Mean(Dat.Xtr.Ncol());
    Pds::Vector Std(Dat.Xtr.Ncol());
    
    Dat.Xtr.NormalizeCols(Mean,Std);
    Dat.Xcv.NormalizeColsWith(Mean,Std);
    Dat.Xtt.NormalizeColsWith(Mean,Std);
    
    // Trainig the model
    F=Pds::Kernel::Polynomial(Dat.Xtr,M);

    Pds::Perceptron Neurona(Conf,F,Dat.Ytr);
    Neurona.Print("\nNeurona:\n");
    std::cout<<"W.Size(): "<<Neurona.GetW().Size()<<std::endl;
    
    // Evaluate cross-val data
    F=Pds::Kernel::Polynomial(Dat.Xcv,M);
    Yeval=Neurona.Evaluate(F);
    
    Metrics = Pds::ClassificationMetrics::Calculate(0.5,Yeval,Dat.Ycv);
    Metrics.Print("\nMetrics of cross-validation data:\n");

    octaveplot::featureX3DY(outputpath,Dat.Xcv,Yeval,"dataset_crossval");
    
    // Evaluate testing data
    F=Pds::Kernel::Polynomial(Dat.Xtt,M);
    Yeval=Neurona.Evaluate(F);
    
    Metrics = Pds::ClassificationMetrics::Calculate(0.5,Yeval,Dat.Ytt);
    Metrics.Print("\nMetrics of testing data:\n");
    
    // Saving data
    Neurona.GetW().Save(Pds::Ra::FullFile({outputpath,modelsubdir,filename_w}));
    Mean.Save(Pds::Ra::FullFile({outputpath,modelsubdir,filename_mean}));
    Std.Save(Pds::Ra::FullFile({outputpath,modelsubdir,filename_std}));
    
    return 0;
}
