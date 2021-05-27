
namespace octaveplot{
    
    /** \brief Imprime un grafico en 3 dimensiones con los datos en X, colores son agregados
     *  a las muestras usando Y. Las imagenes resultantes son guardadas usando "prename".
     *
     *  \param[in] outputpath Directorio de salida del plot.
     *  \param[in] X Datos de la feature X.
     *  \param[in] Y Datos de la feature Y.
     *  \param[in] prename El prename usado para genera "[prename].m" y "[prename].png".
     */
    void featureX3DY(   const std::string &outputpath,
                        const Pds::Matrix &X,
                        const Pds::Matrix &Y,
                        const std::string &prename);

}

void octaveplot::featureX3DY(   const std::string &outputpath,
                                const Pds::Matrix &X,
                                const Pds::Matrix &Y,
                                const std::string &prename)
{
    Pds::Octave::XLabel="Mean";
    Pds::Octave::YLabel="Std";
    Pds::Octave::ZLabel="AVD";
    std::string filename_m  =outputpath+Pds::Ra::FileSep+prename+".m";
    std::string filename_png=outputpath+Pds::Ra::FileSep+prename+".png";
    
    std::cout<<"\n";
    std::cout<<"+++ Printing data using OCTAVE ...\n";
    Pds::Ra::Tic();
    Pds::Octave::Plot::ScatterX3DY(X,Y,filename_m,filename_png);
    std::cout<<"+++ ";Pds::Ra::Toc();
    std::cout<<"+++ Files "+outputpath+Pds::Ra::FileSep+prename+".* were printed.\n";
}

