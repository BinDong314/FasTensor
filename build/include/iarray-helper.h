#ifndef  __IARRAY_HELP_H__
#define  __IARRAY_HELP_H__

#include <mpi.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unistd.h> 
#include <cmath>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>




//------------------------------------------------------------------------------
// Local array layout information
// Need to have a row-major order
struct iArrayLocal{
  int  rank;
  std::vector<int> count;    //Size for each dim
  std::vector<int> offset;    //Starting point for each dim
  std::vector<int> rowStride; // rowStride = count 
   
 iArrayLocal(std::vector<int> c, std::vector<int> rs) :
  count(c), rowStride( rs )
  {
    rank=c.size();
    offset.resize(rank);
    for(int i = 0; i < rank; i++) offset[i] = 0;
  }

 iArrayLocal(std::vector<int> c, std::vector<int> rs, std::vector<int> os)
  {
    count = c;
    rowStride = rs;
    offset = os;
    rank = c.size();
  }

  iArrayLocal() 
  {}
};

inline std::ostream& operator<<( std::ostream& os, const iArrayLocal& a ) {
  int rank = a.count.size();
  os <<  "offset: ";
  for(int i = 0; i < rank; i++)
    os << a.offset[i] << ", ";
  os<<std::endl;
  
  os << "count : ";
  for(int i = 0; i < rank; i++)
    os << a.count[i] << ", ";
  os<<std::endl;

  os<<"rowStride : ";
  for(int i = 0; i < rank; i++)
    os << a.rowStride[i] << ", ";
  os<<std::endl;
  
  return os;
}

//------------------------------------------------------------------------------
// Accessor for 2D arrays: Allow for random access to 2D domain given pointer 
// and layout information 
template < typename T >
class iArrayLocalAccessor {
public:
  const T& operator()( std::vector<int> coordinate) const {
    //layout_.offset =  0;
    size_t data_offset;
    for(int i = 0 ; i < layout_.rank ; i++){
      coordinate[i] =  coordinate[i] + layout_.offset[i];
    }
    data_offset = coordinate[0];
    for(int i = 0 ; i < layout_.rank -1; i++)
      data_offset = data_offset* layout_.rowStride[i+1] + coordinate[i+1];
    return *( data_ + data_offset ); 
  }
  T& operator()( std::vector<int> coordinate) {
    //layout_.offset =  0;
    size_t data_offset;
    for(int i = 0 ; i < layout_.rank ; i++){
      coordinate[i] =  coordinate[i] + layout_.offset[i];
    }
    data_offset = coordinate[0];
    for(int i = 0 ; i < layout_.rank -1; i++)
      data_offset = data_offset* layout_.rowStride[i+1] + coordinate[i+1];

    //printf("current offset = %u, value = %f \n", data_offset, *(data_ + data_offset) );
    return *( data_ + data_offset ); 
    
  }
  
  iArrayLocalAccessor() : data_( 0 ) {}
  iArrayLocalAccessor( T* data, const iArrayLocal& layout ) : data_( data ), layout_( layout) {}
  const iArrayLocal& Layout() const { return layout_; }
private:
  iArrayLocal layout_;
  T* data_;
};

//------------------------------------------------------------------------------
// Region ids: Used to identify areas in the local grid

enum RegionID {
  //For 1D data
  LEFT_1D,    CENTER_1D,  RIGHT_1D,  
  //For 2D data's receive on Core+ghost zone
  TOP_LEFT_2D,    TOP_CENTER_2D,    TOP_RIGHT_2D, 
  CENTER_LEFT_2D,   CENTER_2D,        CENTER_RIGHT_2D,
  BOTTOM_LEFT_2D,   BOTTOM_CENTER_2D, BOTTOM_RIGHT_2D,
  //For 2D data's send on Core data
  TOP_2D,    LEFT_2D,   BOTTOM_2D,        RIGHT_2D,
  
  //for 3D data's receive on Core+ghost zone
  TOP_LEFT_3D,   TOP_CENTER_TOP_3D,   TOP_RIGHT_3D,   TOP_FRONT_3D,   TOP_BACK_3D,   TOP_LEFT_FRONT_3D,   TOP_LEFT_BACK_3D,   TOP_RIGHT_FRONT_3D, TOP_RIGHT_BACK_3D, 
  CENTER_LEFT_3D, CENTER_3D,      CENTER_RIGHT_3D, CENTER_FRONT_3D, CENTER_BACK_3D, CENTER_LEFT_FRONT_3D, CENTER_LEFT_BACK_3D, CENTER_RIGHT_FRONT_3D, CENTER_RIGHT_BACK_3D,
  BOTTOM_LEFT_3D, BOTTOM_CENTER_BOTTOM_3D, BOTTOM_RIGHT_3D, BOTTOM_FONT_3D, BOTTOM_BACK_3D, BOTTOM_LEFT_FRONT_3D,BOTTOM_LEFT_BACK_3D, BOTTOM_RIGHT_FRONT_3D,BOTTOM_RIGHT_BACK_3D,

