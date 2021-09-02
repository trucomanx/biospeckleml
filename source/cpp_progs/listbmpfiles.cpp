/** \example listbmpfiles.cpp
 *  \brief Programa para el testeo de las funciones.
    
Para compilar o código principal.cpp:
\code{.sh}
    g++ -static -o listbmpfiles listbmpfiles.cpp -lpdsramm -Wall -Wextra
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


#include "listbmpfiles.hpp"

int main(int argc,char *argv[])
{
    std::string dir, pattern, exclude, outfile;
    
    listbmpfiles_init(  argc,argv,
                        dir    ,dir_default,
                        pattern,pattern_default,
                        exclude,exclude_default,
                        outfile,outfile_default);
    
    std::vector<std::string> bmp_file;
    
    // Obtiene todos los archivos
    bmp_file=Pds::Ra::GetFiles(dir,pattern,exclude);
    
    // Ordena en orden alfabético
    std::sort(bmp_file.begin(), bmp_file.end(),Pds::Ra::NaturalCompare);
    
    // Salva los archivos en un archivo
    if(bmp_file.size()>0)   Pds::Ra::SaveVectorString(outfile,bmp_file);
    else                    std::cout<<"No files found.\n";
    
    return 0;
}
