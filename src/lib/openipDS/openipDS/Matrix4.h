/* 
 * File:   Matrix4.h
 * Author: Dzsekob
 *
 * Created on 2010. szeptember 28., 13:47
 */

#ifndef MATRIX4_H
#define	MATRIX4_H

#include <openipDS/MatrixNN.h>
#include <openipDS/Vector4.h>

namespace openip
{
    /**
     * Matrix4 represents a static 3x3 Matrix of template type T
     */
    template<typename T>
    class Matrix4
    {
    public:
        /**
         * default constructor
         */
        Matrix4();

        /**
         * copy constructor
         * @param m object to copy
         */
        Matrix4(const Matrix4& m);

        /**
         * constructor based on a pointer; the first 16 elements of the
         * memory area will be used as the elements of the matrix
         * @param data pointer to at least 16 elements of type T
         */
        Matrix4(T* data);

        /**
         * constructor based on 16 parameters representing the elements of
         * the matrix
         * @param a element (0,0)
         * @param b element (0,1)
         * @param c element (0,2)
         * @param d element (0,3)
         * @param e element (1,0)
         * @param f element (1,1)
         * @param g element (1,2)
         * @param h element (1,3)
         * @param i element (2,0)
         * @param j element (2,1)
         * @param k element (2,2)
         * @param l element (2,3)
         * @param m element (3,0)
         * @param n element (3,1)
         * @param o element (3,2)
         * @param p element (3,3)
         */
        Matrix4(T a, T b, T c, T d, T e, T f, T g, T h, T i, T j, T k, T l, T m, T n, T o, T p);

        /**
         * destructor
         */
        ~Matrix4();

        /**
         * assignment operator
         * @param b the instance to copy
         */
        Matrix4& operator=(const Matrix4& b);

        /**
         * adds the values of the paramter to this instance point-wise
         *
         * @param b the instance to add
         * @return this instance
         */
        Matrix4& operator+=(const Matrix4& b);

        /**
         * subtracts the values of the paramter to this instance point-wise
         *
         * @param b the instance to subtract
         * @return this instance
         */
        Matrix4& operator-=(const Matrix4& b);

        /**
         * multiplies the values of the paramter to this instance point-wise
         *
         * @param b the instance to multiply
         * @return this instance
         */
        Matrix4& operator*=(const Matrix4& b);

        /**
         * multiplies the parameter to this matrix with row-column composition
         *
         * @param b the matrix to multiply with
         * @return this
         */
        Matrix4& operator^=(const Matrix4& b);

        /**
         * divides the values of the paramter to this instance point-wise
         *
         * @param b the instance to divide
         * @return this instance
         */
        Matrix4& operator/=(const Matrix4& b);

        /**
         * sets the values of the matrix to a given constant
         *
         * @param t constant to set the elements to
         * @return this
         */
        Matrix4& operator=(const T& t);

        /**
         * adds a constant to the elements of the matrix point-wise
         *
         * @param t the constant to add
         * @return this
         */
        Matrix4& operator+=(const T& t);

        /**
         * subtracts a constant from the elements of the matrix point-wise
         *
         * @param t the constant to subtract
         * @return this
         */
        Matrix4& operator-=(const T& t);

        /**
         * multiplies a constant with the elements of the matrix point-wise
         *
         * @param t the constant to multiply with
         * @return this
         */
        Matrix4& operator*=(const T& t);

        /**
         * divides the elements of the matrix with a constant point-wise
         *
         * @param t the constant to divide with
         * @return this
         */
        Matrix4& operator/=(const T& t);

        /**
         * adds two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix4 operator+(const Matrix4& b) const;

        /**
         * substracts two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix4 operator-(const Matrix4& b) const;

        /**
         * multiplies two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix4 operator*(const Matrix4& b) const;

        /**
         * divides two matrices point-wise and returns the new instance
         *
         * @param b the second operand
         * @return the new instance
         */
        Matrix4 operator/(const Matrix4& b) const;

