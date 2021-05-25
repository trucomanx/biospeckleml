/** \example principal_arbol.cpp
 *  \brief Programa para el testeo de las funciones.
    
Para compilar o código principal.cpp:
\code{.sh}
    g++ -static -o principal_arbol principal_arbol.cpp -lpdsnnmm -lpdsmlmm -lpdsspmm  -lpdscamm -lpdsramm
\endcode
Para executar o programa:
\code{.sh}
    ./principal_arbol
\endcode  
 */
    
#include <cmath>
#include <Pds/Ra>
#include <Pds/Ca>
#include <Pds/Sp>
#include <Pds/Ml>
#include <Pds/Nn>


std::string dirpath ="/mnt/boveda/DATASETs/BIOSPECKLE/cafe-biospeckle/sem1";
std::string pattern ="*.bmp";
std::string imglabel="/mnt/boveda/DATASETs/BIOSPECKLE/cafe-biospeckle/sem1.bmp";
std::string output  ="./output";

#include "headers/extra_preproccesing.cpp"
#include "headers/extra_postproccesing.cpp"


int main(void)
{

    ////////////////////////////////////////////////////////////////////////////
    // ETAPA 1
    
    DatStruct DS;
    // Obtem DS.X y DS.Y
    DS=pre_proccesing2(dirpath,pattern,imglabel);
    std::cout<<std::endl;
        
    ////////////////////////////////////////////////////////////////////////////
    // ETAPA 2
    
    // Split data
    Pds::Ra::Tic();
    Pds::DataSetBlock Dat=Pds::DataSet::Split(DS.X,DS.Y,1,1,48);	
    Pds::Ra::Toc();
    
    std::cout<<"Dat.Ytr.Min():"<<Dat.Ytr.Min()<<"\n";
    std::cout<<"Dat.Ytr.Max():"<<Dat.Ytr.Max()<<"\n";
    // Ploting training data
    std::cout<<"Training samples: "<<Dat.Ytr.Size()<<std::endl;
    
    Pds::Octave::XLabel="Mean";
    Pds::Octave::YLabel="Std";
    Pds::Octave::ZLabel="AVD";
    Pds::Octave::Plot::ScatterX3DY( Dat.Xtr,Dat.Ytr,
                                    output+Pds::Ra::FileSep+"dataset_training.m",
                                    output+Pds::Ra::FileSep+"dataset_training.png");	
    
    // Feature scaling !!!!!!!!!!!!!!!!!!!!!!!!!!!
    Pds::Vector Mean(Dat.Xtr.Ncol());
    Pds::Vector Std(Dat.Xtr.Ncol());
    
    Dat.Xtr.NormalizeCols(Mean,Std);
    Dat.Xcv.NormalizeColsWith(Mean,Std);
    Dat.Xtt.NormalizeColsWith(Mean,Std);
    DS.X.NormalizeColsWith(Mean,Std);
    ////////////////////////////////////////////////////////////////////////////
    // ETAPA 3
     
    Pds::Matrix Yeval;
    
    // Entrenando un arbol
    Pds::DecisionTree::Counter=0;
    Pds::DecisionTree Arbol(Dat.Xtr,Dat.Ytr,0.9,8);
    std::cout<<"Pds::DecisionTree::Counter: "<<Pds::DecisionTree::Counter<<std::endl;
    
    // Evaluate el arbol
    Yeval=Arbol.Evaluate(Dat.Xcv);
    
    Pds::Octave::Plot::ScatterX3DY( Dat.Xcv,Yeval,
                                    output+Pds::Ra::FileSep+"dataset_crossval.m",
                                    output+Pds::Ra::FileSep+"dataset_crossval.png");	
    
    // Metrics of training
    Pds::ClassificationMetrics Metrics; 
    Metrics = Pds::ClassificationMetrics::Calculate(0.5,Yeval,Dat.Ycv);
    Metrics.Print("\n");

    ////////////////////////////////////////////////////////////////////////////
    // ETAPA 4
    
    // Evaluate el arbol
    Yeval=Arbol.Evaluate(DS.X);
    
    // Convierto un vector columna en una matriz (imagen)
    Yeval.Reshape(DS.Nlin,DS.Ncol); 
    
    // Post procesado (plot) de la matriz Yeval.
    post_proccesing1(output,Yeval);

    return 0;
}
