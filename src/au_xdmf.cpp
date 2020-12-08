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

#include "au_xdmf.h"

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

    std::ostringstream dimensions_str;
    if (!dimensions.empty())
    {
        // Convert all but the last element to avoid a trailing ","
        std::copy(dimensions.begin(), std::prev(dimensions.end(), 1),
                  std::ostream_iterator<unsigned long long>(dimensions_str, " "));

        // Now add the last element with no delimiter
        dimensions_str << dimensions.back();
    }
    std::cout << "dimensions_str = " << dimensions_str.str() << "\n";
    if (dimensions.size() == 2)
    {
        fprintf(fp, topology, dimensions_str.str().c_str(), topology_type_2d);
        fprintf(fp, geometry, geometry_type_2d, 2, geometry_origin_2d, 2, geometry_dxdy_2d);
    }
    else
    {
        fprintf(fp, topology, dimensions_str.str().c_str(), topology_type_3d);
        fprintf(fp, geometry, geometry_type_3d, 3, geometry_origin_3d, 3, geometry_dydydz_3d);
    }

    fprintf(fp, attribute_dataitem, xdmf_dset_name.c_str(), dimensions_str.str().c_str(), hdf5_file_name.c_str(), dset_name.c_str());

    fputs(footer, fp);
    fclose(fp);

    return 0;
}