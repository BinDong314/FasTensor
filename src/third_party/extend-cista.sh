#!/bin/bash

N=201
to_tuple_extended_file="cista_au_extended_tuple.h"
auto_tuple_str="auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20"
retu_tuple_str="return std::tie(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20"

echo "//ArrayUDF's extension cista to more variables" > $to_tuple_extended_file
echo "//include this file in cista.h " >> $to_tuple_extended_file
echo "" >> $to_tuple_extended_file

for (( i  = 21; i < $N; i++))
do
    echo "template <typename T>" >> $to_tuple_extended_file
    echo "inline auto to_tuple(T &t, field_count<$i>)" >> $to_tuple_extended_file
    echo "{" >> $to_tuple_extended_file
    
    auto_tuple_str="$auto_tuple_str ,p$i"
    retu_tuple_str="$retu_tuple_str ,p$i"
    echo "$auto_tuple_str ]=t;" >> $to_tuple_extended_file
    echo "$retu_tuple_str );" >> $to_tuple_extended_file
    #echo auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16,
    #     p17, p18, p19, p20] = t;
    #return std::tie(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
    #               p15, p16, p17, p18, p19, p20);
    echo "}" >> $to_tuple_extended_file
    echo "" >> $to_tuple_extended_file
done


to_tuple_extended_ptr_file="cista_au_extended_ptr_tuple.h"
echo "//ArrayUDF's extension cista to more variables" > $to_tuple_extended_ptr_file
echo "//include this file in cista.h " >> $to_tuple_extended_ptr_file
echo "" >> $to_tuple_extended_ptr_file

  #auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16,
  #       p17, p18, p19,
  #       p20] = t;
  #return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10,
  #                       &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19,
  #                       &p20);
auto_ptr_tuple_str="auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16,p17, p18, p19,p20"
retu_ptr_tuple_str="return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19,&p20"
for (( i  = 21; i < $N; i++))
do
    echo "template <typename T>" >> $to_tuple_extended_ptr_file
    echo "inline auto to_ptr_tuple(T &t, field_count<$i>)" >> $to_tuple_extended_ptr_file
    echo "{" >> $to_tuple_extended_ptr_file
    
    auto_ptr_tuple_str="$auto_ptr_tuple_str ,p$i"
    retu_ptr_tuple_str="$retu_ptr_tuple_str ,&p$i"
    echo "$auto_ptr_tuple_str ]=t;" >> $to_tuple_extended_ptr_file
    echo "$retu_ptr_tuple_str );" >> $to_tuple_extended_ptr_file
    echo "}" >> $to_tuple_extended_ptr_file
    echo "" >> $to_tuple_extended_ptr_file
done


#MAKE_ARITY_FUNC(19)
#MAKE_ARITY_FUNC(20)

to_make_arity_extended_file="cista_au_extended_arity.h"

echo "//ArrayUDF's extension cista to more variables" > $to_make_arity_extended_file
echo "//include this file in cista.h " >> $to_make_arity_extended_file
echo "" >> $to_make_arity_extended_file
for (( i  = 21; i < $N; i++))
do
    echo "MAKE_ARITY_FUNC($i)" >> $to_make_arity_extended_file
done
echo "" >> $to_make_arity_extended_file

