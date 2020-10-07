/**
 *
 * Author:  Bin Dong, dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include "ft.h"

using namespace std;
using namespace FT;

int main(int argc, char *argv[])
{
    //Init the ArrayUDF, MPI, etc.
    AU_Init(argc, argv);

    cout << "Hello word!" << endl;

    AU_Finalize();

    return 0;
}
