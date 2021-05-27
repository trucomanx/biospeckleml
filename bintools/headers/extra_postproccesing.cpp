
// valores reales entre 0 y 1
void post_proccesing1(const std::string &output,const Pds::Matrix &Bout, const std::string &prename)
{
    // Spatial filtering radio.
    unsigned int radio=5;
    
    // imprimiendo resultados
    Pds::Matrix Out=Bout*255;
    Out.ExportBmpFile(Pds::Colormap::Gray,output+Pds::Ra::FileSep+prename+"_1.bmp");
    
    Pds::Matrix OutMean=Out.FilterMean(radio);
    OutMean.ExportBmpFile(Pds::Colormap::Jet,output+Pds::Ra::FileSep+prename+"_2.bmp");
    
    Pds::Matrix OutBin=OutMean.Geq(128)*255;
    OutBin.ExportBmpFile(Pds::Colormap::Gray,output+Pds::Ra::FileSep+prename+"_3.bmp");
}
