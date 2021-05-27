/** \example listbmpfiles.cpp
 *  \brief Programa para el testeo de las funciones.
    
Para compilar o código principal.cpp:
\code{.sh}
    g++ -static -o listbmpfiles listbmpfiles.cpp -lpdsramm
\endcode
Para executar o programa:
\code{.sh}
    ./listbmpfiles
\endcode  
 */
 
     
#include <cmath>
#include <Pds/Ra>
#include <algorithm>

std::string     dir_default=".";
std::string pattern_default="*.bmp";
std::string exclude_default=".*";
std::string outfile_default="salida.txt";


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
    
    return D;
}

int main(int argc,char *argv[])
{
    Pds::CmdHelp Help=init_help();
    
    if( Pds::Ra::ExistArgument(argc,argv,"--help","-h") )
    {
        Help.Print();
        return 0;
    }
    
    std::string dir     = Pds::Ra::GetStringArgument(argc,argv,"--dir",dir_default);
    std::string pattern = Pds::Ra::GetStringArgument(argc,argv,"--pattern",pattern_default);
    std::string exclude = Pds::Ra::GetStringArgument(argc,argv,"--exclude",exclude_default);
    std::string outfile = Pds::Ra::GetStringArgument(argc,argv,"--outfile","-o",outfile_default);
    
    if(dir.compare(".")==0)
    {
        dir=Pds::Ra::Pwd();
        //std::cout<<"dir:"<<dir<<"\n";
    }
    
    std::vector<std::string> bmp_file; 	
    bmp_file=Pds::Ra::GetFiles(dir,pattern,exclude);
    
    std::sort(bmp_file.begin(), bmp_file.end(),Pds::Ra::NaturalCompare);
    
    Pds::Ra::SaveVectorString(outfile,bmp_file);
    
    return 0;
}
