import h5py
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

#HDF5 "westSac_merged_arrayudf.h5p" {
#GROUP "/" {
#   DATASET "DataByChannelTime" {
#      DATATYPE  H5T_IEEE_F32LE
#      DATASPACE  SIMPLE { ( 120, 11648 ) / ( 120, 11648 ) }
#   }
#}
#}

filename='westSac_merged_arrayudf.h5p'
file    = h5py.File(filename, 'r')   # 'r' means
dataset = file['/DataByChannelTime']



print(dataset.shape)
fig = plt.figure()
plt.xlabel('Channel');
plt.ylabel('Time');
plt.imshow(dataset, aspect='auto');
plt.colorbar()
plt.show()
with PdfPages('westSac_merged_arrayudf.pdf') as pdf:
    pdf.savefig(fig)

