#include <GameEngine/geometry/Matrix.hpp>
#include <GameEngine/geometry/vectors/Vector.hpp>
#include <GameEngine/geometry/quaternions/Quaternion.hpp>
using namespace GameEngine;

Matrix::Matrix()
{
}

Matrix::Matrix(const Matrix& other) : _rows(other.rows()), _columns(other.columns())
{
    allocate();
    *this = other;
}

Matrix::Matrix(unsigned int r, unsigned int c) : _rows(r), _columns(c)
{
    allocate();
}

Matrix::Matrix(const std::initializer_list<std::initializer_list<double>>& elements)
{
    std::vector<std::vector<double>> data;
    for (const std::initializer_list<double>& item : elements)
    {
        data.push_back(item);
    }
    _rows = data.size();
    _columns = data[0].size();
    allocate();
    for (unsigned int i=0; i<_rows; i++)
    {
        if (data[i].size() != _columns)
        {
            THROW_ERROR("All lines must have the same dimension when initializing a Matrix")
        }
        for (unsigned int j=0; j<_columns; j++)
        {
            _data[i][j] = data[i][j];
        }
    }
}

Matrix::Matrix(const Vector& vector) : _rows(3), _columns(1)
{
    allocate();
    _data[0][0] = vector.x;
    _data[1][0] = vector.y;
    _data[2][0] = vector.z;
}

Matrix::Matrix(const Quaternion& quat) : _rows(3), _columns(3)
{
    allocate();
    _data[0][0] = std::pow(quat.w, 2.) + std::pow(quat.x, 2.) - std::pow(quat.y, 2.) - std::pow(quat.z, 2.);
    _data[0][1] = 2*quat.x*quat.y - 2*quat.w*quat.z;
    _data[0][2] = 2*quat.w*quat.y + 2*quat.x*quat.z;

    _data[1][0] = 2*quat.w*quat.z + 2*quat.x*quat.y;
    _data[1][1] = std::pow(quat.w, 2.) - std::pow(quat.x, 2.) + std::pow(quat.y, 2.) - std::pow(quat.z, 2.);
    _data[1][2] = 2*quat.y*quat.z - 2*quat.w*quat.x;

    _data[2][0] = 2*quat.x*quat.z - 2*quat.w*quat.y;
    _data[2][1] = 2*quat.w*quat.x + 2*quat.y*quat.z;
    _data[2][2] = std::pow(quat.w, 2.) - std::pow(quat.x, 2.) - std::pow(quat.y, 2.) + std::pow(quat.z, 2.);
}

Matrix::~Matrix()
{
    deallocate();
}

Matrix Matrix::t() const
{
    Matrix result(_columns, _rows);
    for (unsigned int i=0; i<_rows; i++)
    {
        for (unsigned int j=0; j<_columns; j++)
        {
            result(j,i) = _data[i][j];
        }
    }
    return result;
}

unsigned int Matrix::rows() const
{
    return _rows;
}

unsigned int Matrix::columns() const
{
    return _columns;
}

glm::mat4 Matrix::to_mat4() const
{
    if (_rows < 4 || _columns < 4)
    {
        THROW_ERROR("Matrix must have at least 4 rows and 4 columns to be converted to mat4")
    }
    return glm::mat4(_data[0][0], _data[1][0], _data[2][0], _data[3][0],
                     _data[0][1], _data[1][1], _data[2][1], _data[3][1],
                     _data[0][2], _data[1][2], _data[2][2], _data[3][2],
                     _data[0][3], _data[1][3], _data[2][3], _data[3][3]);
}

glm::mat3 Matrix::to_mat3() const
{
    if (_rows < 3 || _columns < 3)
    {
        THROW_ERROR("Matrix must have at least 3 rows and 3 columns to be converted to mat3")
    }
    return glm::mat3(_data[0][0], _data[1][0], _data[2][0],
                     _data[0][1], _data[1][1], _data[2][1],
                     _data[0][2], _data[1][2], _data[2][2]);
}

double& Matrix::operator()(unsigned int i, unsigned int j)
{
    check_range(i,j);
    return _data[i][j];
}

double Matrix::operator()(unsigned int i, unsigned int j) const
{
    check_range(i,j);
    return _data[i][j];
}

Matrix Matrix::operator*(const Matrix& other) const
{
    if (_columns != other.rows())
    {
        THROW_ERROR("Tried to multiply two matrices of incompatible [rows, columns] dimensions: ["+
                    std::to_string(_rows)+","+std::to_string(_columns)+
                    "] and ["+std::to_string(other.rows())+","+std::to_string(other.columns())+"]")
    }
    Matrix result(_rows, other.columns());
    //Looping on matrix cells
    for (unsigned int i=0; i<_rows; i++)
    {
        for (unsigned int j=0; j<other.columns(); j++)
        {
            //calculating cell value
            for (unsigned int k=0; k<_columns; k++)
            {
                result(i,j) += _data[i][k] * other(k,j);
            }
        }
    }
    return result;
}

Matrix& Matrix::operator=(const Matrix& other)
{
    if (other.rows() != _rows || other.columns() != _columns)
    {
        _rows = other.rows();
        _columns = other.columns();
        allocate();
    }
    for (unsigned int i=0; i<_rows; i++)
    {
        for (unsigned int j=0; j<_columns; j++)
        {
            _data[i][j] = other(i,j);
        }
    }
    return *this;
}

namespace GameEngine
{
    std::ostream& operator<<(std::ostream& os, const Matrix& mat)
    {
        for (unsigned int i=0; i<mat.rows(); i++)
        {
            os << "\n{";
            for (unsigned int j=0; j<mat.columns(); j++)
            {
                os << mat(i,j);
                if (j < mat.columns()-1)
                {
                    os << ",\t";
                }
                else
                {
                    os << "}";
                }
            }
        }
        os << std::endl;
        return os;
    }
}

void Matrix::allocate()
{
    deallocate();
    if (_rows == 0 || _columns == 0)
    {
        return;
    }
    _data = new double*[_rows];
    _data[0] = new double[_rows*_columns]{0.};
    for (unsigned int i=1; i<_rows; i++)
    {
        _data[i] = _data[i-1] + _columns;
    }
}

void Matrix::deallocate()
{
    if (_data != nullptr)
    {
        delete[] _data[0];
        delete[] _data;
    }
    _data = nullptr;
}

void Matrix::check_range(unsigned int i, unsigned int j) const
{
    if (i>=_rows || j>=_columns)
    {
        THROW_ERROR("Can't access row "+std::to_string(i)+" and column "+std::to_string(j)
                    +" in matrix with "+std::to_string(_rows)+" rows and "+std::to_string(_columns)+" columns")
    }
}
