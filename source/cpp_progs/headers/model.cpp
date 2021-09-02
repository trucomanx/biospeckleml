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
                    
    bool save(  const ModelBlock &Model,
                const std::string &modelpath,
                const std::string &filename_w,
                const std::string &filename_mean,
                const std::string &filename_std);

    int order(unsigned int Wnel, unsigned int N);

}

int model::order(unsigned int Wnel, unsigned int N)
{
    unsigned int L;
    unsigned int m;
    L=0;
    for(m=0;L<Wnel;m++)
    {
        L+=Pds::NchooseK(N+m-1,m);
        
        if(L==Wnel) 
        {
            std::cout<<"   M:"<<m<<std::endl<<std::endl;
            return m;
        }
    }
    
    std::cout<<"   M:"<<-1<<std::endl<<std::endl;
    return -1;
}

bool model::save(   const ModelBlock &Model,
                    const std::string &modelpath,
                    const std::string &filename_w,
                    const std::string &filename_mean,
                    const std::string &filename_std)
{
    std::string filepath_w    = Pds::Ra::FullFile({modelpath,filename_w});
    std::string filepath_mean = Pds::Ra::FullFile({modelpath,filename_mean});
    std::string filepath_std  = Pds::Ra::FullFile({modelpath,filename_std});
    
    unsigned int Nw=filepath_w.length();
    unsigned int Nmean=filepath_mean.length();
    unsigned int Nstd=filepath_std.length();
    unsigned int N=std::max(Nw,std::max(Nmean,Nstd));
    
    // Loading model
    Model.W.Save(filepath_w);
    Model.Mean.Save(filepath_mean);
    Model.Std.Save(filepath_std);
    

    unsigned int width_w   =log10(Model.W.Nlin());
    unsigned int width_mean=log10(Model.Mean.Nlin());
    unsigned int width_std =log10(Model.Std.Nlin());
    unsigned int width=std::max(width_w,std::max(width_mean,width_std))+1;

    std::cout<<"\nSaving model: "<<std::endl;    
    std::cout<<"    W: "<<std::setw(N)<<filepath_w;
    std::cout<<"\tsize: "<<std::setw(width)<<Model.W.Nlin()<<" "<<Model.W.Ncol()<<std::endl;
    std::cout<<" Mean: "<<std::setw(N)<<filepath_mean;
    std::cout<<"\tsize: "<<std::setw(width)<<Model.Mean.Nlin()<<" "<<Model.Mean.Ncol()<<std::endl;
    std::cout<<"  Std: "<<std::setw(N)<<filepath_std;
    std::cout<<"\tsize: "<<std::setw(width)<<Model.Std.Nlin()<<" "<<Model.Std.Ncol()<<std::endl;
    
    std::cout<<std::endl;
    
    return true;
}



ModelBlock model::load( const std::string &modelpath,
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
    
    // Loading model
    Model.W.Load(filepath_w);
    Model.Mean.Load(filepath_mean);
    Model.Std.Load(filepath_std);
    

    unsigned int width_w   =log10(Model.W.Nlin());
    unsigned int width_mean=log10(Model.Mean.Nlin());
    unsigned int width_std =log10(Model.Std.Nlin());
    unsigned int width=std::max(width_w,std::max(width_mean,width_std))+1;
    
    std::cout<<"    W: "<<std::setw(N)<<filepath_w;
    std::cout<<"\tsize: "<<std::setw(width)<<Model.W.Nlin()<<" "<<Model.W.Ncol()<<std::endl;
    std::cout<<" Mean: "<<std::setw(N)<<filepath_mean;
    std::cout<<"\tsize: "<<std::setw(width)<<Model.Mean.Nlin()<<" "<<Model.Mean.Ncol()<<std::endl;
    std::cout<<"  Std: "<<std::setw(N)<<filepath_std;
    std::cout<<"\tsize: "<<std::setw(width)<<Model.Std.Nlin()<<" "<<Model.Std.Ncol()<<std::endl;
    
    std::cout<<std::endl;
    
    return Model;
}