  //for 3D data's send on Core data
  TOP_MIDDLE_3D_SEND, TOP_RIGHT_3D_SEND,   TOP_LEFT_3D_SEND,   TOP_BACK_3D_SEND,   TOP_FRONT_3D_SEND,
  CENTER_FRONT_MIDDLE_3D_SEND, CENTER_BACK_MIDDLE_3D_SEND, CENTER_LEFT_MIDDLE_3D_SEND, CENTER_RIGHT_MIDDLE_3D_SEND,
  CENTER_FRONT_LEFT_3D_SEND,    CENTER_FRONT_RIGHT_3D_SEND,  CENTER_BACK_LEFT_3D_SEND, CENTER_BACK_RIGHT_3D_SEND,
  BOTTOM_MIDDLE_3D_SEND, BOTTOM_RIGHT_3D_SEND,   BOTTOM_LEFT_3D_SEND,   BOTTOM_BACK_3D_SEND,   BOTTOM_FRONT_3D_SEND,
 
};




//------------------------------------------------------------------------------
// MPI grid cell  ids: Used to identify neighbors in the global MPI cartesian grid
enum MPIGridCellID {
  //for 1D
  MPI_LEFT_1D,    MPI_CENTER_1D,    MPI_RIGHT_1D,
  //for 2D
  MPI_TOP_LEFT_2D,    MPI_TOP_CENTER_2D,    MPI_TOP_RIGHT_2D,
  MPI_CENTER_LEFT_2D,                       MPI_CENTER_RIGHT_2D,
  MPI_BOTTOM_LEFT_2D, MPI_BOTTOM_CENTER_2D, MPI_BOTTOM_RIGHT_2D,
  //for 3D data
  MPI_TOP_LEFT_3D,   MPI_TOP_CENTER_TOP_3D,   MPI_TOP_RIGHT_3D,   MPI_TOP_FRONT_3D,   MPI_TOP_BACK_3D,   MPI_TOP_LEFT_FRONT_3D,   MPI_TOP_LEFT_BACK_3D,   MPI_TOP_RIGHT_FRONT_3D, MPI_TOP_RIGHT_BACK_3D, 
  MPI_CENTER_LEFT_3D, MPI_CENTER_3D,      MPI_CENTER_RIGHT_3D, MPI_CENTER_FRONT_3D, MPI_CENTER_BACK_3D, MPI_CENTER_LEFT_FRONT_3D, MPI_CENTER_LEFT_BACK_3D, MPI_CENTER_RIGHT_FRONT_3D, MPI_CENTER_RIGHT_BACK_3D,
  MPI_BOTTOM_LEFT_3D, MPI_BOTTOM_CENTER_BOTTOM_3D, MPI_BOTTOM_RIGHT_3D, MPI_BOTTOM_FONT_3D, MPI_BOTTOM_BACK_3D, MPI_BOTTOM_LEFT_FRONT_3D,MPI_BOTTOM_LEFT_BACK_3D, MPI_BOTTOM_RIGHT_FRONT_3D, MPI_BOTTOM_RIGHT_BACK_3D
};

//------------------------------------------------------------------------------
// Print content of 2D array given pointer to data and layout info
template < typename T >
void PrintiArrayLocal( T* pdata, const iArrayLocal& g, std::ostream& os ) {
  iArrayLocalAccessor< T > a( pdata, g);
  /*for( int row = 0; row != a.Layout().height; ++row ) {
        for( int column = 0; column != a.Layout().width; ++column ) {
            os << a( column, row ) << ' ';

        }
        os << std::endl;
        }*/
}

