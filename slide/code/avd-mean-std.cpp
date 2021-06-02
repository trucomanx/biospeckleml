/** \example avd-mean-std.cpp
 *  \brief Programa para el testeo de las funciones.
    
Para compilar o código principal.cpp:
\code{.sh}
    g++ -static -o avd-mean-std avd-mean-std.cpp -lpdsmlmm -lpdsspmm -lpdsramm -Wall -Wextra
\endcode
Para executar o programa:
\code{.sh}
    ./avd-mean-std
\endcode
testado con: libpdsspmm-0.1.2.tar.gz
testado con: libpdsramm-0.1.2.tar.gz
testado con: libpdsmlmm-0.1.2.tar.gz 
 */

#include <iostream>
#include <string>
#include <Pds/Ra>
#include <Pds/Ml>
#include <Pds/Sp>

std::string DIR="/mnt/boveda/DATASETs/BIOSPECKLE/cafe-biospeckle/sem1";

int main(void)//(int argc, char** argv)
{
	std::vector<Pds::Matrix> DATA;
	unsigned int radio=5;
	
	DATA=Pds::Image::BmpDataBlock(DIR,"*.bmp"); 
	
    DATA=Pds::Image::SpatialMeanFilterInBlock(DATA,radio);
	
	Pds::Matrix  AVD=Pds::Image::AVD(DATA);
	Pds::Matrix MEAN=Pds::Image::TemporalMean(DATA);
	Pds::Matrix  STD=Pds::Image::TemporalStd(DATA);
	
	DATA[0].ExportBmpFile(Pds::Colormap::Gray,"RAW.bmp");
	AVD.Scale(0,255).ExportBmpFile(Pds::Colormap::Jet,"AVD.bmp");
	STD.Scale(0,255).ExportBmpFile(Pds::Colormap::Jet,"STD.bmp");
	MEAN.Scale(0,255).ExportBmpFile(Pds::Colormap::Jet,"MEAN.bmp");
	return 0;
}
