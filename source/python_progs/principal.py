import sys
sys.path.append("extras")
import files_module      as flmod
import dataset_module    as datmod
import biospeckle_module as biomod

dirs=flmod.scan_all_subdirs("/home/fernando/Downloads/biospeckleml/source");

[print(dirpath) for dirpath in dirs ]

zipfile='/home/fernando/Dropbox/DATASET/biospeckle1/data1.zip';

datapack=biomod.datapack_from_zipfile(zipfile,pattern="*.bmp");
mat=datmod.datapack_to_sample_dataset(datapack);
print("mat[0]\n",mat[0])
print("mat[1]\n",mat[1])
print("mat[2]\n",mat[2])