//------------------------------------------------------------------------------
//Compute Array2D layout from region id of local grid
//Possible to use templated function specialized with RegionID
inline iArrayLocal SubArrayRegion( const iArrayLocal& g, 
                                   std::vector<int> ghost_size, 
                                   RegionID rid ) {
  std::vector<int> offset; offset.resize(g.rank);
  std::vector<int> count; count.resize(g.rank);
  std::vector<int> stride; stride.resize(g.rank);

  switch( rid ) {
    case   LEFT_1D: //for 1D
      offset=g.offset;
      count=ghost_size;
      break;
    case CENTER_1D:
      offset[0]=g.offset[0] + ghost_size[0];
      count[0]=g.count[0] -  2*ghost_size[0];
      break;
    case  RIGHT_1D:
      offset[0]=g.offset[0] + g.count[0] - ghost_size[0];
      count[0]=ghost_size[0];
      break;
    case TOP_LEFT_2D: //for 2D
      offset=g.offset;
      count=ghost_size;
      break;
    case TOP_CENTER_2D:
      count[0] = ghost_size[0];
      count[1] = g.count[1] -  2 * ghost_size[1];
      offset[0] = g.offset[0];
      offset[1] = g.offset[1]+ghost_size[1];
      break;
    case TOP_RIGHT_2D:
      count = ghost_size;
      offset[0]=g.offset[0];
      offset[1]=g.offset[1]+g.count[1]-ghost_size[1];
      break;
    case CENTER_LEFT_2D:
      count[0]  = g.count[0]-2*ghost_size[0];
      count[1]  = ghost_size[1];
      offset[0] = g.offset[0]+ghost_size[0];
      offset[1] = g.offset[1];
      break;
    case CENTER_2D: //core space
      count[0] = g.count[0] - ghost_size[0] * 2;
      count[1] = g.count[1] - ghost_size[1] * 2;
      offset[0] = g.offset[0] + ghost_size[0];
      offset[1] = g.offset[1] + ghost_size[1];
      break;
    case CENTER_RIGHT_2D:
      count[0]  = g.count[0]-2*ghost_size[0];
      count[1]  = ghost_size[1];
      offset[0] = g.offset[0]+ghost_size[0];
      offset[1] = g.offset[1]+g.count[1]-ghost_size[1];
      break;
    case BOTTOM_LEFT_2D:
      count  = ghost_size;
      offset[0] = g.offset[0]+g.count[0] - ghost_size[0];
      offset[1] = g.offset[1];
      break;
    case BOTTOM_CENTER_2D:
      count[0]  = ghost_size[0];  count[1]  = g.count[1] - 2*ghost_size[1];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1] + ghost_size[1];
      break;
    case BOTTOM_RIGHT_2D:
      count  = ghost_size;
      offset[0] = g.offset[0]+g.count[0] - ghost_size[0];
      offset[1] = g.offset[1]+g.count[1] - ghost_size[1];
      break;
    case TOP_2D:
      count[0]  = ghost_size[0];   count[1]  = g.count[1];
      offset[0] = g.offset[0];     offset[1] = g.offset[1];
      break;
    case RIGHT_2D:
      count[0]  = g.count[0];   count[1]  = ghost_size[1]; 
      offset[0] = g.offset[0];  offset[1] = g.offset[1] + g.count[1] - ghost_size[1];
      break;
    case BOTTOM_2D:
      count[0]  = ghost_size[0];  count[1]  =  g.count[1];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];;
      offset[1] = g.offset[1]; 
      break;
    case LEFT_2D:
       count[0]  = g.count[0];   count[1] = ghost_size[1]; 
      offset[0] = g.offset[0];  offset[1] = g.offset[1];
      break;
    case TOP_LEFT_3D: //For 3D
      count[0]  = g.count[0] - 2 * ghost_size[0];
      count[1] = ghost_size[1];
      count[2] = ghost_size[2];
      offset[0] = g.offset[0];
      offset[1] = g.offset[1];
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case TOP_CENTER_TOP_3D:
      count[0]  = g.count[0] - 2 * ghost_size[0];
      count[1]  = g.count[1] - 2 * ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0];
      offset[1] = g.offset[1] + ghost_size[1];
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case TOP_RIGHT_3D:
      count[0]  = g.count[0] - 2 * ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0];
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1];
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case TOP_FRONT_3D:
      count[0]  = ghost_size[0];
      count[1]  = g.count[1] - 2 * ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1] + ghost_size[1];
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case TOP_BACK_3D:
      count[0]  = ghost_size[0];
      count[1]  = g.count[1] - 2 * ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0];
      offset[1] = g.offset[1] + ghost_size[1];
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case TOP_LEFT_FRONT_3D:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1] + ghost_size[1];
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case TOP_LEFT_BACK_3D:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0];
      offset[1] = g.offset[1];
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case TOP_RIGHT_FRONT_3D:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1] ;
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case TOP_RIGHT_BACK_3D:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0];
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1] ;
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case CENTER_LEFT_3D:
      count[0]  = g.count[0] - 2 *ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = g.count[2] - 2 *ghost_size[2];
      offset[0] = g.offset[0] + ghost_size[0];
      offset[1] = g.offset[1];
      offset[2] = g.offset[2] + ghost_size[2];
      break;
    case CENTER_3D:
      count[0]  = g.count[0] - 2 *ghost_size[0];
      count[1]  = g.count[1] - 2 *ghost_size[1];
      count[2]  = g.count[2] - 2 *ghost_size[2];
      offset[0] = g.offset[0] + ghost_size[0];
      offset[1] = g.offset[1] + ghost_size[1];
      offset[2] = g.offset[2] + ghost_size[2];
      break;
    case CENTER_RIGHT_3D:
      count[0]  = g.count[0] - 2 *ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = g.count[2] - 2 *ghost_size[2];
      offset[0] = g.offset[0] + ghost_size[0];
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1];
      offset[2] = g.offset[2] + ghost_size[2];
      break;
    case CENTER_FRONT_3D:
      count[0]  = ghost_size[0];
      count[1]  = g.count[1] - 2 *ghost_size[1];
      count[2]  = g.count[2] - 2 *ghost_size[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1] + ghost_size[1];
      offset[2] = g.offset[2] + ghost_size[2];
      break;
    case CENTER_BACK_3D:
      count[0]  = ghost_size[0];
      count[1]  = g.count[1] - 2 *ghost_size[1];
      count[2]  = g.count[2] - 2 *ghost_size[2];
      offset[0] = g.offset[0];
      offset[1] = g.offset[1] + ghost_size[1];
      offset[2] = g.offset[2] + ghost_size[2];
      break;
    case CENTER_LEFT_FRONT_3D:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = g.count[2] - 2 *ghost_size[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1];
      offset[2] = g.offset[2] + ghost_size[2];
      break;
    case CENTER_LEFT_BACK_3D:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = g.count[2] - 2 *ghost_size[2];
      offset[0] = g.offset[0];
      offset[1] = g.offset[1];
      offset[2] = g.offset[2] + ghost_size[2];
      break;
    case CENTER_RIGHT_FRONT_3D:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = g.count[2] - 2 *ghost_size[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1];
      offset[2] = g.offset[2] + ghost_size[2];
      break;
    case CENTER_RIGHT_BACK_3D:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = g.count[2] - 2 *ghost_size[2];
      offset[0] = g.offset[0];
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1];
      offset[2] = g.offset[2] + ghost_size[2];
      break;
    case BOTTOM_LEFT_3D:
      count[0]  = g.count[0] -  2 * ghost_size[0] ;
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] + ghost_size[0];
      offset[1] = g.offset[1];
      offset[2] = g.offset[2];
      break;
    case BOTTOM_CENTER_BOTTOM_3D:
      count[0]  = g.count[0] -  2 * ghost_size[0] ;
      count[1]  = g.count[1] -  2 * ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] + ghost_size[0];
      offset[1] = g.offset[1] + ghost_size[1];
      offset[2] = g.offset[2];
      break;
    case BOTTOM_RIGHT_3D:
      count[0]  = g.count[0] -  2 * ghost_size[0] ;
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] + ghost_size[0];
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1];
      offset[2] = g.offset[2];
      break;
    case BOTTOM_FONT_3D:
      count[0]  = ghost_size[0] ;
      count[1]  = g.count[1] - 2 *ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1] + ghost_size[1];
      offset[2] = g.offset[2];
      break;
    case BOTTOM_BACK_3D:
      count[0]  = ghost_size[0] ;
      count[1]  = g.count[1] - 2 *ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0];
      offset[1] = g.offset[1] + ghost_size[1];
      offset[2] = g.offset[2];
      break;
    case BOTTOM_LEFT_FRONT_3D:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1];
      offset[2] = g.offset[2];
      break;
    case BOTTOM_LEFT_BACK_3D:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0];
      offset[1] = g.offset[1];
      offset[2] = g.offset[2];
      break;
    case BOTTOM_RIGHT_FRONT_3D:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1];
      offset[2] = g.offset[2];
      break;
    case BOTTOM_RIGHT_BACK_3D:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1];
      offset[2] = g.offset[2];
      break;
      //for 3D data's send
    case TOP_MIDDLE_3D_SEND:
      count[0]  = g.count[0];
      count[1]  = g.count[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case TOP_RIGHT_3D_SEND:
      count[0]  = g.count[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1];
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case TOP_LEFT_3D_SEND:
      count[0]  = g.count[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case TOP_BACK_3D_SEND:
      count[0]  = ghost_size[0];
      count[1]  = g.count[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case TOP_FRONT_3D_SEND:
      count[0]  = ghost_size[0];
      count[1]  = g.count[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] + g.count[2] - ghost_size[2];
      break;
    case CENTER_FRONT_MIDDLE_3D_SEND:
      count[0]  = ghost_size[0];
      count[1]  = g.count[1];
      count[2]  = g.count[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] ;
      break;
    case CENTER_BACK_MIDDLE_3D_SEND:
      count[0]  = ghost_size[0];
      count[1]  = g.count[1];
      count[2]  = g.count[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] ;
      break;
    case CENTER_LEFT_MIDDLE_3D_SEND:
      count[0]  = g.count[0];
      count[1]  = ghost_size[1];
      count[2]  = g.count[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] ;
      break;
    case CENTER_RIGHT_MIDDLE_3D_SEND:
      count[0]  = g.count[0];
      count[1]  = ghost_size[1];
      count[2]  = g.count[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1];
      offset[2] = g.offset[2] ;
      break;
    case  CENTER_FRONT_LEFT_3D_SEND:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = g.count[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] ;
      break;
    case CENTER_FRONT_RIGHT_3D_SEND:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = g.count[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0];
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1];
      offset[2] = g.offset[2] ;
      break;
    case CENTER_BACK_LEFT_3D_SEND:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = g.count[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] ;
      break;
    case CENTER_BACK_RIGHT_3D_SEND:
      count[0]  = ghost_size[0];
      count[1]  = ghost_size[1];
      count[2]  = g.count[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1] ;
      offset[2] = g.offset[2] ;
      break;
    case BOTTOM_MIDDLE_3D_SEND:
      count[0]  = g.count[0];
      count[1]  = g.count[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] ;
      break;
    case BOTTOM_RIGHT_3D_SEND:
      count[0]  = g.count[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] + g.count[1] - ghost_size[1];
      offset[2] = g.offset[2] ;
      break;
    case BOTTOM_LEFT_3D_SEND:
      count[0]  = g.count[0];
      count[1]  = ghost_size[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] ;
      break;
    case BOTTOM_BACK_3D_SEND:
      count[0]  = ghost_size[0];
      count[1]  = g.count[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] ;
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] ;
      break;
    case BOTTOM_FRONT_3D_SEND:
      count[0]  = ghost_size[0];
      count[1]  = g.count[1];
      count[2]  = ghost_size[2];
      offset[0] = g.offset[0] + g.count[0] - ghost_size[0] ;
      offset[1] = g.offset[1] ;
      offset[2] = g.offset[2] ;
      break;
    default:
      break; 
    }   
    return iArrayLocal(count, stride, offset);
}


