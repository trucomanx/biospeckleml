/** \example evaluate.cpp
 *  \brief Programa para el testeo de las funciones.
    
Para compilar o código principal.cpp:
\code{.sh}
    g++ -static -o evaluate evaluate.cpp -lpdsmlmm -lpdsspmm -lpdsramm -Wall -Wextra
\endcode
Para executar o programa:
\code{.sh}
    ./evaluate
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
std::string dirpath="dataset/evaluate";
std::string ext_x="Xdat";

// input model
std::string modelpath    ="output_training/model";
std::string filename_w   ="LogisticModelW.dat";
std::string filename_mean="FeatureScalingMean.dat";
std::string filename_std ="FeatureScalingStd.dat";

// outputs
std::string outputpath ="output_evaluate";



int main(void)
{ 
    Pds::Matrix F;
    Pds::Matrix X;
    Pds::Matrix Yeval;
    unsigned int Nlin,Ncol;
    
    std::vector<std::string> FileXdat;
    
    Pds::Ra::MakeDir(outputpath);

    ModelBlock Model;
    Model=model::load(modelpath,filename_w,filename_mean,filename_std);
    int M=model::order(Model.W.Nel(),Model.Mean.Nel());
    if(M<0) {pds_print_error_message("Problems finding M."); return 0;}

    
    // Loading data                                        
    FileXdat=Pds::Ra::GetFiles(dirpath,"*."+ext_x,"");
    
    for(unsigned int k=0;k<FileXdat.size();k++)
    {
        std::cout<<"working in: "<<FileXdat[k]<<std::endl;
        
        X=preproccesing::get_featureblock_of_one_sample_x(FileXdat[k],Nlin,Ncol);
        
        // Feature scaling
        X.NormalizeColsWith(Model.Mean,Model.Std);
        Pds::Perceptron Neurona(Model.W);

        // Evaluate data
        F=Pds::Kernel::Polynomial(X,M);
        Yeval=Neurona.Evaluate(F);
        
        // Convierto un vector columna en una matriz (imagen)
        Yeval.Reshape(Nlin,Ncol); 
        
        // Post procesado (plot) de la matriz Yeval.
        postproccesing::create_outputfiles(outputpath,Yeval,Pds::Ra::Filename(FileXdat[k]));
    }   
    return 0;
}
