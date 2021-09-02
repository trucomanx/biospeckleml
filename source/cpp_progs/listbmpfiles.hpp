Pds::CmdHelp init_help(void)
{    
    Pds::CmdHelp D("listbmpfiles","0.0.1");
    
    D.SetCommandExample("listbmpfiles --dir /path/to/dir --pattern \"*.bmp\" --outfile \"out.txt\"\n\t"
                        "listbmpfiles --dir /path/to/dir --outfile \"out.txt\"");
    D.AddParam(0,"--help"   ,"-h","Habilita comentario de ajuda y finaliza el programa.","no habilitado");
    D.AddParam(1,"--dir"    ,""  ,"Directorio de entrada.",dir_default);
    D.AddParam(1,"--pattern",""  ,"Patron de busqueda del archivos.",pattern_default);
    D.AddParam(1,"--exclude",""  ,"Patron de exclusion de busqueda del archivos.",exclude_default);
    D.AddParam(1,"--outfile","-o","Archivo de salida.",outfile_default);
    D.SetAbstract("Lista los archivos en un directorio.");
    
    return D;
}

void listbmpfiles_init( int argc,char *argv[],
                        std::string &dir    ,std::string dir_default,
                        std::string &pattern,std::string pattern_default,
                        std::string &exclude,std::string exclude_default,
                        std::string &outfile,std::string outfile_default)
{
    Pds::CmdHelp Help=init_help();
    
    if( Pds::Ra::ExistArgument(argc,argv,"--help","-h")||(argc==1) )
    {
        Help.Print();
        exit(0);
    }
    
    dir     = Pds::Ra::GetStringArgument(argc,argv,"--dir",dir_default);
    pattern = Pds::Ra::GetStringArgument(argc,argv,"--pattern",pattern_default);
    exclude = Pds::Ra::GetStringArgument(argc,argv,"--exclude",exclude_default);
    outfile = Pds::Ra::GetStringArgument(argc,argv,"--outfile","-o",outfile_default);
    
    if(dir.compare(".")==0)
    {
        dir=Pds::Ra::Pwd();
    }
    
    std::cout<<"    --dir "<<dir<<"\n";
    std::cout<<"--pattern "<<pattern<<"\n";
    std::cout<<"--exclude "<<exclude<<"\n";
    std::cout<<"--outfile "<<outfile<<"\n";
}
