/** \brief Estructura para almacenar de forma agrupada datos X e Y
 *  para ser usados en algun algoritmo de clasificacion binaria.
 *
 */
struct FeatureBlock  
{  
    /* Dato (matriz) X con el mismo numero de lineas que Y.*/
    Pds::Matrix X;
    
    /* Dato (vector) Y con el mismo numero de lineas que X.*/
    Pds::Vector Y;
    
    /* Numero de lineas de X.*/
    unsigned int Nlin;
    
    /* Numero de lineas de Y.*/
    unsigned int Ncol;
};

#include <algorithm>                

namespace preproccesing{

    Pds::Matrix get_featureblock_of_one_sample_x(   const std::string &FileXdat,
                                                    unsigned int &Nlin,
                                                    unsigned int &Ncol);
                                                    
    Pds::Vector get_featureblock_of_one_sample_y(   const std::string &FileYdat,
                                                    unsigned int &Nlin,
                                                    unsigned int &Ncol);
                                                            
    /** \brief Crea un FeatureBlock lista para ser usada en un algoritmo de clasificacion binaria.
     *  Esta es leida desde los archivos sample block x (FileXdat) e y (FileYdat). 
     *
     *  \param[out] FileXdat   Archivos con samples block X. 
     *  Este archivo tiene una lista de filepath de imagenes bmp.
     *  \param[out] FileYdat   Archivos con samples block Y.
     *  Este archivo tiene una unica linea con el filepath de una imagen bmp 
     *  que contiene el marcado (objetivo) para el entrenamiento.
     *  \return Retorna un FeatureBlock lista para ser usada en un algoritmo de clasificacion binaria.
     */
    FeatureBlock get_featureblock_of_one_sample(std::string FileXdat,
                                                std::string FileYdat);

    /** \brief Crea una lista de archivos con samples block X e Y desde un 
     *  directorio dirpath y busca archivos con extension ext_x e ext_y.
     *  La funcion verifica que a cada VecFileXdat[i] le corresponda un VecFileYdat[i],
     *  la funcion descarta VecFileXdat[i] que no tengan correspondecia.
     *
     *  \param[in] dirpath Directorio de lectura de archivos.
     *  \param[in] ext_x   Extension de archivo de cada sample block X.
     *  \param[in] ext_y   Extension de archivo de cada sample block Y.
     *  \param[out] VecFileXdat   Lista de archivos con samples block X. 
     *  Debe tener el mismo numero de elementos que VecFileYdat.
     *  Cada archivo tiene una lista de filepath de imagenes bmp.
     *  \param[out] VecFileYdat   Lista de archivos con samples block Y.
     *  Debe tener el mismo numero de elementos que VecFileXdat.
     *  Cada archivo tiene una unica linea con el filepath de una imagen bmp 
     *  que contiene el marcado (objetivo) para el entrenamiento.
     */
    void reading_input_files(   std::string dirpath,
                                std::string ext_x,
                                std::string ext_y,
                                std::vector<std::string> &VecFileXdat,
                                std::vector<std::string> &VecFileYdat);


    /** \brief Retorna un FeatureBlock creado por la composición vertical
     *  de porciones (factor) de otras FeatureBlock.
     *  Cada FeatureBlock es obtenida desde los archivos sample block X e Y. 
     *
     *  \param[in] dirpath Directorio de lectura de archivos.
     *  \param[in] ext_x   Extension de archivo de cada sample block X.
     *  \param[in] ext_y   Extension de archivo de cada sample block Y.
     *  \param[in] p       Factor del total de muestras en X e Y a ser extraidas en cada FeatureBlock.
     *  \return Retorna un FeatureBlock creado por la composición vertical
     *  de porciones (factor) de otras FeatureBlock.
     *  Esta puede ser usada en un algoritmo de clasificacion binaria.
     */
    FeatureBlock get_featureblock_compound_samples( std::string dirpath,
                                                    std::string ext_x,
                                                    std::string ext_y,
                                                    double p);

}

////////////////////////////////////////////////////////////////////////////////

Pds::Matrix preproccesing::get_featureblock_of_one_sample_x(const std::string &FileXdat,
                                                            unsigned int &Nlin,
                                                            unsigned int &Ncol)
{
    unsigned int radio=5;
    
    std::vector<Pds::Matrix> DATA;
    std::vector<std::string> FilenameBmpX;
        
    // Load Xraw
    FilenameBmpX=Pds::Ra::GenSignificativeLines(FileXdat);
    
    DATA=Pds::Image::BmpDataBlock(FilenameBmpX);
    if(DATA.size()==0)
    {
        pds_print_error_message("Problems loading files list in: "+FileXdat);
        return Pds::Matrix();
    }
    
    Nlin=DATA[0].Nlin();
    Ncol=DATA[0].Ncol();
    
    DATA=Pds::Image::SpatialMeanFilterInBlock(DATA,radio);
    
    Pds::Matrix U  = Pds::Image::TemporalMean(DATA);
    Pds::Matrix STD= Pds::Image::TemporalStd(DATA,U);
    Pds::Matrix AVD= Pds::Image::AVD(DATA);
    
    Pds::Matrix X=Pds::MergeHor({Pds::Vector(U),Pds::Vector(STD),Pds::Vector(AVD)});
    DATA.clear();
    return X;
}

