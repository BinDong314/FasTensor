#include <vector>
#include <mpi.h>
#include "ga.h"
#include  "macdecls.h"
#include <assert.h>     /* assert */


#define SIZE  16
#define CHUNK 4
//#define TWO_GAS 1
#define READ_GHOST 1

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  GA_Initialize();
  int heap=30000, stack=30000;
  MA_init(C_INT, stack, heap);
  //if(!) GA_Error("MA_init failed",stack+heap);
  
  int me = GA_Nodeid();
  int nproc = GA_Nnodes();
  if (me == 0) {
    printf("Running on %d processors\n\n",nproc);
  }
 
  int64_t dims[2];
  dims[0] = SIZE;
  dims[1] = SIZE;
  int ndim = 2;
  int64_t width[2];
  width[0] = 1;
  width[1] = 1;

  int64_t chunk[2];
  chunk[0] = CHUNK;
  chunk[1] = CHUNK;

  int size_temp;

  
  std::vector<int> A_data, B_data;
  //for (int i = 0; i < CHUNK*CHUNK; i++) {A_data[i]=me; B_data[i] = me + 16;}

  std::vector<int64_t> A_dist_lo, A_dist_hi, B_dist_lo, B_dist_hi, ld;
  A_dist_lo.resize(ndim);  A_dist_hi.resize(ndim);  B_dist_lo.resize(ndim);  B_dist_hi.resize(ndim);
  ld.resize(ndim-1); 

  // Create GA with ghost cells
  int g_A = NGA_Create_ghosts64(C_INT,ndim, dims, width,"array A", &(chunk[0]));
  NGA_Distribution64(g_A, me, &(A_dist_lo[0]), &(A_dist_hi[0]));
  if(me < 2) printf("Ater create A & B:  A: ga_rank=%d, dist_lo=(%lld,%lld), dist_hi=(%lld,%lld)\n",me, A_dist_lo[0],A_dist_lo[1],A_dist_hi[0],A_dist_hi[1]); fflush(stdout);
  
#ifdef TWO_GAS
  int g_B = NGA_Create_ghosts64(C_INT,ndim, dims, width,"array B", &(chunk[0]));
  NGA_Distribution64(g_B, me, &(B_dist_lo[0]), &(B_dist_hi[0]));
  if(me < 2) printf("Ater create A & B:  B: ga_rank=%d, dist_lo=(%lld,%lld), dist_hi=(%lld,%lld)\n",me, B_dist_lo[0],B_dist_lo[1],B_dist_hi[0],B_dist_hi[1]); fflush(stdout);
