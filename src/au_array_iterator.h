
#ifndef AU_ARRAY_ITERATOR_H
#define AU_ARRAY_ITERATOR_H

#include <array>
#include <iostream>
#include <vector>

template <typename T>
class ArrayIterator
{
    using A = std::vector<T>;
    A b, i, e;
    T N;

public:
    /*
    //The e_p is not included
    //The b_p is included
    //e.g., b_p  = (0, 0), e_p = (3,3)
    //output is:
        0, 0
        0, 1
        0, 2
        1, 0
        1, 1
        1, 2
        2, 0
        2, 1
        2, 2
    */
    ArrayIterator(const A &b_p, const A &e_p) : b(b_p), i(b_p), e(e_p), N(b_p.size()) {}

    ArrayIterator &operator++()
    {
        for (size_t n = N - 1; n >= 0; --n)
        {
            if (++i[n] == e[n])
            {
                if (n >= 1)
                    i[n] = b[n];
            }
            else
            {
                break;
            }
        }
        return *this;
    }
    operator bool() { return i[0] != e[0]; }
    T &operator[](size_t n) { return i[n]; }
    const T &operator[](size_t n) const { return i[n]; }
};

/*
int main()
{
    using A = std::vector<size_t>;

    A begin = {1, 1};
    A end = {4, 4};
    A current(2);

    for (ArrayIterator<size_t> c(begin, end); c; ++c)
    {
        current[0] = c[0];
        current[1] = c[1];
        cout << current[0] << ", " << current[1] << endl;
    }
}
*/

//Some ref code
/*
class counter
{
	using A = std::array<T, N>;
	A b, i, e;

public:
	counter(const A& b, const A& e) : b(b), i(b), e(e) { }

	counter& operator++()
	{
		for (size_t n = 0; n < N; ++n)
		{
			if (++i[n] == e[n])
			{
				if (n < N - 1)
					i[n] = b[n];
			}
			else
				break;
		}

		return *this;
	}

	operator bool() { return i[N - 1] != e[N - 1]; }

	T&       operator[](size_t n)       { return i[n]; }
	const T& operator[](size_t n) const { return i[n]; }
};

template<typename S, typename T, size_t N>
S& operator<<(S& s, const counter<T, N>& a)
{
	for (size_t n = 0; n < N; ++n)
		s << a[n] << " ";

	return s;
}
*/
#endif