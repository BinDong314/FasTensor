#ifndef AU_VIS_H
#define AU_VIS_H

typedef enum FTVisType
{
    FT_VIS_NONE = -1,
    FT_VIS_XDMF = 0,   //from/to HDF5
    EP_VIS_PYTHON = 1, //from/to NETCDF; todo
    EP_VIS_R = 2,      //from/to NETCDF; todo
    EP_NCLASSES
} FTVisType;

#endif