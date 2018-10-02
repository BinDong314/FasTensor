#!/usr/bin/python

import sys
import h5py
from nptdms import TdmsFile
import numpy as np
import getopt, sys
import glob
import ntpath
import os
        
def usage():
  print('Usage: '+sys.argv[0]+' -i <input tdms file or directory in -b model> -o <output hdf file or directory in -b model>  [-g hdf group (root by defualt), -d hdf dataset (DataByChannelTime by defualt) -b bath model]')
def tdms2ht(input_tdms_file, output_hdf_file, hdf_dataset):
    tdms_file = TdmsFile(input_tdms_file)
    df=tdms_file.as_dataframe()
    f = h5py.File(output_hdf_file, "w")
    f.create_dataset(hdf_dataset, data=df.values, dtype='f4')
    f.close()

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "ho:i:g:d:b", ["help", "output=", "input=", "group=", "dataset=", "bath"])
    except getopt.GetoptError as err:
        # print help information and exit:
        print(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    input_tdms_file=''
    output_hdf_file=''
    hdf_group='/'
    hdf_dataset="DataByChannelTime"
    bath_mode=False
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-o", "--output"):
            output_hdf_file = a
        elif o in ("-i", "--input"):
            input_tdms_file = a
        elif o in ("-g", "--group"):
            hdf_group =  a
        elif o in ("-d", "--dataset"):
            hdf_dataset =  a
        elif o in ("-b", "--bath"):
            bath_mode =  True
        else:
            assert False, "unhandled option"
    if bath_mode == False:
        tdms2ht(input_tdms_file, output_hdf_file, hdf_dataset);
    else:
        for tdms_file in glob.glob(os.path.join(input_tdms_file, '*.tdms')):
            tdms2ht(tdms_file, os.path.join(output_hdf_file, os.path.splitext(ntpath.basename(tdms_file))[0]+'.h5p'), hdf_dataset)
            
if __name__ == "__main__":
    main()
    