        /**
         * adds a constant to the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix4 operator+(const T& t) const;

        /**
         * subtracts a constant from the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix4 operator-(const T& t) const;

        /**
         * multiplies a constant with the matrix and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix4 operator*(const T& t) const;

        /**
         * divides the elements of the matrix with a constant and returns a new instance
         *
         * @param t the second operand
         * @return the new instance
         */
        Matrix4 operator/(const T& t) const;

        /**
         * (0,0)th element
         */
        T a;

        /**
         * (0,1)th element
         */
        T b;

        /**
         * (0,2)th element
         */
        T c;

        /**
         * (0,3)th element
         */
        T d;

        /**
         * (1,0)th element
         */
        T e;

        /**
         * (1,1)th element
         */
        T f;

        /**
         * (1,2)th element
         */
        T g;

        /**
         * (1,3)th element
         */
        T h;

        /**
         * (2,0)th element
         */
        T i;

        /**
         * (2,1)th element
         */
        T j;

        /**
         * (2,2)th element
         */
        T k;

        /**
         * (2,3)th element
         */
        T l;

        /**
         * (3,0)th element
         */
        T m;

        /**
         * (3,1)th element
         */
        T n;

        /**
         * (3,2)th element
         */
        T o;

        /**
         * (3,3)th element
         */
        T p;

    };

    /**
     * matrix multiplication with row-column composition
     * @param a left operand
     * @param b right operand
     * @return result as new matrix instance
     */
    template<typename T>
    Matrix4<T> operator^(const Matrix4<T>& a, const Matrix4<T>& b)
    {
        Matrix4<T> tmp;
        tmp.a= a.a*b.a + a.b*b.e + a.c*b.i + a.d * b.m;
        tmp.b= a.a*b.b + a.b*b.f + a.c*b.j + a.d * b.n;
        tmp.c= a.a*b.c + a.b*b.g + a.c*b.k + a.d * b.o;
        tmp.d= a.a*b.d + a.b*b.h + a.c*b.l + a.d * b.p;

        tmp.e= a.e*b.a + a.f*b.e + a.g*b.i + a.h * b.m;
        tmp.f= a.e*b.b + a.f*b.f + a.g*b.j + a.h * b.n;
        tmp.g= a.e*b.c + a.f*b.g + a.g*b.k + a.h * b.o;
        tmp.h= a.e*b.d + a.f*b.h + a.g*b.l + a.h * b.p;

        tmp.i= a.i*b.a + a.j*b.e + a.k*b.i + a.l * b.m;
        tmp.j= a.i*b.b + a.j*b.f + a.k*b.j + a.l * b.n;
        tmp.k= a.i*b.c + a.j*b.g + a.k*b.k + a.l * b.o;
        tmp.l= a.i*b.d + a.j*b.h + a.k*b.l + a.l * b.p;

        tmp.m= a.m*b.a + a.n*b.e + a.o*b.i + a.p * b.m;
        tmp.n= a.m*b.b + a.n*b.f + a.o*b.j + a.p * b.n;
        tmp.o= a.m*b.c + a.n*b.g + a.o*b.k + a.p * b.o;
        tmp.p= a.m*b.d + a.n*b.h + a.o*b.l + a.p * b.p;

        return tmp;
    }

    /**
     * matrix vector multiplication with row-column composition
     * @param m left matrix operand
     * @param v right vector operand
     * @return result vector as new instance
     */
    template<typename T>
    Vector4<T> operator^(const Matrix4<T>& m, const Vector4<T>& v)
    {
        Vector4<T> tmp;

        tmp.x()= m.a*v.x() + m.b*v.y() + m.c*v.z() + m.d*v.w();
        tmp.y()= m.e*v.x() + m.f*v.y() + m.g*v.z() + m.h*v.w();
        tmp.z()= m.i*v.x() + m.j*v.y() + m.k*v.z() + m.l*v.w();
        tmp.w()= m.m*v.x() + m.n*v.y() + m.o*v.z() + m.p*v.w();

        return tmp;
    }

