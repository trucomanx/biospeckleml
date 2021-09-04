import sys
sys.path.append("extras")
#import files_module      as flmod
import dataset_module    as datmod
#import biospeckle_module as biomod
import modelpixel_module as modelmod
import numpy as np

x_zip_files=[
'/home/fernando/Dropbox/DATASET/biospeckle1/data1.zip',
'/home/fernando/Dropbox/DATASET/biospeckle1/data2.zip',
'/home/fernando/Dropbox/DATASET/biospeckle1/data3.zip'
]

y_bmp_files=[
'/home/fernando/Dropbox/DATASET/biospeckle1/data1.bmp',
'/home/fernando/Dropbox/DATASET/biospeckle1/data2.bmp',
'/home/fernando/Dropbox/DATASET/biospeckle1/data3.bmp'
]


data_x_list,data_y_list=datmod.create_dataset_list(x_zip_files,y_bmp_files)
#datmod.plot_sample_of_dataset(data_x_list[0],data_y_list[0],'sample.png')


data_x_pixel_list,data_y_pixel_list=datmod.dataset_list_to_pixel_dataset_list(data_x_list,data_y_list);
#datmod.plot_pixel_dataset_list(data_x_pixel_list,data_y_pixel_list,png_filepath='scatter3d.png')
data_x_list=[];data_y_list=[];


################################################################################

    
model, score=modelmod.training_model(data_x_pixel_list,data_y_pixel_list,test_size=0.7);

print('score:',score)
print('model:',model)
################################################################################



import pickle
with open("test.moddat", "wb") as fp:   #Pickling
    pickle.dump(model, fp)

'''
with open("test.moddat", "rb") as fp:   # Unpickling
    RES = pickle.load(fp)

print(RES)
'''

################################################################################

zipfile='/home/fernando/Dropbox/DATASET/biospeckle1/data1.zip';



y_mat=modelmod.predict_model(model,zipfile);

import matplotlib.pyplot as plt
plt.imshow(y_mat)
plt.show();

