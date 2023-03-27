#include <iostream>

template <typename T>
class DynamicArray
{
    size_t m_size;
    T*     m_array; 

public:
    DynamicArray(size_t size)
        : m_size (size)
        , m_array(new T[size])
    {
        std::cout << "Class Constructed" << std::endl;
    }

    ~DynamicArray()
    {
        delete [] m_array;
        std::cout << "Class Destroyed" << std::endl;
    }

    void print() const
    {
        for (size_t index=0; index < m_size; ++index)
        {
            std::cout << index << " " << &m_array[index] << " " << m_array[index] << std::endl;
        };
    };

    T& operator [] (size_t index)
    {
        return m_array[index];
    }
};

int main()
{
    DynamicArray<int> my_arr(10);
    my_arr.print();

    for (size_t i=0; i < sizeof(my_arr); ++i)
    {
        my_arr[i] = i + 1000;
    };
    
    my_arr.print();
    return 0;
}