    /**
     * vector matrix multiplication with row-column composition
     * @param v left vector operand
     * @param m right matrix operand
     * @return result vector as new instance
     */
    template<typename T>
    Vector4<T> operator^(const Vector4<T>& v, const Matrix4<T>& m)
    {
        Vector4<T> tmp;

        tmp.x()= v.x()*m.a + v.y()*m.e + v.z()*m.i + v.w()*m.m;
        tmp.y()= v.x()*m.b + v.y()*m.f + v.z()*m.j + v.w()*m.n;
        tmp.z()= v.x()*m.c + v.y()*m.g + v.z()*m.k + v.w()*m.o;
        tmp.w()= v.x()*m.d + v.y()*m.h + v.z()*m.l + v.w()*m.p;

        return tmp;
    }

    template<class T>
    Matrix4<T>::Matrix4()
    {
        a= b= c= d= e= f= g= h= i= j= k= l= m= n= o= p= 0;
    }

    template<class T>
    Matrix4<T>::Matrix4(const Matrix4& b)
    {
        this->a= b.a;
        this->b= b.b;
        this->c= b.c;
        this->d= b.d;
        this->e= b.e;
        this->f= b.f;
        this->g= b.g;
        this->h= b.h;
        this->i= b.i;
        this->j= b.j;
        this->k= b.k;
        this->l= b.l;
        this->m= b.m;
        this->n= b.n;
        this->o= b.o;
        this->p= b.p;
    }

    template<typename T>
    Matrix4<T>::Matrix4(T* data)
    {
        this->a= data[0];
        this->b= data[1];
        this->c= data[2];
        this->d= data[3];
        this->e= data[4];
        this->f= data[5];
        this->g= data[6];
        this->h= data[7];
        this->i= data[8];
        this->j= data[9];
        this->k= data[10];
        this->l= data[11];
        this->m= data[12];
        this->n= data[13];
        this->o= data[14];
        this->p= data[15];
    }

    template<typename T>
    Matrix4<T>::Matrix4(T a, T b, T c, T d, T e, T f, T g, T h, T i, T j, T k, T l, T m, T n, T o, T p)
    {
        this->a= a;
        this->b= b;
        this->c= c;
        this->d= d;
        this->e= e;
        this->f= f;
        this->g= g;
        this->h= h;
        this->i= i;
        this->j= j;
        this->k= k;
        this->l= l;
        this->m= m;
        this->n= n;
        this->o= o;
        this->p= p;
    }


    template<class T>
    Matrix4<T>::~Matrix4()
    {
    }

    template<typename T>
    Matrix4<T>& Matrix4<T>::operator=(const Matrix4<T>& b)
    {
        this->a= b.a;
        this->b= b.b;
        this->c= b.c;
        this->d= b.d;
        this->e= b.e;
        this->f= b.f;
        this->g= b.g;
        this->h= b.h;
        this->i= b.i;
        this->j= b.j;
        this->k= b.k;
        this->l= b.l;
        this->m= b.m;
        this->n= b.n;
        this->o= b.o;
        this->p= b.p;

        return *this;
    }

    template<typename T>
    Matrix4<T>& Matrix4<T>::operator+=(const Matrix4<T>& b)
    {
        this->a+= b.a;
        this->b+= b.b;
        this->c+= b.c;
        this->d+= b.d;
        this->e+= b.e;
        this->f+= b.f;
        this->g+= b.g;
        this->h+= b.h;
        this->i+= b.i;
        this->j+= b.j;
        this->k+= b.k;
        this->l+= b.l;
        this->m+= b.m;
        this->n+= b.n;
        this->o+= b.o;
        this->p+= b.p;

        return *this;
    }

