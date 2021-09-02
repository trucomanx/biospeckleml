import sys
sys.path.append("extras")
import files_module     as flmod

dirs=flmod.scan_all_subdirs("/home/fernando/Downloads/biospeckleml/source");

[print(dirpath) for dirpath in dirs ]

