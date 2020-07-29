/**
   * @brief update the seek_offset
   * 
*/
#include "au_endpoint_tdms.h"

void EndpointTDMS::UpdateSeekOffset()
{

    uint64_t nByte_metadata;
    FILE *fp = this->GetFP();
    fseek(fp, 20, SEEK_SET);
    fread(&nByte_metadata, sizeof(uint64_t), 1, fp);
    long int seek_offset = nByte_metadata + NBYTE_LEADIN;

    this->SetSeekOffset(seek_offset);

    //printf("TDMS header: %ld (%d fixed + %lld user's)\n", seek_offset, NBYTE_LEADIN, nByte_metadata);
}