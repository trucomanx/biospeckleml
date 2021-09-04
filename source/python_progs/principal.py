import sys
sys.path.append("extras")
import files_module      as flmod
import dataset_module    as datmod
import biospeckle_module as biomod
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


X_pixel,y_pixel=datmod.concatenate_list_of_data(data_x_pixel_list,data_y_pixel_list);
data_x_pixel_list=[];data_y_pixel_list=[];

from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import PolynomialFeatures
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split

scaler = StandardScaler()
scaler.fit(X_pixel);
X_pixel=scaler.transform(X_pixel);

poly = PolynomialFeatures(2,include_bias=False);
X_pixel= poly.fit_transform(X_pixel);

X_train, X_test, y_train, y_test = train_test_split(X_pixel,y_pixel, test_size=0.33, random_state=42)

logisticRegr = LogisticRegression();
logisticRegr.fit(X_train, y_train);
score = logisticRegr.score(X_test, y_test);
print('score:',score)


