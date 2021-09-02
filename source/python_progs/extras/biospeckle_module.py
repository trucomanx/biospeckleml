import shutil
import os
import glob
from natsort import natsorted
from skimage import color
from skimage import io
import numpy as np

import files_module      as flmod

## return a list of numpy arrays
def datapack_from_zipfile(zipfile,pattern="*.bmp",temporal_dir='temporal_dir'):
  print("working over: ",zipfile);
  shutil.unpack_archive(zipfile, temporal_dir)
  
  dirs=flmod.scan_all_subdirs(temporal_dir)
  
  pattern_filepath=os.path.join(dirs[0],pattern);
  bmp_files=natsorted(glob.glob(pattern_filepath));
  datapack=[];
  for data in bmp_files:
    IMG=color.rgb2gray(io.imread(data));
    datapack.append(np.array(IMG,dtype='f'));
  try:
    shutil.rmtree(temporal_dir)
  except OSError as e:
    print("Error: %s - %s." % (e.filename, e.strerror))
  return datapack

def datapack_to_avd(datapack):
  avd=datapack[0]*0;
  L=len(datapack);
  for n in range(1,L):
    avd=avd+np.abs(datapack[n]-datapack[n-1])/(L-1.0);
  return avd;

def datapack_to_mean_and_std(datapack):
  mean=datapack[0]*0;
  std =datapack[0]*0;
  L=len(datapack);
  for n in range(L):
    mean=mean+datapack[n]/L;
  for n in range(L):
    std=std+np.power(datapack[n]-mean,2)/L;
  std=np.power(std,0.5);
  return mean,std;

