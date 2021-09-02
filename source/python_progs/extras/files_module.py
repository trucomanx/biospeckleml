import os

def scan_all_subdirs(dirname):
    subfolders= [f.path for f in os.scandir(dirname) if f.is_dir()]
    for dirname in list(subfolders):
        subfolders.extend(scan_all_subdirs(dirname))
    return subfolders
