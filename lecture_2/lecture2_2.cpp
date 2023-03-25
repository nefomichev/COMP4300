//
// Created by Nikita Fomichev on 25.03.2023.
//

#include <iostream>
#include <vector>
#include <fstream>

class Student
{
    std::string m_firstName = "Name";
    std::string m_lastName = "Surname";
    int m_age = 0;

public:
    Student() {}
    Student(const std::string& first, const std::string& last, const int& age)
    : m_firstName(first)
    , m_lastName(last)
    , m_age(age)
    {
    }
    std::string getFirstName() const
    {
        return m_firstName;
    }
    std::string getLastName() const
    {
        return m_lastName;
    }
    int getAge() const
    {
        return m_age;
    }
    void print() const
    {
        std::cout << m_firstName << " " << m_lastName << " " << m_age << std::endl;
    }
};

class Course
{
    std::string m_name = "Course";
    std::vector<Student> m_students;

public:
    Course() {}
    Course(const std::string& name) {}

    void addStudent(const Student& student)
    {
        m_students.push_back(student);
    }

    const std::vector<Student>& getStudents() const
    {
        return m_students;
    }

    void printStudents() const
    {
        for (auto i: m_students)
        {
            i.print();
        }
    }
    void loadFromFile(const std::string& filename)
    {
       std::ifstream  fin(filename);
       std::string name;
       std::string surname;
       int age;

        while (fin >> name)
        {
            fin >> surname >> age;
            addStudent(Student(name, surname, age));
        }
    }
};

int main(int argc, char* argv[])
{
    Course c1("COMP4300");
    c1.loadFromFile("lecture_2/students.txt");
    c1.printStudents();

    return 0;
}