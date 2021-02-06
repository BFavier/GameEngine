#pragma once
#include <GameEngine/Utilities.hpp>
#include <GameEngine/Macro.hpp>

namespace GameEngine
{
    class Vector;
    class Referential;

    class Quaternion
    {
    public:
        Quaternion();
        Quaternion(const Quaternion& other);
        Quaternion(double w, double x, double y, double z);
        Quaternion(double angle, const Vector& axis);
        Quaternion(const Vector& from, const Vector& to);
        ~Quaternion();
    public:
        double w = 1.;
        double x = 0.;
        double y = 0.;
        double z = 0.;
    public:
        ///< Return the norm of the quaternion
        double norm() const;
        ///< Normalize the quaternion so that it's norm is 1
        void normalize();
        ///< Returns the quaternion of unit norm
        Quaternion normed() const;
        ///< Returns the conjugate
        Quaternion conjugate() const;
        ///< Returns the inverted quaternion
        Quaternion inverted() const;
        ///< Returns true if the two quaternions are close enough to be considered equals
        static bool equal(const Quaternion& q1, const Quaternion& q2, double tolerance=1.0E-6);
        ///< Dot product of two quaternions
        static double dot(const Quaternion& q1, const Quaternion& q2);
        ///< SLERP of two quaternions (Spherical Linear intERPolation). q1 has weight (1-t) and q2 has weight t.
        static Quaternion SLERP(const Quaternion& q1, const Quaternion& q2, double t);
    public:
        friend std::ostream& operator<<(std::ostream& os, const Quaternion& rot);
        Quaternion& operator=(const Quaternion& other);
        Quaternion operator*(const Quaternion& other) const;
        Vector operator*(const Vector& vector) const;
        Quaternion operator*(double other) const;
        friend Quaternion operator*(double other, const Quaternion& quat);
        Quaternion& operator*=(double other);
        Quaternion operator/(double other) const;
        Quaternion& operator/=(double other);
        Quaternion operator+(const Quaternion& other) const;
        Quaternion& operator+=(const Quaternion& other);
        Quaternion operator-(const Quaternion& other) const;
        Quaternion operator-() const;
        Quaternion& operator-=(const Quaternion& other);
    };
}