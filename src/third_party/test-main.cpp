#include <string>
#include "cista.h"

struct A
{
  CISTA_PRINTABLE(A)
  CISTA_COMPARABLE()
  //some
  int i_ = 1;
  int j_ = 2;
  double d_ = 100.0;
  std::string s_ = "hello";
  int i0;
  int i011, i012;
  int i1;
  int i2;
  //int
  int i3, i30, i31;
  int i4;
  int i5;
  int i6;
  int i7;
  int i8;
  int i9;
  int i10;
  int i11;
  int i12;
  int i13;
  int i14;
  int i15;
  int i16;
  int i17;
  int i18;
  int i19;
  float ex, ey, ez;
  float bx, by, bz;
  // densities
  float e1n, e2n, i1n, i2n;
  //electron current
  float e1jx, e1jy, e1jz;
  float e2jx, e2jy, e2jz;
  //ion current vector
  float i1jx, i1jy, i1jz;
  float i2jx, i2jy, i2jz;
  // electron pressure
  float e1pxx, e1pyy, e1pzz, e1pxy, e1pxz, e1pyz;
  float e2pxx, e2pyy, e2pzz, e2pxy, e2pxz, e2pyz;
  float e1pyx, e1pzx, e1pzy, e2pyx, e2pzx, e2pzy;
  // ion pressure
  float i1pxx, i1pyy, i1pzz, i1pxy, i1pxz, i1pyz;
  float i2pxx, i2pyy, i2pzz, i2pxy, i2pxz, i2pyz;
  float i1pyx, i1pzx, i1pzy, i2pyx, i2pzx, i2pzy;
};

int main()
{
  using cista::to_tuple;
  using std::get;
  A a1;

  get<0>(to_tuple(a1)) = 5;
  get<1>(to_tuple(a1)) = 7;
  get<2>(to_tuple(a1)) = 2.0;
  get<3>(to_tuple(a1)) = "yeah";

  int iii = 0;
  cista::for_each_field(a1, [](auto &&m) { iii++; m = 1.111; });

  std::cout << " read 0" << get<0>(to_tuple(a1)) << "\n\n";
  std::cout << " read 3" << get<3>(to_tuple(a1)) << "\n\n";

  std::cout << a1 << "\n";

  A a2;
  get<0>(to_tuple(a2)) = 5;
  get<1>(to_tuple(a2)) = 7;
  get<2>(to_tuple(a2)) = 2.1;
  get<3>(to_tuple(a2)) = "yeah";

  std::cout << a2 << "\n";

  if (a1 == a2)
    std::cout << "a1 = a2"
              << "\n";
  else
    std::cout << "a1 != a2"
              << "\n";

  cista::for_each_field(a1, [](auto &&m) { std::cout << m << "\n"; });
}
