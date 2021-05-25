struct DatStruct  
{  
    Pds::Matrix X;
    Pds::Vector Y;
    unsigned int Nlin;
    unsigned int Ncol;
};

Pds::Vector NonZeroFrequencyEntropy(Pds::Matrix &Xmod)
{
    Pds::Vector Factor(Xmod.Nlin());
    unsigned int Nlin=Xmod.Nlin();
    unsigned int Ncol=Xmod.Ncol();
    double max,hb,p;
    
    double sum1,sum2;
    unsigned int lin,col;
    for(lin=0;lin<Nlin;lin++)
    {
        max=0;
        for(col=1;col<Ncol;col++)
        max+=Xmod.GetRaw(lin,col);
        
        hb=0;
        for(col=1;col<Ncol;col++)
        {
            p=Xmod.GetRaw(lin,col)/max;
            if(p!=0)    hb+=p*log2(p);
        }
        
        Factor.SetRaw(lin,hb);
    }
    
    return Factor;
}

DatStruct pre_proccesing1(std::string dirpath,std::string pattern,std::string imglabel)
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
    
    Xraw=Pds::Matrix::GetSamples(DATA);
    DATA.clear();
    
    // Load Y
    IMG=Pds::Image::BmpToGrayMatrix(imglabel);
    Dat.Y.Vectorize(IMG);
    Dat.Y = Dat.Y.Geq(Ylimear);
    
    ////////////////////////////////////////////////////////////////////////////
    // Mostrando dadtos
    
    std::cout<<"Xraw.Size(): "<<Xraw.Size()<<std::endl;
    std::cout<<"   Y.Size(): "<<Dat.Y.Size()<<std::endl;    
    
    ////////////////////////////////////////////////////////////////////////////
    // ETAPA 2
    
    unsigned int Nft=Xraw.Ncol();
    Pds::Fft FT(Nft);
    Pds::CMatrix Xft(Xraw.Nlin(),Nft);
    
    FT.Evaluate(Xraw,Xft);
    Xraw.MakeEmpty();

    ////////////////////////////////////////////////////////////////////////////
    // ETAPA 3
    Pds::Matrix Xmod=Xft.Module();
    Xft.MakeEmpty();
    //Xmod.SetColValue(0,0.0);
    
    Pds::Vector Factor=NonZeroFrequencyEntropy(Xmod);
    Pds::Vector Mean(Xmod.Nlin());
    Pds::Vector Std(Xmod.Nlin());
    Xmod.MeanStdInRows(Mean,Std);
    
    Dat.X=Pds::MergeHor({Mean,Std,Factor});
    // Dat.X=Pds::MergeHor({Mean,Std,MeanHFreq/MeanLFreq}); No funciona max acompanha mean e std
    // Dat.X=Pds::MergeHor({Mean,Std,Max}); No funciona max acompanha mean
    
    return Dat;
}


DatStruct pre_proccesing2(std::string dirpath,std::string pattern,std::string imglabel)
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
    
    std::cout<<"X.Size(): "<<Dat.X.Size()<<std::endl;
    std::cout<<"Y.Size(): "<<Dat.Y.Size()<<std::endl;    
    
    return Dat;
}
