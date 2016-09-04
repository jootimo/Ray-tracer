#include <cmath>
#include "Math.h"


template <class T> Vec3<T>::Vec3()
{
    this-> x = this-> y = this-> z = 0;
}

template <class T> Vec3<T>::Vec3(T n)
{
    this-> x = this-> y = this-> z = n;
}

template <class T> Vec3<T>::Vec3(T x, T y, T z)
{
    this-> x = x; 
    this-> y = y;
    this-> z = z;
}

template <class T> Vec3<T> Vec3<T>::operator +(const Vec3 &vec) const
{
    return Vec3<T>(x + vec.x, y + vec.y, z + vec.z);
}

template <class T> Vec3<T> Vec3<T>::operator -(const Vec3 &vec) const
{
    return Vec3<T>(x - vec.x, y - vec.y, z - vec.z);
}

template <class T> Vec3<T> Vec3<T>::operator -() const
{
    return Vec3<T>(-x, -y, -z);
}

template <class T> Vec3<T> Vec3<T>::operator *(const Vec3 &vec) const
{
    return Vec3<T>(x * vec.x, y * vec.y, z * vec.z);
}

template <class T> Vec3<T> Vec3<T>::operator *(const T &n) const
{
    return Vec3<T>(x * n, y * n, z * n);
}

template <class T> Vec3<T>& Vec3<T>::operator *=(const T &n) 
{
    x = x * n; y = y * n; z = z * n;
    return *this; 
}

template <class T> Vec3<T> Vec3<T>::operator /(const T &n) const
{
    return Vec3<T>(x / n, y / n, z / n); 
}


template <class T> const T& Vec3<T>::operator [] (int i) const 
{ 
    return (&x)[i]; 
}

template <class T> T& Vec3<T>::operator [] (int i)
{ 
    return (&x)[i]; 
} 

template <class T> T Vec3<T>::length() const
{
    return sqrt(dot_product(*this));
}

template <class T> T Vec3<T>::dot_product(const  Vec3<T> &vec) const
{
    return x * vec.x + y * vec.y + z * vec.z;
}

template <class T> Vec3<T> Vec3<T>::cross_product(const Vec3 &vec) const
{
    return Vec3<T>(y * vec.z - z * vec.y,
                   z * vec.x - x * vec.z,
                   x * vec.y - y * vec.x);
}

template <class T> Vec3<T>& Vec3<T>::normalize()
{
    T len = length();
    if(len > 0) {
        //Since multiplication is faster than division and normalization is
        //done frequently, this little optimization is in place.
        T inverse = 1 / len;
        x *= inverse; y *= inverse; z *= inverse; 
    }
    return *this;
}


//Have to tell the compiler to compile the class with these types.
//This can be avoided if all the implementation is moved to the header file.  
template class Vec3<float>;
//template class Vec3<double>;
//template class Vec3<int>;


template <class T> Vec2<T>::Vec2()
{
    this->x = this->y = 0;
}

template <class T> Vec2<T>::Vec2(T x, T y)
{
    this->x = x;
    this->y = y;
}

template <class T> Vec2<T> Vec2<T>::operator *(const T &n) const
{
    return Vec2<T>(x * n, y * n);
}

template <class T> Vec2<T> Vec2<T>::operator +(const T &n) const
{
    return Vec2<T>(x + n, y + n);
}

template <class T> Vec2<T> Vec2<T>::operator +(const Vec2 &vec) const
{
    return Vec2<T>(x + vec.x, y + vec.y);
}


template class Vec2<float>;
//template class Vec2<int>;


template <class T> Matrix44<T>::Matrix44() 
{
    //Set as identity matrix
    //matr = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}}; 
}

template <class T> Matrix44<T>::Matrix44(T a, T b, T c, T d, 
                                        T e, T f, T g, T h, 
                                        T i, T j, T k, T l, 
                                        T m, T n, T o, T p)
{
    matr[0][0] = a;
    matr[0][1] = b;
    matr[0][2] = c;
    matr[0][3] = d;
    matr[1][0] = e;
    matr[1][1] = f;
    matr[1][2] = g;
    matr[1][3] = h;
    matr[2][0] = i;
    matr[2][1] = j;
    matr[2][2] = k;
    matr[2][3] = l;
    matr[3][0] = m;
    matr[3][1] = n;
    matr[3][2] = o;
    matr[3][3] = p;   
}

template <class T> const T* Matrix44<T>::operator [](int i) const
{
    return matr[i];
}

template <class T> T* Matrix44<T>::operator [](int i)
{
    return matr[i];
}

