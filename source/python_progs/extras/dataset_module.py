import biospeckle_module as biomod
import numpy as np
from skimage import io
from scipy import signal
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

## concatenate nmatrix in a list
def concatenate_list_of_data(data_x_pixel_list,data_y_pixel_list):
    n=0;
    X_dataset=[];
    y_dataset=[];
    for data_x,data_y in zip(data_x_pixel_list,data_y_pixel_list):
        if(n==0):
            X_dataset=data_x;
            y_dataset=data_y;
        else:
            X_dataset=np.concatenate((X_dataset,data_x),axis=0)
            y_dataset=np.concatenate((y_dataset,data_y),axis=0)
        n=n+1;
    return X_dataset,y_dataset
    

## Filter a datapack with a mean kernel of size ksize
## datapack: list od numpy array
## return a datapack filtered
def datapack_spatial_mean_filter(datapack,ksize=7):
    datapack_res=[];
    Kernel=np.ones((ksize,ksize))/(ksize*ksize*1.0);
    for IMG in datapack:
        IMG=signal.convolve2d(IMG, Kernel, mode='same');
        datapack_res.append(IMG);
    return datapack_res;
    

## Convert a datapack to sample 
## datapack: List of 2D numpy array
##   sample: 2D numpy array
def datapack_to_sample_dataset(datapack,ksize=7):
    datapack=datapack_spatial_mean_filter(datapack,ksize);
    avd=biomod.datapack_to_avd(datapack);
    mean,std=biomod.datapack_to_mean_and_std(datapack);
    Sz=np.shape(avd);
    mat=np.zeros((3,Sz[0],Sz[1]), dtype=float);
    mat[0,:,:]=avd;
    mat[1,:,:]=mean;
    mat[2,:,:]=std;
    return mat;
  

## return a data y from bmp file
def data_y_from_file(bmpfile):
    print("working over: ",bmpfile);
    img_y=io.imread(bmpfile);
    
    if(img_y.dtype=='uint8'):
        img_y=np.where(img_y>128, 1.0, 0.0);
    else:
        max_y=np.max(img_y);
        min_y=np.min(img_y);
        t=(max_y+min_y)/2.0;
        img_y=np.where(img_y>t, 1.0, 0.0);
    
    if len(np.shape(img_y))>2 :
        img_y=color.rgb2gray(img_y);
    
    return np.array(img_y,dtype='f');
    
## create a dataset list from a zip and bmp file
def create_dataset_list(x_zip_files,y_bmp_files,ksize=7):
    if len(x_zip_files)!=len(y_bmp_files):
        print('The list files have different sizes.');
        sys.exit();
    
    data_x_list=[];
    data_y_list=[];
    for zipfile, bmpfile in zip(x_zip_files,y_bmp_files):
        ## Create x mat
        datapack=biomod.datapack_from_zipfile(zipfile);
        mat=datapack_to_sample_dataset(datapack,ksize);
        data_x_list.append(mat);
        
        ## Create y mat
        mat=data_y_from_file(bmpfile);
        data_y_list.append(mat);
        
    return data_x_list,data_y_list

## Plot one sample return of create_dataset_list()
def plot_sample_of_dataset(data_x,data_y,png_filepath='sample.png'):
    SzX=np.shape(data_x);
    SzY=np.shape(data_y);
    
    if(np.size(SzX)!=3):
        print("np.shape(data_x):",SzX);
        sys.exit('The data_x dont have dimension equal to 3');
    if(np.size(SzY)!=2):
        print("np.shape(data_y):",SzY);
        sys.exit('The data_x dont have dimension equal to 2');
    
    fig, axs = plt.subplots(1, 4, figsize=(16,4))
    
    pos0=axs[0].imshow(data_x[0,:,:], cmap='jet')#
    axs[0].set_title('Avd');
    fig.colorbar(pos0, ax=axs[0])
    
    pos1=axs[1].imshow(data_x[1,:,:], cmap='jet')#
    axs[1].set_title('Mean');
    fig.colorbar(pos1, ax=axs[1])
    
    pos2=axs[2].imshow(data_x[2,:,:], cmap='jet')#
    axs[2].set_title('Std');
    fig.colorbar(pos2, ax=axs[2])
    
    pos3=axs[3].imshow(data_y, cmap='gray')#
    axs[3].set_title('Result');
    fig.colorbar(pos3, ax=axs[3])
    
    plt.savefig(png_filepath);
    plt.show()


def dataset_x_to_pixel_dataset_x(data_x, flat_type='F'):
    Sz=np.shape(data_x);
    
    mat=np.zeros((Sz[1]*Sz[2],Sz[0]));
    for n in range(Sz[0]):
        mat[:,n]=data_x[n,:,:].flatten(flat_type);
    return mat;
    
    
def dataset_list_to_pixel_dataset_list(data_x_list,data_y_list):
    dataset_x_res=[];
    dataset_y_res=[];
    for data_x,data_y in zip(data_x_list,data_y_list):
        flat_type='F';
        '''
        Sz=np.shape(data_x);
        mat=np.zeros((Sz[1]*Sz[2],Sz[0]));
        for n in range(Sz[0]):
            mat[:,n]=data_x[n,:,:].flatten(flat_type);
        '''
        mat=dataset_x_to_pixel_dataset_x(data_x, flat_type);
        
        dataset_x_res.append(mat);
        dataset_y_res.append(data_y.flatten(flat_type));
    return dataset_x_res,dataset_y_res;

def plot_pixel_dataset_list(data_x_list,data_y_list,percent=0.01,png_filepath='scatter3d.png'):
    dataset_x=[];
    dataset_y=[];
    n=0;
    Ltot=0;
    L=0;
    for data_x,data_y in zip(data_x_list,data_y_list):
        Sz=np.shape(data_x);
        Lpart=int(Sz[0]*percent);
        idx = np.random.randint(Sz[0], size=Lpart);
        Ltot=Ltot+Sz[0];
        L=L+Lpart;
        if(n==0):
            dataset_x=data_x[idx,:];
            dataset_y=data_y[idx];
        else:
            dataset_x=np.concatenate((dataset_x,data_x[idx,:]),axis=0)
            dataset_y=np.concatenate((dataset_y,data_y[idx]),axis=0)
        n=n+1
    
    #print('np.shape(dataset_x):',np.shape(dataset_x))
    #print('np.shape(dataset_y):',np.shape(dataset_y))
    
    title='Plotting '+str(L)+' samples of total of '+str(Ltot);
    # Creating figure
    fig = plt.figure(figsize = (10, 7))
    ax = plt.axes(projection ="3d")
    sctt=ax.scatter(dataset_x[:,0],dataset_x[:,1],dataset_x[:,2], c = dataset_y, marker='o')
    ax.set_xlabel('Avd' , fontweight ='bold')
    ax.set_ylabel('Mean', fontweight ='bold')
    ax.set_zlabel('Std' , fontweight ='bold')
    ax.set_title(title, fontweight ='bold');
    fig.colorbar(sctt, ax = ax, shrink = 0.5, aspect = 5)
    plt.savefig(png_filepath);
    plt.show()