Pds::Vector preproccesing::get_featureblock_of_one_sample_y(const std::string &FileYdat,
                                                            unsigned int &Nlin,
                                                            unsigned int &Ncol)
{
    Pds::Matrix IMG;
    Pds::Vector Y;
    unsigned int Ylimear=128;
    
    std::vector<std::string> FilenameBmpY=Pds::Ra::GenSignificativeLines(FileYdat); 
    if(FilenameBmpY.size()==0)
    {
        pds_print_error_message("Problems loading files list in: "+FileYdat);
        return Pds::Matrix();
    }
    
    IMG=Pds::Image::BmpToGrayMatrix(FilenameBmpY[0]);
    
    Nlin=IMG.Nlin();
    Ncol=IMG.Ncol();
    
    Y.Vectorize(IMG);
    Y = Y.Geq(Ylimear);
    return Y;
}

FeatureBlock preproccesing::get_featureblock_of_one_sample( std::string FileXdat,
                                                            std::string FileYdat)
{
    FeatureBlock Dat;
    FeatureBlock ERROR; ERROR.X.MakeEmpty(); ERROR.Y.MakeEmpty();
    
    // Load X  
    Dat.X=get_featureblock_of_one_sample_x(FileXdat,Dat.Nlin,Dat.Ncol);
    if(Dat.X.IsEmpty())
    {
        pds_print_error_message("Problems loading files list in: "+FileXdat);
        return ERROR;
    }                                             
    
    // Load Y
    unsigned int Nlin,Ncol;
    Dat.Y = preproccesing::get_featureblock_of_one_sample_y(FileYdat,Nlin,Ncol);
    if(Dat.Y.IsEmpty())
    {
        pds_print_error_message("Problems loading files list in: "+FileYdat);
        return ERROR;
    }
    
    // verifico tamanhos
    if((Dat.Nlin!=Nlin)||(Dat.Ncol!=Ncol))
    {
        pds_print_error_message("Tamaños diferentes entre: \n"+FileXdat+"\n"+FileYdat+"\n");
        return ERROR;
    }
    
    // Mostrando dadtos
    std::cout<<" filepath X: "<<FileXdat<<std::endl;
    std::cout<<"   X.Size(): "<<Dat.X.Size()<<std::endl;
    std::cout<<" filepath Y: "<<FileYdat<<std::endl;
    std::cout<<"   Y.Size(): "<<Dat.Y.Size()<<std::endl;    
    std::cout<<std::endl;
        
    return Dat;
}

////////////////////////////////////////////////////////////////////////////////

void preproccesing::reading_input_files(std::string dirpath,
                                        std::string ext_x,
                                        std::string ext_y,
                                        std::vector<std::string> &VecFileXdat,
                                        std::vector<std::string> &VecFileYdat)
{
    std::vector<std::string> File;
    VecFileXdat.clear();
    VecFileYdat.clear();
    
    std::string name,dir,tmp;
    
    // Obteniendo todos los archivos X
    File=Pds::Ra::GetFiles(dirpath,"*."+ext_x,"");
    std::sort(File.begin(), File.end(),Pds::Ra::NaturalCompare);

    // Verificando si tienen sus correspondientes Y
    for(unsigned int k=0;k<File.size();k++)
    {
        if(Pds::Ra::IsFile(File[k]))
        {
            dir=Pds::Ra::Dirname(File[k]);
            name=Pds::Ra::Filename(File[k]);
            tmp=Pds::Ra::FullFile({dir,name+"."+ext_y});
            if(Pds::Ra::IsFile(tmp))
            {
                VecFileXdat.push_back(File[k]);
                VecFileYdat.push_back(tmp);
            }
        }
    }
    
    // Imprimiendo
    /*
    std::cout<<"Sample in X:\n";
    Pds::Ra::PrintVectorString(VecFileXdat);
    std::cout<<"\n";
    
    std::cout<<"Sample in Y:\n";
    Pds::Ra::PrintVectorString(VecFileYdat);
    std::cout<<"\n";
    */
} 

////////////////////////////////////////////////////////////////////////////////

FeatureBlock preproccesing::get_featureblock_compound_samples(  std::string dirpath,
                                                                std::string ext_x,
                                                                std::string ext_y,
                                                                double p)
{
    FeatureBlock D;
    p=fabs(p);
    
    if(p>1.0) return D;
    
    std::vector<std::string> VecFileXdat(0);
    std::vector<std::string> VecFileYdat(0);
    
    // Obteniendo VecFileXdat y VecFileYdat
    preproccesing::reading_input_files(dirpath,ext_x,ext_y,VecFileXdat,VecFileYdat);
    
    // verifico si hay archivos
    if(VecFileXdat.size()==0) return D;
    if(VecFileXdat.size()!=VecFileYdat.size())  return D;
    
    Pds::Matrix X;
    Pds::Vector Y;
    std::vector<unsigned int> ID;
    unsigned int N;
    
    for(unsigned int k=0;k<VecFileXdat.size();k++)
    {
        D=preproccesing::get_featureblock_of_one_sample(VecFileXdat[k],VecFileYdat[k]);
        N=p*D.Y.Nel();
        ID=Pds::Ra::IndicesRandVector(D.Y,0.5,N);
        
        if(k==0)
        {
            X=D.X.GetRows(ID);
            Y=D.Y.GetRows(ID);
        }
        else
        {
            X=Pds::MergeVer({X,D.X.GetRows(ID)});
            Y=Pds::MergeVer({Y,D.Y.GetRows(ID)});
        }
        
    }
    
    D.X=X;
    D.Y=Y;
    D.Nlin=X.Nlin();
    D.Ncol=X.Ncol();
    
    return D;
}
