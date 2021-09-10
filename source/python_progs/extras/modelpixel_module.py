import dataset_module    as datmod
import biospeckle_module as biomod

import numpy as np

from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import PolynomialFeatures
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split

def training_model(data_x_list,data_y_list,test_size=0.7):
    data_x_pixel_list,data_y_pixel_list=datmod.dataset_list_to_pixel_dataset_list(data_x_list,data_y_list);
    #datmod.plot_pixel_dataset_list(data_x_pixel_list,data_y_pixel_list,png_filepath='scatter3d.png')
    data_x_list=[];data_y_list=[];

    X_pixel,y_pixel=datmod.concatenate_list_of_data(data_x_pixel_list,data_y_pixel_list);
    data_x_pixel_list=[];data_y_pixel_list=[];
    
    scaler = StandardScaler()
    scaler.fit(X_pixel);
    X_pixel=scaler.transform(X_pixel);
    
    Order=2;
    poly = PolynomialFeatures(Order,include_bias=False);
    X_pixel= poly.fit_transform(X_pixel);
    
    X_train, X_test, y_train, y_test = train_test_split(X_pixel,y_pixel, test_size=test_size)
    
    logisticRegr = LogisticRegression();
    logisticRegr.fit(X_train, y_train);
    
    score = logisticRegr.score(X_test, y_test);
    
    model={'poly':poly,'scaler':scaler,'logreg':logisticRegr}
    
    return model, score;

def predict_model(model,zipfile, threshold=0.5):
    datapack=biomod.datapack_from_zipfile(zipfile);
    mat=datmod.datapack_to_sample_dataset(datapack,ksize=7);
    Sz=np.shape(mat);
    X_mat=datmod.dataset_x_to_pixel_dataset_x(mat, flat_type='F');
    X_mat=model['scaler'].transform(X_mat);
    X_mat=model['poly'].fit_transform(X_mat);
    y_mat=model['logreg'].predict(X_mat);
    y_mat=np.reshape(y_mat,(Sz[1],Sz[2]), order='F')
    y_mat=datmod.datapack_spatial_mean_filter([y_mat],ksize=7)[0];
    y_mat=np.where(y_mat>threshold, 1.0, 0.0);
    return y_mat
