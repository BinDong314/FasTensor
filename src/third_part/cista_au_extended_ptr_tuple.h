//FasTensor's extension cista to more variables
//include this file in cista.h

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<21>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<22>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<23>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<24>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<25>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<26>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<27>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<28>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<29>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<30>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<31>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<32>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<33>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<34>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<35>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<36>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<37>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<38>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<39>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<40>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<41>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<42>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<43>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<44>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<45>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<46>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<47>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<48>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<49>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<50>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<51>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<52>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<53>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<54>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<55>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<56>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<57>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<58>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<59>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<60>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<61>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<62>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<63>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<64>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<65>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<66>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<67>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<68>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<69>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<70>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<71>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<72>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<73>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<74>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<75>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<76>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<77>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<78>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<79>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<80>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<81>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<82>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<83>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<84>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<85>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<86>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<87>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<88>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<89>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<90>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<91>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<92>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<93>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<94>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<95>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<96>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<97>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<98>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<99>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<100>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<101>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<102>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<103>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<104>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<105>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<106>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<107>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<108>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<109>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<110>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<111>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<112>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<113>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<114>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<115>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<116>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<117>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<118>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<119>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<120>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<121>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<122>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<123>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<124>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<125>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<126>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<127>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<128>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<129>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<130>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<131>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<132>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<133>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<134>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<135>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<136>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<137>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<138>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<139>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<140>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<141>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<142>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<143>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<144>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<145>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<146>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<147>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<148>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<149>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<150>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<151>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<152>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<153>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<154>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<155>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<156>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<157>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<158>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<159>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<160>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<161>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<162>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<163>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<164>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<165>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<166>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<167>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<168>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<169>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<170>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<171>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<172>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<173>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<174>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<175>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<176>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<177>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<178>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<179>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<180>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<181>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<182>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<183>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<184>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<185>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<186>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<187>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<188>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<189>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188, p189] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188, &p189);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<190>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188, p189, p190] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188, &p189, &p190);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<191>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188, p189, p190, p191] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188, &p189, &p190, &p191);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<192>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188, p189, p190, p191, p192] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188, &p189, &p190, &p191, &p192);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<193>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188, p189, p190, p191, p192, p193] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188, &p189, &p190, &p191, &p192, &p193);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<194>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188, p189, p190, p191, p192, p193, p194] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188, &p189, &p190, &p191, &p192, &p193, &p194);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<195>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188, p189, p190, p191, p192, p193, p194, p195] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188, &p189, &p190, &p191, &p192, &p193, &p194, &p195);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<196>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188, p189, p190, p191, p192, p193, p194, p195, p196] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188, &p189, &p190, &p191, &p192, &p193, &p194, &p195, &p196);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<197>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188, p189, p190, p191, p192, p193, p194, p195, p196, p197] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188, &p189, &p190, &p191, &p192, &p193, &p194, &p195, &p196, &p197);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<198>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188, p189, p190, p191, p192, p193, p194, p195, p196, p197, p198] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188, &p189, &p190, &p191, &p192, &p193, &p194, &p195, &p196, &p197, &p198);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<199>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188, p189, p190, p191, p192, p193, p194, p195, p196, p197, p198, p199] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188, &p189, &p190, &p191, &p192, &p193, &p194, &p195, &p196, &p197, &p198, &p199);
}

template <typename T>
inline auto to_ptr_tuple(T &t, field_count<200>)
{
    auto &[p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, p55, p56, p57, p58, p59, p60, p61, p62, p63, p64, p65, p66, p67, p68, p69, p70, p71, p72, p73, p74, p75, p76, p77, p78, p79, p80, p81, p82, p83, p84, p85, p86, p87, p88, p89, p90, p91, p92, p93, p94, p95, p96, p97, p98, p99, p100, p101, p102, p103, p104, p105, p106, p107, p108, p109, p110, p111, p112, p113, p114, p115, p116, p117, p118, p119, p120, p121, p122, p123, p124, p125, p126, p127, p128, p129, p130, p131, p132, p133, p134, p135, p136, p137, p138, p139, p140, p141, p142, p143, p144, p145, p146, p147, p148, p149, p150, p151, p152, p153, p154, p155, p156, p157, p158, p159, p160, p161, p162, p163, p164, p165, p166, p167, p168, p169, p170, p171, p172, p173, p174, p175, p176, p177, p178, p179, p180, p181, p182, p183, p184, p185, p186, p187, p188, p189, p190, p191, p192, p193, p194, p195, p196, p197, p198, p199, p200] = t;
    return std::make_tuple(&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14, &p15, &p16, &p17, &p18, &p19, &p20, &p21, &p22, &p23, &p24, &p25, &p26, &p27, &p28, &p29, &p30, &p31, &p32, &p33, &p34, &p35, &p36, &p37, &p38, &p39, &p40, &p41, &p42, &p43, &p44, &p45, &p46, &p47, &p48, &p49, &p50, &p51, &p52, &p53, &p54, &p55, &p56, &p57, &p58, &p59, &p60, &p61, &p62, &p63, &p64, &p65, &p66, &p67, &p68, &p69, &p70, &p71, &p72, &p73, &p74, &p75, &p76, &p77, &p78, &p79, &p80, &p81, &p82, &p83, &p84, &p85, &p86, &p87, &p88, &p89, &p90, &p91, &p92, &p93, &p94, &p95, &p96, &p97, &p98, &p99, &p100, &p101, &p102, &p103, &p104, &p105, &p106, &p107, &p108, &p109, &p110, &p111, &p112, &p113, &p114, &p115, &p116, &p117, &p118, &p119, &p120, &p121, &p122, &p123, &p124, &p125, &p126, &p127, &p128, &p129, &p130, &p131, &p132, &p133, &p134, &p135, &p136, &p137, &p138, &p139, &p140, &p141, &p142, &p143, &p144, &p145, &p146, &p147, &p148, &p149, &p150, &p151, &p152, &p153, &p154, &p155, &p156, &p157, &p158, &p159, &p160, &p161, &p162, &p163, &p164, &p165, &p166, &p167, &p168, &p169, &p170, &p171, &p172, &p173, &p174, &p175, &p176, &p177, &p178, &p179, &p180, &p181, &p182, &p183, &p184, &p185, &p186, &p187, &p188, &p189, &p190, &p191, &p192, &p193, &p194, &p195, &p196, &p197, &p198, &p199, &p200);
}
