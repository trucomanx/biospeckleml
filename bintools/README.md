# Bibliotecas necesarias

    https://sourceforge.net/projects/libpdsmm/files/sources/libpdrapmm
    https://sourceforge.net/projects/libpdsmm/files/sources/libpdsspmm
    https://sourceforge.net/projects/libpdsmm/files/sources/libpdmlpmm

# evaluate.cpp

Evalua un modelp a partir de un conjunto de archivos *.Xdat en un directorio.

compilar:

    g++ -static -o evaluate evaluate.cpp -lpdsmlmm -lpdsspmm -lpdsramm -Wall -Wextra

comando:

    ./evaluate -h

# testing.cpp

Evalua y mide un modelo a partir de un conjunto de archivos *.Xdat y *.Ydat en un directorio.

compilar:

    g++ -static -o testing testing.cpp -lpdsmlmm -lpdsspmm -lpdsramm -Wall -Wextra

comando:

    ./testing -h


# training.cpp

Entrena, evalua y mide un medole a partir de conjunto de archivos *.Xdat y *.Ydat en un directorio.

compilar:

    g++ -static -o training training.cpp -lpdsmlmm -lpdsspmm  -lpdscamm -lpdsramm -Wall -Wextra

comando:

    ./training -h