inline iArrayLocal SubArrayRegionContigious( const iArrayLocal& g, 
                                         std::vector<int> start, 
                                         std::vector<int> count){
  //std::vector<int> start_int, count_int;
  //start_int.resize(start.size()); count_int.resize(count.size());
  //start_int[0] =start[0]; start_int[1] = start[1];
  //count_int[0] =count[0]; count_int[1] = count_int[1];
  
  return iArrayLocal(count, g.rowStride, start);
}

//------------------------------------------------------------------------------
// Point of customization for client code: specialize this function for the
// array element data type of choice
template < typename T > MPI_Datatype CreateArrayElementType();


//------------------------------------------------------------------------------
template < typename T >
MPI_Datatype CreateMPISubArrayType( const iArrayLocal& g, const iArrayLocal& subgrid ) {
    int dimensions = g.rank;
    std::vector<int> sizes = g.count;
    std::vector<int> subsizes = subgrid.count;
    std::vector<int> offsets  = subgrid.offset;
    int order = MPI_ORDER_C;
    MPI_Datatype arrayElementType = CreateArrayElementType< T >();// array element type
    MPI_Datatype newtype;
    MPI_Type_create_subarray( dimensions,
                              &sizes[0],
                              &subsizes[0],
                              &offsets[0],
                              order,
                              arrayElementType,
                              &newtype );
    MPI_Type_commit( &newtype );
    return newtype;
}

