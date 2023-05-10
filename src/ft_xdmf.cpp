/*
****************************

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy).
All rights reserved.

If you have questions about your rights to use or distribute this software,
please contact Berkeley Lab's Intellectual Property Office at
IPO@lbl.gov.

NOTICE.  This Software was developed under funding from the U.S. Department
of Energy and the U.S. Government consequently retains certain rights.  As
such, the U.S. Government has been granted for itself and others acting on
its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the
Software to reproduce, distribute copies to the public, prepare derivative
works, and perform publicly and display publicly, and to permit others to do so.


****************************


*** License Agreement ***

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy).
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley
National Laboratory, U.S. Dept. of Energy nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches,
or upgrades to the features, functionality or performance of the source
code ("Enhancements") to anyone; however, if you choose to make your
Enhancements available either publicly, or directly to Lawrence Berkeley
National Laboratory, without imposing a separate written license agreement
for such Enhancements, then you hereby grant the following license: a
non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software,
distribute, and sublicense such enhancements or derivative works thereof,
in binary and source code form.
*/

/**
 *
 * Author: Bin Dong dbin@lbl.gov
 * Web: https://crd.lbl.gov/bin-dong
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

/*
<?xml version="1.0"?>
<!DOCTYPE Xdmf SYSTEM "Xdmf.dtd" []>
<Xdmf xmlns:xi="http://www.w3.org/2001/XInclude" Version="2.0">
<Domain>
<Grid GridType="Uniform">
<Topology Dimensions="16 16" TopologyType="2DCoRectMesh"/>
<Geometry Type="ORIGIN_DXDY">
<DataItem Dimensions="2" Format="XML">0.000000 0.000000</DataItem>
<DataItem Dimensions="2" Format="XML">1.000000 1.000000</DataItem>
</Geometry>
<Attribute AttributeType ="Scalar" Center="Node"  Name="dat">
<DataItem ItemType="Uniform" Dimensions="16 16" DataType="Float" Precision="4" Format="HDF">
    tutorial.h5:/dat
</DataItem>
</Attribute>
</Grid>
</Domain>
</Xdmf>
*/

const char *header = "\
<?xml version=\"1.0\"?>\n\
<!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\" []>\n\
<Xdmf xmlns:xi=\"http://www.w3.org/2001/XInclude\" Version=\"2.0\">\n\
<Domain>\n\
<Grid GridType=\"Uniform\">\n";

const char *topology_type_2d = "2DCoRectMesh";
const char *topology_type_3d = "3DCoRectMesh";

const char *topology = "<Topology Dimensions=\"%s\" TopologyType=\"%s\"/>\n";

const char *geometry_type_2d = "ORIGIN_DXDY";
const char *geometry_type_3d = "ORIGIN_DXDYXZ";
const char *geometry_origin_2d = "0.000000 0.000000";
const char *geometry_origin_3d = "0.000000 0.000000 0.000000";

const char *geometry_dxdy_2d = "1.000000 1.000000";
const char *geometry_dydydz_3d = "1.000000 1.000000 1.000000";

const char *geometry = "\
<Geometry Type=\"%s\"> \n\
<DataItem Dimensions=\"%d\" Format=\"XML\">%s</DataItem> \n\
<DataItem Dimensions=\"%d\" Format=\"XML\">%s</DataItem> \n\
</Geometry> \n\
";

const char *attribute_dataitem = "\
<Attribute AttributeType =\"Scalar\" Center=\"Node\"  Name=\"%s\">   \n\
<DataItem ItemType=\"Uniform\" Dimensions=\"%s\" DataType=\"Float\" Precision=\"4\" Format=\"HDF\"> \n\
    %s:%s  \n\
</DataItem>   \n\
</Attribute>  \n\
";

const char *footer = "\
</Grid> \n\
</Domain>\n\
</Xdmf>";

#include "ft_xdmf.h"

int create_xdmf(std::string file_name, std::string dset_name, std::vector<unsigned long long> dimensions, FTType data_element_type)
{
    PrintVector("dimensions = ", dimensions);
    if (!(dimensions.size() != 2 || dimensions.size() != 3))
    {
        AU_EXIT("create_xdmf only supports 2D and 3D data !\n");
    }

    std::string xdmf_file_name = file_name + ".xdmf";
    std::string hdf5_file_name = ExtractFileName(file_name);

    std::string xdmf_dset_name = ExtractFileName(dset_name);
    std::cout << " xdmf_file_name = " << xdmf_file_name << "\n";
    std::cout << " xdmf_dset_name = " << xdmf_dset_name << "\n";

    FILE *fp = fopen(xdmf_file_name.c_str(), "w");
    fputs(header, fp);

    // std::ostringstream dimensions_str;
    // if (!dimensions.empty())
    // {
    //     // Convert all but the last element to avoid a trailing ","
    //     std::copy(dimensions.begin(), std::prev(dimensions.end(), 1),
    //               std::ostream_iterator<unsigned long long>(dimensions_str, " "));

    //     // Now add the last element with no delimiter
    //     dimensions_str << dimensions.back();
    // }
    // std::cout << "dimensions_str = " << dimensions_str.str() << "\n";

    std::stringstream ss;
    for (const auto &n : dimensions)
    {
        ss << n << " ";
    }
    std::string dimensions_str = ss.str();

    if (dimensions.size() == 2)
    {
        fprintf(fp, topology, dimensions_str.c_str(), topology_type_2d);
        fprintf(fp, geometry, geometry_type_2d, 2, geometry_origin_2d, 2, geometry_dxdy_2d);
    }
    else
    {
        fprintf(fp, topology, dimensions_str.c_str(), topology_type_3d);
        fprintf(fp, geometry, geometry_type_3d, 3, geometry_origin_3d, 3, geometry_dydydz_3d);
    }

    fprintf(fp, attribute_dataitem, xdmf_dset_name.c_str(), dimensions_str.c_str(), hdf5_file_name.c_str(), dset_name.c_str());

    fputs(footer, fp);
    fclose(fp);

    return 0;
}