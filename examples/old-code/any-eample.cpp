#include <any>
#include <iostream>
#include <vector>
#include <stdlib.h>
int main()
{
    int *buf = (int *)malloc(sizeof(int) * 10);
    for (int i = 0; i < 10; i++)
    {
        buf[i] = i;
    }

    std::vector<std::any> vany(buf, buf + 10);
    for (int i = 0; i < 10; i++)
        std::cout << std::any_cast<int>(vany[i]) << std::endl;
}