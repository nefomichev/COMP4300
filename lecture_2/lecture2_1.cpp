//
// Created by Nikita Fomichev on 25.03.2023.
//

#include <iostream>
#include <vector>
#include <filesystem>

int main(int argc, char* argv[])
{
    std::vector<int> vec;
    vec.push_back(43);
    vec.push_back(33);

    std::cout << vec[0] << std::endl;
    std::cout << vec[1] << std::endl;

    // whatever happened to be in memory in this location. C++ does not control it
    // no error !! 
    //std::cout << vec[2] << std::endl;


    // Old school
    for (size_t i = 0; i < vec.size(); ++i)
    // do not use int for i since size() would have different datatype
    {
        std::cout << vec[i] << std::endl;
    }

    // c++ 11
    for (int i : vec)
    // type of i should match the type in vector, otherwise we convert since it is assigment
    {
        std::cout << i << std::endl;
    }

    // modern c++
    for (auto i : vec)
    // compiler guess the type from vec definition.
        // however we still copy data in each loop
    {
        std::cout << i << std::endl;
    }

    // avoid coping
    for (auto& i : vec)
        // use reference to avoid coping
    {
        std::cout << i << std::endl;
    }
    return 0;
}