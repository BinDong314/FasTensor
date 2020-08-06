
/**
 *
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include "au.h"

using namespace std;
using namespace AU;

int rows_chunk = 11648;
int cols_chunk = 60000;

//UDF One: duplicate the original data
inline Stencil<double> udf_kf1p(const Stencil<short> &iStencil)
{
    Stencil<double> oStencil;
    double oDouble = iStencil(0, 0) * 1.0;
    oStencil = oDouble;
    return oStencil;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    // 11648, 30000 for each dataset
    std::vector<int> chunk_size(2);
    chunk_size[0] = rows_chunk;
    chunk_size[1] = cols_chunk;
    std::vector<int> overlap_size = {0, 0};

    //Input data
    Array<short> *A = new Array<short>("EP_DIR:EP_TDMS:/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/examples/das/tdms-dir", chunk_size, overlap_size);
    std::vector<std::string> aug_merge_index, aug_dir_sub_cmd, aug_input_search_rgx;

    aug_merge_index.push_back("1");
    aug_dir_sub_cmd.push_back("BINARY_ENABLE_TRANSPOSE_ON_READ");
    aug_input_search_rgx.push_back("^(.*)[135]\\.tdms$");

    A->EndpointControl(DIR_MERGE_INDEX, aug_merge_index);
    //A->EndpointControl(DIR_SUB_CMD_ARG, aug_dir_sub_cmd1); //Not needed
    A->EndpointControl(DIR_SUB_CMD_ARG, aug_dir_sub_cmd);
    A->EndpointControl(DIR_INPUT_SEARCH_RGX, aug_input_search_rgx);

    //Result data
    //Array<double> *B = new Array<double>("EP_DIR:EP_TDMS:/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/examples/das/tdms-dir-dec");

    //Array<double> *B = new Array<double>("EP_HDF5:./tdms-dir-dec/test.h5:/DataCT");

    //http://www.cplusplus.com/reference/regex/ECMAScript/
    Array<double> *B = new Array<double>("EP_DIR:EP_HDF5:./tdms-dir-dec/:/DataCT");

    std::vector<std::string> aug_output_replace_arg;
    aug_output_replace_arg.push_back("^(.*)\\.tdms$");
    aug_output_replace_arg.push_back("$1.h5");
    B->EndpointControl(DIR_MERGE_INDEX, aug_merge_index);
    B->EndpointControl(DIR_OUPUT_REPLACE_RGX, aug_output_replace_arg);
    //Run
    A->Apply(udf_kf1p, B);

    //Clear
    delete A;
    delete B;

    AU_Finalize();

    return 0;
}
