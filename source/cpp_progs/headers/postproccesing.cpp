namespace postproccesing{

    /** \brief Imprime un grafico en 3 dimensiones con los datos en X, colores son agregados
     *  a las muestras usando Y. Las imagenes resultantes son guardadas usando "prename".
     *
     *  \param[in] outputpath Directorio de salida de las imagenes.
     *  \param[in] ImgBinMat Matriz binaria a ser procesada.
     *  Si la matriz tiene Nlin lieneas y Ncol columnas las imagenes de salida
     *  tendran este mismo tamaño.
     *  \param[in] prename El prename usado para generar "[prename]_1.bmp", "[prename]_2.bmp" y "[prename]_3.bmp".
     */
    void create_outputfiles(const std::string &outputpath,
                            const Pds::Matrix &ImgBinMat, 
                            const std::string &prename);

}

void postproccesing::create_outputfiles(    const std::string &outputpath,
                                            const Pds::Matrix &ImgBinMat, 
                                            const std::string &prename)
{
    // Spatial filtering radio.
    unsigned int radio=5;
    
    // imprimiendo resultados
    Pds::Matrix Out=ImgBinMat*255;
    Out.ExportBmpFile(Pds::Colormap::Gray,outputpath+Pds::Ra::FileSep+prename+"_1.bmp");
    
    Pds::Matrix OutMean=Out.FilterMean(radio);
    OutMean.ExportBmpFile(Pds::Colormap::Jet,outputpath+Pds::Ra::FileSep+prename+"_2.bmp");
    
    Pds::Matrix OutBin=OutMean.Geq(128)*255;
    OutBin.ExportBmpFile(Pds::Colormap::Gray,outputpath+Pds::Ra::FileSep+prename+"_3.bmp");
}
