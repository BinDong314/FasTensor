'''A simple example of building a virtual dataset.

This makes four 'source' HDF5 files, each with a 1D dataset of 100 numbers.
Then it makes a single 4x100 virtual dataset in a separate file, exposing
the four sources as one dataset.
'''

import h5py
import numpy as np
import getopt, sys
import glob
import ntpath
import os


def usage():
    print('Usage: '+sys.argv[0]+' -i <input directory containing hdf5 files> -o <output hdf5 file> -d datasest -g group -f filter')


def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "ho:i:d:g:f:", ["help", "output=", "input=", "dataset=", "group=", "filter="])
    except getopt.GetoptError as err:
        # print help information and exit:
        print(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    input_directory=''
    output_hdf_file=''
    hdf_group='/'
    hdf_dataset='DataByChannelTime'
    filter_pattern='*.h5*'
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-o", "--output"):
            output_hdf_file = a
        elif o in ("-i", "--input"):
            input_directory = a
        elif o in ("-g", "--group"):
            hdf_group =  a
        elif o in ("-d", "--dataset"):
            hdf_dataset =  a
        elif o in ("-f", "--filter"):
            filter_pattern =  a
        else:
            assert False, "unhandled option"

    #https://docs.python.org/2/library/glob.html
    h5_file  = glob.glob(os.path.join(input_directory, filter_pattern))
    print(h5_file[0])
    h5_shape = h5py.File(h5_file[0], 'r')[hdf_dataset].shape # get the first ones shape.
    h5_file_count = len(h5_file)
    print('Assume each file has shape as: ', h5_shape)
    print('Total files: ', h5_file_count)
    
    layout = h5py.VirtualLayout(shape=(h5_shape[0]*h5_file_count, h5_shape[1]), dtype='i2')
    n=0
    for filename in sorted(h5_file):
        print('merging file: ', filename)
        vsource = h5py.VirtualSource(filename, hdf_dataset, shape=h5_shape, dtype='i2')
        layout[n*h5_shape[0]:(n+1)*h5_shape[0], :] = vsource
        n += 1

        # Add virtual dataset to output file
    with h5py.File(output_hdf_file, 'w', libver='latest') as f:
        f.create_virtual_dataset(hdf_dataset, layout, fillvalue=0)


if __name__ == "__main__":
    main()