#endif


  A_data.resize(CHUNK*CHUNK);
  
  //repeated "write/update ghost cell/read" on A
  for (int ite = 0; ite < 3; ite++){
    for (int i = 0; i < CHUNK*CHUNK; i++) {A_data[i]=i+me+ite;}
    
    //Write Data to A;
    ld[0] = A_dist_hi[0] -  A_dist_lo[0] + 1;
    NGA_Put64(g_A,  static_cast<int64_t *>(A_dist_lo.data()),  static_cast<int64_t *>(A_dist_hi.data()), static_cast<void*>(A_data.data()),  static_cast<int64_t *>(ld.data()));
    GA_Sync(); 	  
    GA_Update_ghosts(g_A);
    if(me < 2) printf("Ater updatin ghosts on A:  A: ga_rank=%d, dist_lo=(%lld,%lld), dist_hi=(%lld,%lld)\n",me, A_dist_lo[0], A_dist_lo[1], A_dist_hi[0], A_dist_hi[1]); fflush(stdout);

    //To read chunk with ghost from A
    //Figure out the right size : skip out-of-range data
    size_temp = 1;
    for(int j = 0 ; j < ndim; j++) {
      assert( (A_dist_hi[j] - A_dist_lo[j] + 1) == CHUNK);
#ifdef READ_GHOST
      if(A_dist_hi[j]+1 < SIZE){
        A_dist_hi[j]=A_dist_hi[j]+1;
      }
      
      if (A_dist_lo[j]-1 >= 0){
        A_dist_lo[j]=A_dist_lo[j]-1;
      }
#endif
      size_temp = size_temp *  (A_dist_hi[j] -  A_dist_lo[j] + 1);
    }
    
    ld[0] = A_dist_hi[0] -  A_dist_lo[0] + 1;
    A_data.resize(size_temp);
    NGA_Get_ghost_block64(g_A, static_cast<int64_t *>(A_dist_lo.data()),  static_cast<int64_t *>(A_dist_hi.data()), static_cast<void*>(A_data.data()),  static_cast<int64_t *>(ld.data()));
    if(me < 2) { printf("Data read from A after ghost update on rank %d, size_temp = %d, lo = (%lld, %lld), hi=(%lld, %lld) : ", me,size_temp, A_dist_lo[0], A_dist_lo[1], A_dist_hi[0], A_dist_hi[1]); for(int k = 0 ; k < size_temp; k++) printf(" %d, ", A_data[k]);  printf("\n"); fflush(stdout);}
    
    NGA_Distribution64(g_A, me, &(A_dist_lo[0]), &(A_dist_hi[0]));
    if(me < 2) printf("Ater read ghosts on A:  A: ga_rank=%d, dist_lo=(%lld,%lld), dist_hi=(%lld,%lld)\n",me, A_dist_lo[0], A_dist_lo[1], A_dist_hi[0], A_dist_hi[1]); fflush(stdout);
 
  }
  
  
  ////////////////////////////////////////////////////
  //Here we do some computing from A_data to B_data //
  //For test: we just write B_data directly         //
  ///////////////////////////////////////////////////

#ifdef TWO_GAS
  B_data.resize(CHUNK*CHUNK); 
  for (int i = 0; i < CHUNK*CHUNK; i++) {B_data[i] = me + 16;}
  ld[0]= B_dist_hi[0] - B_dist_lo[0] + 1;
  NGA_Put64(g_B,  &(B_dist_lo[0]),  &(B_dist_hi[0]), static_cast<void*>(B_data.data()),  &(ld[0]));
  GA_Update_ghosts(g_B);
  NGA_Distribution64(g_B, me, &(B_dist_lo[0]), &(B_dist_hi[0]));

  size_temp = 1;
  for(int j = 0 ; j < ndim; j++) {
    assert( (B_dist_hi[j] - B_dist_lo[j] + 1) == CHUNK);
#ifdef READ_GHOST
    if(B_dist_hi[j]+1 < SIZE){
      B_dist_hi[j]=B_dist_hi[j]+1;
    }

    if (B_dist_lo[j]-1 >= 0){
      B_dist_lo[j]=B_dist_lo[j]-1;
    }
#endif
    size_temp = size_temp *  (B_dist_hi[j] -  B_dist_lo[j] + 1);
  }
  
  ld[0] = B_dist_hi[0] -  B_dist_lo[0] + 1;
  B_data.resize(size_temp);
  NGA_Get_ghost_block64(g_B, &(B_dist_lo[0]),  &(B_dist_hi[0]), static_cast<void*>(B_data.data()),  &(ld[0]));
  if(me < 2) { printf("Data read from B after ghost update on rank %d: ", me); for(int k = 0 ; k < size_temp; k++) printf(" %d, ", B_data[k]);  printf("\n"); fflush(stdout);}
  
  NGA_Distribution64(g_B, me, &(B_dist_lo[0]), &(B_dist_hi[0]));
  if(me < 2) printf("Ater read ghosts on B:  B: ga_rank=%d, dist_lo=(%lld,%lld), dist_hi=(%lld,%lld)\n",me, B_dist_lo[0],B_dist_lo[1],B_dist_hi[0],B_dist_hi[1]); fflush(stdout);
  GA_Destroy(g_B);
#endif

  GA_Destroy(g_A);
  GA_Terminate();
  MPI_Finalize();
}
