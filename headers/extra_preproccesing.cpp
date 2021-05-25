struct DatStruct  
{  
    Pds::Matrix X;
    Pds::Vector Y;
    unsigned int Nlin;
    unsigned int Ncol;
};

DatStruct pre_proccesing(std::string dirpath,std::string pattern,std::string imglabel)
{
    DatStruct Dat;
    DatStruct ERROR;
    
    std::vector<Pds::Matrix> DATA;
    Pds::Matrix IMG;
    
    Pds::Matrix Xraw;
    
    unsigned int radio=5;
    unsigned int Ylimear=128;
    
    ////////////////////////////////////////////////////////////////////////////
    // ETAPA 0
    
    // Load Xraw
    DATA=Pds::Image::BmpDataBlock(dirpath,pattern);
    if(DATA.size()==0)
    {
        pds_print_error_message("Problems loading files from: "+dirpath);
        return ERROR;
    }
    
    Dat.Nlin=DATA[0].Nlin();
    Dat.Ncol=DATA[0].Ncol();
    
    DATA=Pds::Image::SpatialMeanFilterInBlock(DATA,radio);
    
    Pds::Matrix U  = Pds::Image::TemporalMean(DATA);
    Pds::Matrix STD= Pds::Image::TemporalStd(DATA,U);
    Pds::Matrix AVD= Pds::Image::AVD(DATA);
    
    Dat.X=Pds::MergeHor({Pds::Vector(U),Pds::Vector(STD),Pds::Vector(AVD)});
    DATA.clear();
    
    // Load Y
    IMG=Pds::Image::BmpToGrayMatrix(imglabel);
    Dat.Y.Vectorize(IMG);
    Dat.Y = Dat.Y.Geq(Ylimear);
    
    ////////////////////////////////////////////////////////////////////////////
    // Mostrando dadtos
    
    std::cout<<"  Total X.Size(): "<<Dat.X.Size()<<std::endl;
    std::cout<<"  Total Y.Size(): "<<Dat.Y.Size()<<std::endl;    
    std::cout<<std::endl;
        
    return Dat;
}
