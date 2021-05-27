#include <Pds/Ra>

struct ModelBlock  
{  
    /* W for logistic polinomial kernel.*/
    Pds::Matrix W;
    
    /* Feature scaling: Mean.*/
    Pds::Vector Mean;
    
    /* Feature scaling: Std.*/
    Pds::Vector Std;
    
    /* Poly kernel order.*/
    unsigned int M;
};

#include <iomanip> //std::setw

namespace model{

    ModelBlock load(const std::string &modelpath,
                    const std::string &filename_w,
                    const std::string &filename_mean,
                    const std::string &filename_std);

}

ModelBlock load(const std::string &modelpath,
                const std::string &filename_w,
                const std::string &filename_mean,
                const std::string &filename_std)
{
    ModelBlock Model;

    std::string filepath_w    = Pds::Ra::FullFile({modelpath,filename_w});
    std::string filepath_mean = Pds::Ra::FullFile({modelpath,filename_mean});
    std::string filepath_std=Pds::Ra::FullFile({modelpath,filename_std});
    
    unsigned int Nw=filepath_w.length();
    unsigned int Nmean=filepath_mean.length();
    unsigned int Nstd=filepath_std.length();
    unsigned int N=std::max(Nw,std::max(Nmean,Nstd));
    
    // Loading model W
    Model.W.Load(filepath_w);
    std::cout<<"   W: "<<std::setw(N)<<filepath_w<<"\tsize: "<<Model.W.Size()<<std::endl;
    
    // Loading model Mean
    Model.Mean.Load(filepath_mean);
    std::cout<<"Mean: "<<std::setw(N)<<filepath_mean<<"\tsize: "<<Model.Mean.Size()<<std::endl;
    
    // Loading model Std
    Model.Std.Load(filepath_std);
    std::cout<<" Std: "<<std::setw(N)<<filepath_std<<"\tsize: "<<Model.Std.Size()<<std::endl;
    
    std::cout<<std::endl;

    //Pds::Matrix(Model.W).T().Print("   W:");
    //Pds::Matrix(Model.Mean).T().Print("Mean:");
    //Pds::Matrix(Model.Std).T().Print(" Std:");    
    
    return Model;
}

