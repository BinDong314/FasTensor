import h5py
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages


filename='westSac_merged_arrayudf.h5'
file    = h5py.File(filename, 'r')   # 'r' means
dataset = file['/DataTimeChannel']



print(dataset.shape)
fig = plt.figure()
plt.xlabel('Channel');
plt.ylabel('Time');
plt.imshow(dataset, aspect='auto');
plt.colorbar()
plt.show()
with PdfPages('westSac_merged_arrayudf.pdf') as pdf:
    pdf.savefig(fig)