template <class T> Matrix44<T> Matrix44<T>::operator *(Matrix44 &m)
    {
    Matrix44<T> result;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            result[i][j] = matr[i][0] * m[0][j] + matr[i][1] * m[1][j] +
                           matr[i][2] * m[2][j] + matr[i][3] * m[3][j];
        }
    }
    return result;
}

template<class T> Vec3<T> Matrix44<T>::multVecMatrix(const Vec3<T> &vec) const
{
    Vec3<T> result;
    T a, b, c, w;

    a = vec[0] * matr[0][0] + vec[1] * matr[1][0] + vec[2] * matr[2][0] + matr [3][0];
    b = vec[0] * matr[0][1] + vec[1] * matr[1][1] + vec[2] * matr[2][1] + matr [3][1];
    c = vec[0] * matr[0][2] + vec[1] * matr[1][2] + vec[2] * matr[2][2] + matr [3][2];
    w = vec[0] * matr[0][3] + vec[1] * matr[1][3] + vec[2] * matr[2][3] + matr [3][3];
    result.x = a / w;
    result.y = b / w;
    result.z = c / w;

    return result;
}

template<class T> Vec3<T> Matrix44<T>::multDirMatrix(const Vec3<T> &vec) const
{
    Vec3<T> result;
    result.x = vec[0] * matr[0][0] + vec[1] * matr[1][0] + vec[2] * matr[2][0];
    result.y = vec[0] * matr[0][1] + vec[1] * matr[1][1] + vec[2] * matr[2][1];
    result.z = vec[0] * matr[0][2] + vec[1] * matr[1][2] + vec[2] * matr[2][2];

    return result;
}

//Using Gaussian elimination
//https://en.wikipedia.org/wiki/Gaussian_elimination
template <class T> Matrix44<T> Matrix44<T>::inverse() const
{
    int i, j, k;
    Matrix44<T> inverted;
    Matrix44<T> m;

    //forward elimination
    for(i = 0; i < 3; i++) {
        int pivot = i;
        T pivotsize = m[i][j];
        if(pivotsize < 0) pivotsize = -pivotsize;

        for(j = i + 1; j < 4; j++) {
            T tmp = m[j][i];        
            if(tmp < 0) tmp = -tmp;
            if(tmp > pivotsize) {
                pivot = j;
                pivotsize = tmp;
            }
        }

        if(pivotsize == 0){
            //cannot invert a singular matrix
            return Matrix44<T>();
        }

        if(pivot != i) {
            for(j = 0; j < 4; j++) {
                T tmp;
                tmp = m[i][j];
                m[i][j] = m[pivot][j];
                m[pivot][j] = tmp;

                tmp = inverted[i][j];
                inverted[i][j] = inverted[pivot][j];
                inverted[pivot][j] = tmp;  
            }
        }
        for(j = i + 1; j < 4; j++) {
            T f = m[j][i] / m[i][j];
            for(k = 0; k < 4; k++) {
                m[j][k] -= f * m[i][k];
                inverted[j][k] -= f * inverted[i][k];
            }
        }
    }

    //backward substitution
    for(i = 3; i >= 0; --i) {
        T f;

        if((f = m[i][j]) == 0) {
            //cannot invert a singular matrix
            return Matrix44<T>();
        }
        for(j = 0; j < 4; j++) {
            m[i][j] /= f;
            inverted[i][j] /= f;
        }
        for(j = 0; j < i; j++) {
            f = m[i][j];
            for(k = 0; k < 4; k++) {
                m[j][k] -= f * m[i][k];
                inverted[j][k] -= f * inverted[i][k];
            }
        }
    }

    return inverted;
}

template class Matrix44<float>;
template class Matrix44<int>;


bool solve_quadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
    //There is a possibility of loss of siqnificance when solving the quadratic equation
    //A safer algorithm for floating point numbers is used here
    //https://en.wikipedia.org/wiki/Loss_of_significance#A_better_algorithm
    float discriminant = b * b - 4 * a * c;
    if(discriminant < 0) {
        return false;   //no roots found
    } else if(discriminant == 0) {
        x0 = -0.5 * b / a;
    } else {
        float q;
        if(b == 0) {
            q = -0.5 * (b + sqrt(discriminant));
        } else {
            q = -0.5 * (b - sqrt(discriminant));
        }
        x0 = q / a;
        x1 = c / q;
    }
    if(x0 > x1) {   //we want smaller first
        float temp = x0;
        x0 = x1;
        x1 = temp;
    } 
    return true;
}