    template<typename T>
    Matrix4<T>& Matrix4<T>::operator-=(const Matrix4<T>& b)
    {
        this->a-= b.a;
        this->b-= b.b;
        this->c-= b.c;
        this->d-= b.d;
        this->e-= b.e;
        this->f-= b.f;
        this->g-= b.g;
        this->h-= b.h;
        this->i-= b.i;
        this->j-= b.j;
        this->k-= b.k;
        this->l-= b.l;
        this->m-= b.m;
        this->n-= b.n;
        this->o-= b.o;
        this->p-= b.p;

        return *this;
    }

    template<typename T>
    Matrix4<T>& Matrix4<T>::operator*=(const Matrix4<T>& b)
    {
        this->a*= b.a;
        this->b*= b.b;
        this->c*= b.c;
        this->d*= b.d;
        this->e*= b.e;
        this->f*= b.f;
        this->g*= b.g;
        this->h*= b.h;
        this->i*= b.i;
        this->j*= b.j;
        this->k*= b.k;
        this->l*= b.l;
        this->m*= b.m;
        this->n*= b.n;
        this->o*= b.o;
        this->p*= b.p;

        return *this;
    }

    template<typename T>
    Matrix4<T>& Matrix4<T>::operator^=(const Matrix4<T>& b)
    {
        this->a= this->a*b.a + this->b*b.e + this->c*b.i + this->d*b.m;
        this->b= this->a*b.b + this->b*b.f + this->c*b.j + this->d*b.n;
        this->c= this->a*b.c + this->b*b.g + this->c*b.k + this->d*b.o;
        this->d= this->a*b.d + this->b*b.h + this->c*b.l + this->d*b.p;

        this->e= this->e*b.a + this->f*b.e + this->g*b.i + this->h*b.m;
        this->f= this->e*b.b + this->f*b.f + this->g*b.j + this->h*b.n;
        this->g= this->e*b.c + this->f*b.g + this->g*b.k + this->h*b.o;
        this->h= this->e*b.d + this->f*b.h + this->g*b.l + this->h*b.p;

        this->i= this->i*b.a + this->j*b.e + this->k*b.i + this->l*b.m;
        this->j= this->i*b.b + this->j*b.f + this->k*b.j + this->l*b.n;
        this->k= this->i*b.c + this->j*b.g + this->k*b.k + this->l*b.o;
        this->l= this->i*b.d + this->j*b.h + this->k*b.l + this->l*b.p;

        this->m= this->m*b.a + this->n*b.e + this->o*b.i + this->p*b.m;
        this->n= this->m*b.b + this->n*b.f + this->o*b.j + this->p*b.n;
        this->o= this->m*b.c + this->n*b.g + this->o*b.k + this->p*b.o;
        this->p= this->m*b.d + this->n*b.h + this->o*b.l + this->p*b.p;

        return *this;
    }

    template<typename T>
    Matrix4<T>& Matrix4<T>::operator/=(const Matrix4<T>& b)
    {
        this->a/= b.a;
        this->b/= b.b;
        this->c/= b.c;
        this->d/= b.d;
        this->e/= b.e;
        this->f/= b.f;
        this->g/= b.g;
        this->h/= b.h;
        this->i/= b.i;
        this->j/= b.j;
        this->k/= b.k;
        this->l/= b.l;
        this->m/= b.m;
        this->n/= b.n;
        this->o/= b.o;
        this->p/= b.p;

        return *this;
    }

    template<typename T>
    Matrix4<T>& Matrix4<T>::operator=(const T& t)
    {
        this->a= t;
        this->b= t;
        this->c= t;
        this->d= t;
        this->e= t;
        this->f= t;
        this->g= t;
        this->h= t;
        this->i= t;
        this->j= t;
        this->k= t;
        this->l= t;
        this->m= t;
        this->n= t;
        this->o= t;
        this->p= t;

        return *this;
    }