//------------------------------------------------------------------------------
// Return MPI task id from offset in MPI cartesian grid
inline int OffsetTaskId( MPI_Comm comm, std::vector<int> offset) {
    int thisRank = -1;
    MPI_Comm_rank( MPI_COMM_WORLD, &thisRank );
    std::vector<int> coord;  coord.resize(offset.size());
    MPI_Cart_coords(comm, thisRank, offset.size(), &coord[0] );
//    printf( "%d %d\n", coord[ 0 ], coord[ 1 ] );
    for(int i = 0; i < offset.size(); i++)
      coord[i] += offset[i];
    //coord[ 0 ] += xOffset;   coord[ 1 ] += yOffset;
    int rank = -1;
    MPI_Cart_rank( comm, &coord[0], &rank );
//    printf( "In rank: %d, offset: %d, %d; out rank: %d\n", thisRank, xOffset, yOffset, rank ); 
    return rank; 
}


//------------------------------------------------------------------------------
// Offset from current MPI taks in MPI cartesian grid
struct Offset {
  std::vector<int> coordinate;
 Offset( std::vector<int> c ) : coordinate( c ){}
};

// compute the offset in the MPI grid from the Region id
// of the local grid;
// assuming a row-major top-to-bottom MPI compute grid
// i.e. topmost row is 0 
inline Offset MPIOffsetRegion( MPIGridCellID mpicid ) {
  std::vector<int> off(3, -1);
  switch( mpicid ) {
    case   MPI_LEFT_1D:
      off.resize(1);
      off[0] = -1;
      break;
    case MPI_CENTER_1D:
      off.resize(1);
      off[0] = 0;
      break;
    case  MPI_RIGHT_1D:
      off.resize(1);
      off[0] = 1;
      break;
      
    case MPI_TOP_LEFT_2D:
      off.resize(2);
      off[0] =  -1;
      off[1] =  -1;
      break;
    case MPI_TOP_CENTER_2D:
      off.resize(2);
      off[0] =  -1;
      off[1] =  0;
      break;
    case MPI_TOP_RIGHT_2D:
      off.resize(2);
      off[0] = -1;
      off[1] = 1;
      break;
    case MPI_CENTER_LEFT_2D:
      off.resize(2);
        off[0] =  0;
        off[1] =  -1;
        break;
    case MPI_CENTER_RIGHT_2D:
      off.resize(2);
        off[0] = 0;
        off[1] = 1;
        break;
    case MPI_BOTTOM_LEFT_2D:
      off.resize(2);
        off[0] =  1;
        off[1] =  -1;
        break;
    case MPI_BOTTOM_CENTER_2D:
      off.resize(2);
        off[0] =  1;
        off[1] =  0;
        break;
    case MPI_BOTTOM_RIGHT_2D:
      off.resize(2);
        off[0] =  1;
        off[1] =  1;
        break;

    case MPI_TOP_LEFT_3D:
      off[0] =   0;
      off[1] =  -1;
      off[2] =   1;
     break;
    case MPI_TOP_CENTER_TOP_3D:
      off[0] =   0;
      off[1] =   0;
      off[2] =   1;
      break;
    case MPI_TOP_RIGHT_3D:
      off[0] =   0;
      off[1] =   1;
      off[2] =   1;
      break;
    case MPI_TOP_FRONT_3D:
      off[0] =   1;
      off[1] =   0;
      off[2] =   1;
      break;
    case MPI_TOP_BACK_3D:
      off[0] =  -1;
      off[1] =   0;
      off[2] =   1;
      break;
    case MPI_TOP_LEFT_FRONT_3D:
      off[0] =   1;
      off[1] =  -1;
      off[2] =   1;
      break;
    case MPI_TOP_LEFT_BACK_3D:
      off[0] =  -1;
      off[1] =  -1;
      off[2] =   1;
      break;
    case MPI_TOP_RIGHT_FRONT_3D:
      off[0] =   1;
      off[1] =   1;
      off[2] =   1;
      break;
    case MPI_TOP_RIGHT_BACK_3D:
      off[0] =   0;
      off[1] =   1;
      off[2] =   1;
      break;
    case  MPI_CENTER_LEFT_3D:
      off[0] =   0;
      off[1] =  -1;
      off[2] =   0;
      break;
    case MPI_CENTER_3D:
      off[0] =   0;
      off[1] =   0;
      off[2] =   0;
      break;
    case MPI_CENTER_RIGHT_3D:
      off[0] =   0;
      off[1] =   1;
      off[2] =   0;
      break;
    case MPI_CENTER_FRONT_3D:
      off[0] =   1;
      off[1] =   0;
      off[2] =   0;
      break;
    case MPI_CENTER_BACK_3D:
      off[0] =  -1;
      off[1] =   0;
      off[2] =   0;
      break;
    case MPI_CENTER_LEFT_FRONT_3D:
      off[0] =   1;
      off[1] =  -1;
      off[2] =   0;
      break;
    case MPI_CENTER_LEFT_BACK_3D:
      off[0] =   -1;
      off[1] =   -1;
      off[2] =   0;
      break;
    case MPI_CENTER_RIGHT_FRONT_3D:
      off[0] =   1;
      off[1] =   1;
      off[2] =   0;
      break;
    case MPI_CENTER_RIGHT_BACK_3D:
      off[0] =   -1;
      off[1] =   1;
      off[2] =   0;
      break;
    case MPI_BOTTOM_LEFT_3D:
      off[0] =   0;
      off[1] =   -1;
      off[2] =   -1;
      break;
    case MPI_BOTTOM_CENTER_BOTTOM_3D:
      off[0] =   0;
      off[1] =   0;
      off[2] =   -1;
      break;
    case MPI_BOTTOM_RIGHT_3D:
      off[0] =   0;
      off[1] =   1;
      off[2] =   -1;
      break;
    case MPI_BOTTOM_FONT_3D:
      off[0] =   1;
      off[1] =   0;
      off[2] =  -1;
      break;
    case MPI_BOTTOM_BACK_3D:
      off[0] =   -1;
      off[1] =   0;
      off[2] =   -1;
      break;
    case MPI_BOTTOM_LEFT_FRONT_3D:
      off[0] =   1;
      off[1] =   -1;
      off[2] =   -1;
      break;
    case MPI_BOTTOM_LEFT_BACK_3D:
      off[0] =   -1;
      off[1] =   -1;
      off[2] =   -1;
      break;
    case MPI_BOTTOM_RIGHT_FRONT_3D:
      off[0] =   1;
      off[1] =   1;
      off[2] =   -1;
      break;
    case MPI_BOTTOM_RIGHT_BACK_3D:
      off[0] =   -1;
      off[1] =    1;
      off[2] =   -1;
      break;
    default:
        break;
    }
    return Offset(off);
}

