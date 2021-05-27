/** \example testing.cpp
 *  \brief Programa para el testeo de las funciones.
    
Para compilar o código principal.cpp:
\code{.sh}
    g++ -static -o testing testing.cpp -lpdsmlmm -lpdsspmm -lpdsramm -Wall -Wextra
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
std::string dirpath="dataset/testing";
std::string ext_x="Xdat";
std::string ext_y="Ydat";

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
    Pds::Matrix F;
    
    std::vector<std::string> FileXdat;
    std::vector<std::string> FileYdat;
    
    Pds::Ra::MakeDir(outputpath);

    ModelBlock Model;
    Model=model::load(modelpath,filename_w,filename_mean,filename_std);
    int M=model::order(Model.W.Nel(),Model.Mean.Nel());
    if(M<0) {pds_print_error_message("Problems finding M."); return 0;}

    
    // Loading data                                        
    preproccesing::reading_input_files(dirpath,ext_x,ext_y,FileXdat,FileYdat);
    
    for(unsigned int k=0;k<FileXdat[k].size();k++)
    {
        DS=preproccesing::get_featureblock_of_one_sample(FileXdat[k],FileYdat[k]);
        
        // Feature scaling
        DS.X.NormalizeColsWith(Model.Mean,Model.Std);
        Pds::Perceptron Neurona(Model.W);

        // Evaluate data
        F=Pds::Kernel::Polynomial(DS.X,M);
        Yeval=Neurona.Evaluate(F);
        
        Metrics = Pds::ClassificationMetrics::Calculate(0.5,Yeval,DS.Y);
        Metrics.Print("\nMetrics of testing data:\n");
        
        // Convierto un vector columna en una matriz (imagen)
        Yeval.Reshape(DS.Nlin,DS.Ncol); 
        
        // Post procesado (plot) de la matriz Yeval.
        postproccesing::create_outputfiles(outputpath,Yeval,Pds::Ra::Filename(FileXdat[k]));
    }   
    return 0;
}
