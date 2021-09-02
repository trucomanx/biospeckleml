import biospeckle_module as biomod
import numpy as np

def datapack_to_sample_dataset(datapack):
  avd=biomod.datapack_to_avd(datapack);
  mean,std=biomod.datapack_to_mean_and_std(datapack);
  Sz=np.shape(avd);
  mat=np.zeros((3,Sz[0],Sz[1]), dtype=float);
  mat[0,:,:]=avd;
  mat[1,:,:]=mean;
  mat[2,:,:]=std;
  return mat;