//------------------------------------------------------------------------------
// Danta transfer information to be used by MPI send/receive operations
struct TransferInfo {
    int srcTaskId;
    int destTaskId;
    int tag;
    void* data;
    MPI_Request request; // currently not used
    MPI_Datatype type;
    MPI_Comm comm;
};

//------------------------------------------------------------------------------
// Create entry with information for data transfer from a remote MPI task
// to a local subregion of a 2d array
// IN: remote MPI task, local target memory region, data pointer, sub-array layout
// OUT: transfer information including endpoints and MPI datatype matching the
//      2d array layout passed as input 
template < typename T > 
TransferInfo CreateReceiveInfo( T* pdata, MPI_Comm cartcomm, int rank, 
			        MPIGridCellID remoteSource, RegionID localTargetRegion,
                                iArrayLocal& g, std::vector<int> ghost_size, int tag ) {
    TransferInfo ti;
    ti.comm = cartcomm;
    ti.data = pdata;
    ti.destTaskId = rank;
    ti.tag = tag;
    ti.type = CreateMPISubArrayType< T >( g, 
                                          SubArrayRegion( g, ghost_size, localTargetRegion ) );
    Offset offset = MPIOffsetRegion( remoteSource ); 
    ti.srcTaskId = OffsetTaskId( cartcomm, offset.coordinate);

// printf( "source %d dest %d\n", ti.srcTaskId, ti.destTaskId ); 
      return ti;     
}
 
//------------------------------------------------------------------------------
// Create entry with information for data transfer from a local memory area
// inside the core space(local grid minus ghost regions) to a remote MPI task
// IN: local region, remote MPI task, data pointer, sub-array layout
// OUT: transfer information including endpoints and MPI datatype matching the
//      2d array layout passed as input 
template< typename T > 
TransferInfo CreateSendInfo( T* pdata, MPI_Comm cartcomm, int rank, 
                             MPIGridCellID remoteTarget, RegionID localSourceRegion, iArrayLocal& g,
                             std::vector<int> ghost_size, int tag ) {
    TransferInfo ti;
    ti.comm = cartcomm;
    ti.data = pdata;
    ti.srcTaskId = rank;
    ti.tag = tag;

    RegionID id;
    switch(g.rank){
      case 1:
        id=CENTER_1D;
        break;
      case 2:
        id=CENTER_2D;
        break;
      case 3:
        id=CENTER_3D;
        break;
      default:
        break;
    }
    iArrayLocal core = SubArrayRegion( g, ghost_size, id);
    
    ti.type = CreateMPISubArrayType< T >( g, 
                                          SubArrayRegion( core, ghost_size, localSourceRegion ) );
    Offset offset = MPIOffsetRegion( remoteTarget ); 
    ti.destTaskId = OffsetTaskId( cartcomm, offset.coordinate);  
    return ti;     
}

