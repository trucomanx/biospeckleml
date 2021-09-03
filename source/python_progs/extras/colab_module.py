import os

def download_all_files_in(url_files,directory='dataset'):
  if not os.path.exists(directory):
    os.mkdir(directory)
  n=0;
  dataset_files=[];
  for url_file in url_files:
    basename=os.path.basename(url_file)
    output_filepath=os.path.join(directory,basename);
    !wget $url_file -O $output_filepath 
    dataset_files.append(output_filepath);
    n=n+1
  return dataset_files
