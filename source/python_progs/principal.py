import sys
sys.path.append("extras")
import files_module      as flmod
import dataset_module    as datmod
import biospeckle_module as biomod
import numpy as np


url_x_zip_files=[
'https://www.dropbox.com/s/52vey50x8aqlida/data1.zip',
'https://www.dropbox.com/s/4s03earcvn8rck2/data2.zip',
'https://www.dropbox.com/s/9iiof3pta4175kv/data3.zip'
]

url_y_bmp_files=[
'https://www.dropbox.com/s/0g924qmiajs5c8z/data1.bmp?dl=0',
'https://www.dropbox.com/s/kfkkdauptr5azft/data2.bmp?dl=0',
'https://www.dropbox.com/s/ym988qeway41tag/data3.bmp?dl=0'
]

dirname='/home/fernando/Downloads/presentation-ocarina/sections';
dirs=flmod.scan_all_subdirs(dirname);
print(dirs)