//------------------------------------------------------------------------------
// Iterate over arrays of data transfer info and perform actual MPI data transfers
inline void ExchangeData( std::vector< TransferInfo >& recvArray,
                          std::vector< TransferInfo >& sendArray ) {

    std::vector< MPI_Request  > requests( recvArray.size() + sendArray.size() );
    for( int i = 0; i != recvArray.size(); ++i ) {
        TransferInfo& t = recvArray[ i ];
        MPI_Irecv( t.data, 1, t.type, t.srcTaskId, t.tag, t.comm, &( requests[ i ] ) );  
    }
    for( int i = 0; i != sendArray.size(); ++i ) {
        TransferInfo& t = sendArray[ i ];
        MPI_Isend( t.data, 1, t.type, t.destTaskId, t.tag, t.comm, &( requests[ recvArray.size() + i ] ) );  
    }
    std::vector< MPI_Status > status( recvArray.size() + sendArray.size() );
    MPI_Waitall( requests.size(), &requests[ 0 ], &status[ 0 ] );  
}

//------------------------------------------------------------------------------
// Create pair of <recv, send> info array 
template < typename T > 
std::pair< std::vector< TransferInfo >,
           std::vector< TransferInfo > > 
CreateSendRecvArrays( T* pdata, MPI_Comm cartcomm, int rank, iArrayLocal& g,
                      std::vector<int> ghost_size ) {
    std::vector< TransferInfo > ra;
    std::vector< TransferInfo > sa;
    // send regions: data extracted from core(CENTER) region
    RegionID localSendSource[] = {  TOP_LEFT_2D,    TOP_2D,    TOP_RIGHT_2D,
                                    LEFT_2D,                   RIGHT_2D,
                                    BOTTOM_LEFT_2D, BOTTOM_2D, BOTTOM_RIGHT_2D };

    // remote send targets
    MPIGridCellID remoteSendTarget[] = { MPI_TOP_LEFT_2D,    MPI_TOP_CENTER_2D,    MPI_TOP_RIGHT_2D,
                                         MPI_CENTER_LEFT_2D,                    MPI_CENTER_RIGHT_2D,
                                         MPI_BOTTOM_LEFT_2D, MPI_BOTTOM_CENTER_2D, MPI_BOTTOM_RIGHT_2D };

    // recv regions: data inserted into local grid
    RegionID localRecvTarget[]  = { BOTTOM_RIGHT_2D,  BOTTOM_CENTER_2D,    BOTTOM_LEFT_2D,
                                    CENTER_RIGHT_2D,                CENTER_LEFT_2D,
                                    TOP_RIGHT_2D, TOP_CENTER_2D, TOP_LEFT_2D };


    // remote recv sources
    MPIGridCellID remoteRecvSource[] = { MPI_BOTTOM_RIGHT_2D,  MPI_BOTTOM_CENTER_2D, MPI_BOTTOM_LEFT_2D,
                                         MPI_CENTER_RIGHT_2D,                     MPI_CENTER_LEFT_2D,
                                         MPI_TOP_RIGHT_2D,     MPI_TOP_CENTER_2D,    MPI_TOP_LEFT_2D };

    RegionID*      lss = &localSendSource [ 0 ];
    MPIGridCellID* rst = &remoteSendTarget[ 0 ];
    RegionID*      lrt = &localRecvTarget [ 0 ];
    MPIGridCellID* rrs = &remoteRecvSource[ 0 ];
    const size_t sz = sizeof( localSendSource ) / sizeof( RegionID );
    const RegionID* end = lss + sz;
    while( lss != end ) {
        ra.push_back( CreateReceiveInfo( pdata,    // data 
                                         cartcomm, // MPI cartesian communicator 
                                         rank,     // rank of this process
                                         *rrs,     // MPI cell id of process to read data from
                                         *lrt,     // local receive target i.e. id of local area to fill
                                            g,     // local 2D Array
                                         ghost_size,
                                         *lss ) );  // tag
        sa.push_back( CreateSendInfo( pdata, cartcomm, rank,
                                      *rst, // MPI cell id of process to send data to
                                      *lss, // local send source i.e. id of local area to extract data from
                                         g, // local 2D Array
                                      ghost_size,
                                      *lss ) ); // tag  

        ++lss;
        ++rst;
        ++lrt;
        ++rrs;                                           
    }

    return std::make_pair( ra, sa ); 
}


