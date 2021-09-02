Pds::CmdHelp init_help(void)
{    
    Pds::CmdHelp D("training","0.0.1");
    
    D.SetCommandExample("training --dir /path/to/dir --out-dir \"/path/to/outdir\"");
    D.AddParam(0,"--help"   ,"-h","Habilita comentario de ajuda y finaliza el programa.","no habilitado");
    D.AddParam(1,"--dir"    ,"-d","Directorio de entrada.",dirpath);
    D.AddParam(1,"--out-dir","-o","Archivo de salida.",outputpath);
    
    return D;
}

void training_init( int argc, char *argv[], std::string &dirpath, std::string &outputpath)
{
    Pds::CmdHelp Help=init_help();
    
    if( Pds::Ra::ExistArgument(argc,argv,"--help","-h") )
    {
        Help.Print();
        exit(0);
    }

    dirpath    = Pds::Ra::GetStringArgument(argc,argv,"--dir","-d",dirpath);
    outputpath = Pds::Ra::GetStringArgument(argc,argv,"--out-dir","-o",outputpath);
    
}
