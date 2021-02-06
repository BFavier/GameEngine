#pragma once
#include <GameEngine/Utilities.hpp>

namespace GameEngine
{
    class Vector;
    class Quaternion;

    class Matrix
    {
    public:
        Matrix();
        Matrix(const Matrix& other);
        Matrix(unsigned int rows, unsigned int columns);
        Matrix(const std::initializer_list<std::initializer_list<double>>& elements);
        Matrix(const Vector& vector);
        Matrix(const Quaternion& quat);
        ~Matrix();
    public:
        Matrix t() const;
        unsigned int rows() const;
        unsigned int columns() const;
        glm::mat4 to_mat4() const;
        glm::mat3 to_mat3() const;
        double& operator()(unsigned int i, unsigned int j);
        double operator()(unsigned int i, unsigned int j) const;
        Matrix operator*(const Matrix& other) const;
        Matrix& operator=(const Matrix& other);
        friend std::ostream& operator<<(std::ostream& os, const Matrix& mat);
    protected:
        unsigned int _rows = 0;
        unsigned int _columns = 0;
        double** _data = nullptr;
    protected:
        void allocate();
        void deallocate();
        void check_range(unsigned int i, unsigned int j) const;
    };
}