/* //------------------------------------------------------------------------------ */
/* inline void TestSubRegionExtraction() { */
/*     const int w = 32; */
/*     const int h = 32; */
/*     const int stencilWidth = 5; */
/*     const int stencilHeight = 5; */
/*     const int totalWidth = w + stencilWidth / 2; */
/*     const int totalHeight = h + stencilHeight / 2; */
/*     // Grid(core + halo) */
/*     std::vector< int > data( totalWidth * totalHeight, 0 ); */
/*     Array2D grid( totalWidth, totalHeight, totalWidth ); */
/*     Array2D topleft = SubArrayRegion( grid, stencilWidth, stencilHeight, TOP_LEFT ); */
/*     Array2D topcenter = SubArrayRegion( grid, stencilWidth, stencilHeight, TOP_CENTER ); */
/*     Array2D topright= SubArrayRegion( grid, stencilWidth, stencilHeight, TOP_RIGHT ); */
/*     Array2D centerleft = SubArrayRegion( grid, stencilWidth, stencilHeight, CENTER_LEFT ); */
/*     Array2D center = SubArrayRegion( grid, stencilWidth, stencilHeight, CENTER ); */
/*     Array2D centerright = SubArrayRegion( grid, stencilWidth, stencilHeight, CENTER_RIGHT ); */
/*     Array2D bottomleft = SubArrayRegion( grid, stencilWidth, stencilHeight, BOTTOM_LEFT ); */
/*     Array2D bottomcenter = SubArrayRegion( grid, stencilWidth, stencilHeight, BOTTOM_CENTER ); */
/*     Array2D bottomright = SubArrayRegion( grid, stencilWidth, stencilHeight, BOTTOM_RIGHT ); */
  
/*     std::cout << "\nGRID TEST\n"; */
    
/*     std::cout << "Width: " << totalWidth << ", " << "Height: " << totalHeight << std::endl; */
/*     std::cout << "Stencil: " << stencilWidth << ", " << stencilHeight << std::endl; */
 
/*     std::cout << "top left:      " << topleft      << std::endl; */
/*     std::cout << "top center:    " << topcenter    << std::endl; */
/*     std::cout << "top right:     " << topright     << std::endl; */
/*     std::cout << "center left:   " << centerleft   << std::endl; */
/*     std::cout << "center:        " << center       << std::endl; */
/*     std::cout << "center right:  " << centerright  << std::endl; */
/*     std::cout << "bottom left:   " << bottomleft   << std::endl; */
/*     std::cout << "bottom center: " << bottomcenter << std::endl; */
/*     std::cout << "bottom right:  " << bottomright  << std::endl; */

/*     std::cout << "\nSUBGRID TEST\n"; */

/*     // Core space(area of stencil application) */
/*     Array2D core = center; */
/*     topleft = SubArrayRegion( core, stencilWidth, stencilHeight, TOP_LEFT ); */
/*     topcenter = SubArrayRegion( core, stencilWidth, stencilHeight, TOP_CENTER ); */
/*     topright= SubArrayRegion( core, stencilWidth, stencilHeight, TOP_RIGHT ); */
/*     centerleft = SubArrayRegion( core, stencilWidth, stencilHeight, CENTER_LEFT ); */
/*     center = SubArrayRegion( core, stencilWidth, stencilHeight, CENTER ); */
/*     centerright = SubArrayRegion( core, stencilWidth, stencilHeight, CENTER_RIGHT ); */
/*     bottomleft = SubArrayRegion( core, stencilWidth, stencilHeight, BOTTOM_LEFT ); */
/*     bottomcenter = SubArrayRegion( core, stencilWidth, stencilHeight, BOTTOM_CENTER ); */
/*     bottomright = SubArrayRegion( core, stencilWidth, stencilHeight, BOTTOM_RIGHT ); */
/*     Array2D top = SubArrayRegion( core, stencilWidth, stencilHeight, TOP ); */
/*     Array2D right = SubArrayRegion( core, stencilWidth, stencilHeight, RIGHT ); */
/*     Array2D bottom = SubArrayRegion( core, stencilWidth, stencilHeight, BOTTOM ); */
/*     Array2D left = SubArrayRegion( core, stencilWidth, stencilHeight, LEFT ); */

/*     std::cout << "Width: " << core.width << ", " << "Height: " << core.height << std::endl; */
/*     std::cout << "Stencil: " << stencilWidth << ", " << stencilHeight << std::endl; */
    
/*     std::cout << "top left:      " << topleft      << std::endl; */
/*     std::cout << "top center:    " << topcenter    << std::endl; */
/*     std::cout << "top right:     " << topright     << std::endl; */
/*     std::cout << "center left:   " << centerleft   << std::endl; */
/*     std::cout << "center:        " << center       << std::endl; */
/*     std::cout << "center right:  " << centerright  << std::endl; */
/*     std::cout << "bottom left:   " << bottomleft   << std::endl; */
/*     std::cout << "bottom center: " << bottomcenter << std::endl; */
/*     std::cout << "bottom right:  " << bottomright  << std::endl; */
/*     std::cout << "top:           " << top          << std::endl; */
/*     std::cout << "right:         " << right        << std::endl; */
/*     std::cout << "bottom:        " << bottom       << std::endl; */
/*     std::cout << "left:          " << left         << std::endl; */
/* } */

//------------------------------------------------------------------------------
inline void PrintCartesianGrid( std::ostream& os, MPI_Comm comm, int rows, int columns ) {

    std::vector< std::vector< int > > grid( rows, std::vector< int >( columns, -1 ) );

    for( int r = 0; r != rows; ++r ) {
        for( int c = 0; c != columns; ++c ) {
            int coords[] = { -1, -1 };
            MPI_Cart_coords( comm, r * columns + c, 2, coords );
            grid[ coords[ 0 ] ][ coords[ 1 ] ] = r * columns + c;   
        }        
    }
    for( int r = 0; r != rows; ++r ) {
        for( int c = 0; c != columns; ++c ) {
            os << grid[ r ][ c ] << ' '; 
        }
        os << std::endl;
    }   
}





#endif
