/** \example testing.cpp
 *  \brief Programa para el testeo de las funciones.
    
Para compilar o código principal.cpp:
\code{.sh}
    g++ -static -o testing testing.cpp -lpdsmlmm -lpdsspmm -lpdsramm
\endcode
Para executar o programa:
\code{.sh}
    ./testing
\endcode  
 */
    
#include <cmath>
#include <Pds/Ra>
#include <Pds/Sp>
#include <Pds/Ml>

#include "headers/preproccesing.cpp"
#include "headers/postproccesing.cpp"
#include "headers/model.cpp"

//inputs data
std::string filepath_samplex="dataset/testing/sem1.Xdat";
std::string filepath_sampley="dataset/testing/sem1.Ydat";

// input model
std::string modelpath    ="output_training/model";
std::string filename_w   ="LogisticModelW.dat";
std::string filename_mean="FeatureScalingMean.dat";
std::string filename_std ="FeatureScalingStd.dat";

// outputs
std::string outputpath ="output_testing";



int main(void)
{
    Pds::ClassificationMetrics Metrics; 
    std::string filepath;
    FeatureBlock DS;
    
    Pds::Matrix Yeval;
    unsigned int M=3;
    Pds::Matrix F;
    
    Pds::Ra::MakeDir(outputpath);

    // Loading model W
    filepath=Pds::Ra::FullFile({modelpath,filename_w});
    Pds::Matrix W(Pds::Ra::TextFormat,filepath);
    std::cout<<"   W: "<<filepath<<"\tsize: "<<W.Size()<<std::endl;
    // Loading model Mean
    filepath=Pds::Ra::FullFile({modelpath,filename_mean});
    Pds::Vector Mean(Pds::Ra::TextFormat,filepath);
    std::cout<<"Mean: "<<filepath<<"\tsize: "<<Mean.Size()<<std::endl;
    // Loading model Std
    filepath=Pds::Ra::FullFile({modelpath,filename_std});
    Pds::Vector Std(Pds::Ra::TextFormat,filepath);
    std::cout<<" Std: "<<filepath<<"\tsize: "<<Std.Size()<<std::endl;
    std::cout<<std::endl;

    Pds::Matrix(W).T().Print("   W:");
    Pds::Matrix(Mean).T().Print("Mean:");
    Pds::Matrix(Std).T().Print(" Std:");
    
    // Loading data
    DS=preproccesing::get_featureblock_of_one_sample(filepath_samplex,filepath_sampley);
    
    // Loadig models
    DS.X.NormalizeColsWith(Mean,Std);
    Pds::Perceptron Neurona(W);

    // Evaluate data
    F=Pds::Kernel::Polynomial(DS.X,M);
    Yeval=Neurona.Evaluate(F);
    
    Metrics = Pds::ClassificationMetrics::Calculate(0.5,Yeval,DS.Y);
    Metrics.Print("\nMetrics of testing data:\n");
    
    // Convierto un vector columna en una matriz (imagen)
    Yeval.Reshape(DS.Nlin,DS.Ncol); 
    
    // Post procesado (plot) de la matriz Yeval.
    postproccesing::create_outputfiles(outputpath,Yeval,Pds::Ra::Filename(filepath_samplex));
    
    return 0;
}
