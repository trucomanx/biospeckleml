import dataset_module    as datmod
import biospeckle_module as biomod

import numpy as np

from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import PolynomialFeatures
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split

def training_model(data_x_list,data_y_list,test_size=0.7,show=False):
    
    ## Convierte los datos a una lista de matrices con columnas (avd,mean,std)
    data_x_pixel_list,data_y_pixel_list=datmod.dataset_list_to_pixel_dataset_list(data_x_list,data_y_list);
    #data_x_list=[];data_y_list=[];
    
    if(show):
        datmod.plot_pixel_dataset_list(data_x_pixel_list,data_y_pixel_list,percent=0.01,png_filepath='scatter3d.png')
    
    ## COncatena todas las matrices con columnas (avd,mean,std)
    X_pixel,y_pixel=datmod.concatenate_list_of_data(data_x_pixel_list,data_y_pixel_list);
    data_x_pixel_list=[];data_y_pixel_list=[];
    
    ## Escala los datos para normalizarlos
    scaler = StandardScaler()
    scaler.fit(X_pixel);
    X_pixel=scaler.transform(X_pixel);
    
    ## Aplicamos kernel polinomial
    Order=2;
    poly = PolynomialFeatures(Order,include_bias=False);
    X_pixel= poly.fit_transform(X_pixel);
    
    ## Divido los datos en entrenamiento y test
    X_train, X_test, y_train, y_test = train_test_split(X_pixel,y_pixel, test_size=test_size)
    
    ## Aplicando regresion logistica
    logisticRegr = LogisticRegression();
    logisticRegr.fit(X_train, y_train);
    
    model={'poly':poly,'scaler':scaler,'logreg':logisticRegr,'ksizeend':1}
    
    ## Score de los datos
    score_best=0;
    ksize_best=3;
    confmat_best=[];
    for k in range(3,19,2):
        model['ksizeend']=k;
        confmat=check_conf_matrix(model,data_x_list,data_y_list,threshold=0.5)
        
        score=confmat[0][0]+confmat[1][1];
        if(score>score_best):
            score_best=score;
            ksize_best=k;
            confmat_best=confmat;
        print('Testing ksize_end: ',k,'\tObtained score: ',score)
    
    print("Best confmat:\n",confmat_best)
    print("\n")
    
    model['ksizeend']=ksize_best;
    
    return model, score_best;

def predict_from_sample_mat3d(model,mat,threshold=0.5):
    Sz=np.shape(mat);
    X_mat=datmod.dataset_x_to_pixel_dataset_x(mat, flat_type='F');
    X_mat=model['scaler'].transform(X_mat);
    X_mat=model['poly'].fit_transform(X_mat);
    y_mat=model['logreg'].predict(X_mat);
    y_mat=np.reshape(y_mat,(Sz[1],Sz[2]), order='F')
    y_mat=datmod.datapack_spatial_mean_filter([y_mat],ksize=model['ksizeend'])[0];
    y_mat=np.where(y_mat>threshold, 1.0, 0.0);
    return y_mat;

from sklearn.metrics import confusion_matrix
def check_conf_matrix(model,data_x_list,data_y_list,threshold=0.5):
    n=0;
    for X,y in zip(data_x_list,data_y_list):
        y_pr=predict_from_sample_mat3d(model,X,threshold=threshold);
        if(n==0):
            confmat=confusion_matrix(y.flatten(), y_pr.flatten());
        else:
            confmat=confmat+confusion_matrix(y.flatten(), y_pr.flatten());
        n=n+1;
    confmat=confmat/confmat.sum();
    return confmat;

def predict_model(model,zipfile):
    datapack=biomod.datapack_from_zipfile(zipfile);
    mat=datmod.datapack_to_sample_dataset(datapack,ksize=7);
    
    y_mat=predict_from_sample_mat3d(model,mat);
    return y_mat
