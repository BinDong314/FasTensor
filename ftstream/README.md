# FTStream

FTStream is a tool for simulating streams of HDF5 files containing arrays filled with random values or based on a seed file at different granularity.

## Installation

Ensure GNU Autotools are present on your system.

To install FTStream, follow these steps:

1. Run `autoreconf -i` to generate the configure script.

2. Configure the project with the following command, replacing 

   ```
   /path/to/hdfsrc/build/
   ```

    with the actual path to your HDF5 source build directory:

   ```
   ./configure --with-hdf5=/path/to/hdfsrc/build/ CXX=mpic++ CC=mpic
   ```
   
3. Build the project by running `make`.

4. Set the `LD_PRELOAD` environment variable to the paths of  `libhdf5.so` `libhdf5_hl.so`

    libraries:
   
   ```
   export LD_PRELOAD=/path/to/libhdf5.so:/path/to/libhdf5_hl.so
   ```

```
autoreconf -i 
./configure --with-hdf5=/path/to/hdfsrc/build/ CXX=mpic++ CC=mpic 
make
export LD_PRELOAD=/path/to/libhdf5.so:/path/to/libhdf5_hl.so
```

## Usage

FTStream provides different modes of operation for creating streams of HDF5 files.

### Random Stream

To create a stream of HDF5 files containing arrays filled with random values, use the following command:

```
./ftstream random --directory="/home/mydir" --count=10 --interval=2
```

This will create a stream of 10 HDF5 files in the directory `/home/mydir`, with a new file generated every 2 seconds.

You can also specify the dimensions of the array using the `--dimension` option:

```
./ftstream random --directory="/home/mydir" --count=10 --interval=2 --dimension=4096,4096
```

This will create a stream of 10 HDF5 files, each containing a 4096x4096 array filled with random values.

### Playback Stream

To create an infinite stream of HDF5 files based on a seed file, use the following command:

```
./ftstream playback --seed-file="/path/to/seed/file" --count 0 --interval 10
```

This will create an infinite stream of HDF5 files in the current working directory of FTStream, with a new file generated every 10 seconds based on the provided seed file.

### Parallel File Systems(Lustre):
You might need to comment out O_DIRECT mode.
