# evaluate.cpp

Evalua un conjunto de archivos *.Xdat desde un directorio.

compilar:

    g++ -static -o evaluate evaluate.cpp -lpdsmlmm -lpdsspmm -lpdsramm -Wall -Wextra

comando:

    ./evaluate

# testing.cpp

Evalua y mide un conjunto de archivos *.Xdat y *.Ydat desde un directorio.

compilar:

    g++ -static -o testing testing.cpp -lpdsmlmm -lpdsspmm -lpdsramm -Wall -Wextra

comando:

    ./testing


# training.cpp

Entrena, evalua y mide un conjunto de archivos *.Xdat y *.Ydat desde un directorio.

compilar:

    g++ -static -o training training.cpp -lpdsmlmm -lpdsspmm  -lpdscamm -lpdsramm -Wall -Wextra

comando:

    ./training



