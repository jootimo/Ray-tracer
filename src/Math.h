#pragma once

#include <iostream>

//A class for vectors of lenght 3. Can also simulate a point in 3 space
template <class T> class Vec3 {
public:
    T x, y, z;
    Vec3();
    Vec3(T n);
    Vec3(T x, T y, T z);
    Vec3 operator +(const Vec3 &vec) const;
    Vec3 operator -(const Vec3 &vec) const;
    Vec3 operator -() const;
    Vec3 operator *(const Vec3 &vec) const;
    Vec3 operator *(const T &n) const;
    Vec3& operator *=(const T &n);
    Vec3 operator /(const T &n) const;
    //Accessor operators
    const T& operator [](int i) const;
    T& operator [](int i); 
    T length() const;
    T dot_product(const Vec3 &vec) const;
    Vec3 cross_product(const Vec3 &vec) const;
    Vec3& normalize();
};

template <class T> class Vec2 {
    public:
     T x, y;
     Vec2();
     Vec2(T x, T y);
     Vec2 operator *(const T &n) const;
     Vec2 operator +(const T &n) const;
     Vec2 operator +(const Vec2 &vec) const;
};

template <class T> class Matrix44 {
private:
    T matr[4][4] = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
public:
    T a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;
    Matrix44();
    Matrix44(T a, T b, T c, T d, 
             T e, T f, T g, T h, 
             T i, T j, T k, T l, 
             T m, T n, T o, T p);
    //Accessor operators for both dimensions
    const T* operator [](int i) const;
    T* operator [](int i);
    Matrix44 operator *(Matrix44 &m); 
    //Function for multiplying a matrix with a point
    Vec3<T> multVecMatrix(const Vec3<T> &vec) const;
    //Function for multiplying a matrix with a point
    Vec3<T> multDirMatrix(const Vec3<T> &vec) const;
    Matrix44 inverse() const; 
 
};

//Solve qadratic equation of form ax^2+bx+c and store the roots in x0 and x1.
//Returns false if no roots are found.
bool solve_quadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

//Multiply number by Vec3
template <class T> Vec3<T> mult_number_Vec3(const T &n, const Vec3<T> &vec)
{ return Vec3<T>(vec.x * n, vec.y * n, vec.z * n); }

//Divide number by Vec3
template <class T> Vec3<T> div_number_Vec3(const T &n, const Vec3<T> &vec)
{ return Vec3<T>(n / vec.x, n / vec.y, n / vec.z); }