    template<typename T>
    Matrix4<T>& Matrix4<T>::operator+=(const T& t)
    {
        this->a+= t;
        this->b+= t;
        this->c+= t;
        this->d+= t;
        this->e+= t;
        this->f+= t;
        this->g+= t;
        this->h+= t;
        this->i+= t;
        this->j+= t;
        this->k+= t;
        this->l+= t;
        this->m+= t;
        this->n+= t;
        this->o+= t;
        this->p+= t;

        return *this;
    }

    template<typename T>
    Matrix4<T>& Matrix4<T>::operator-=(const T& t)
    {
        this->a-= t;
        this->b-= t;
        this->c-= t;
        this->d-= t;
        this->e-= t;
        this->f-= t;
        this->g-= t;
        this->h-= t;
        this->i-= t;
        this->j-= t;
        this->k-= t;
        this->l-= t;
        this->m-= t;
        this->n-= t;
        this->o-= t;
        this->p-= t;


        return *this;
    }

    template<typename T>
    Matrix4<T>& Matrix4<T>::operator*=(const T& t)
    {
        this->a*= t;
        this->b*= t;
        this->c*= t;
        this->d*= t;
        this->e*= t;
        this->f*= t;
        this->g*= t;
        this->h*= t;
        this->i*= t;
        this->j*= t;
        this->k*= t;
        this->l*= t;
        this->m*= t;
        this->n*= t;
        this->o*= t;
        this->p*= t;

        return *this;
    }

    template<typename T>
    Matrix4<T>& Matrix4<T>::operator/=(const T& t)
    {
        this->a/= t;
        this->b/= t;
        this->c/= t;
        this->d/= t;
        this->e/= t;
        this->f/= t;
        this->g/= t;
        this->h/= t;
        this->i/= t;
        this->j/= t;
        this->k/= t;
        this->l/= t;
        this->m/= t;
        this->n/= t;
        this->o/= t;
        this->p/= t;

        return *this;
    }

    template<typename T>
    Matrix4<T> Matrix4<T>::operator+(const Matrix4<T>& b) const
    {
        Matrix4<T> n(*this);
        return n+= b;
    }

    template<typename T>
    Matrix4<T> Matrix4<T>::operator-(const Matrix4<T>& b) const
    {
        Matrix4<T> n(*this);
        return n-= b;
    }

    template<typename T>
    Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& b) const
    {
        Matrix4<T> n(*this);
        return n*= b;
    }

    template<typename T>
    Matrix4<T> Matrix4<T>::operator/(const Matrix4<T>& b) const
    {
        Matrix4<T> n(*this);
        return n/= b;
    }

    template<typename T>
    Matrix4<T> Matrix4<T>::operator+(const T& t) const
    {
        Matrix4<T> n(*this);
        return n+= t;
    }

    template<typename T>
    Matrix4<T> Matrix4<T>::operator-(const T& t) const
    {
        Matrix4<T> n(*this);
        return n-= t;
    }

    template<typename T>
    Matrix4<T> Matrix4<T>::operator*(const T& t) const
    {
        Matrix4<T> n(*this);
        return n*= t;
    }

    template<typename T>
    Matrix4<T> Matrix4<T>::operator/(const T& t) const
    {
        Matrix4<T> n(*this);
        return n/= t;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& o, const Matrix4<T>& m)
    {
        o << m.a;
        o << ", ";
        o << m.b;
        o << ", ";
        o << m.c;
        o << ", ";
        o << m.d;
        o << ", ";
        o << m.e;
        o << ", ";
        o << m.f;
        o << ", ";
        o << m.g;
        o << ", ";
        o << m.h;
        o << ", ";
        o << m.i;
        o << ", ";
        o << m.j;
        o << ", ";
        o << m.k;
        o << ", ";
        o << m.l;
        o << ", ";
        o << m.m;
        o << ", ";
        o << m.n;
        o << ", ";
        o << m.o;
        o << ", ";
        o << m.p;
        o << "]";

        return o;
    }
}

#endif	/* MATRIX4_H */

