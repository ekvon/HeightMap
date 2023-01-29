/*
	File:Matrix.h;Version:2.0;Checked:07/05/14;
	Realization of matrix.
*/
#ifndef __Math__MATRIX__H
#define __Math__MATRIX__H

#include <math.h>

#include "array.h"

using namespace storage;

namespace math
{
    template<class T>
    class Matrix :public Array<T>
    {
    public:
        typedef Array<T> array_type;
        typedef typename array_type::size_type size_type;
        typedef typename array_type::const_reference const_reference;
        typedef typename array_type::reference reference;
        typedef typename array_type::value_type value_type;
        typedef typename array_type::pointer pointer;
        typedef typename array_type::const_pointer const_pointer;
    public:
        Matrix(size_type size1 = 0, size_type size2 = 0) :
            array_type(size1*size2),
            m_size1(size1),
            m_size2(size2) {
        }
        Matrix(size_type size1, size_type size2, pointer p) :
            array_type(size1*size2,p),
            m_size1(size1),
            m_size2(size2) {
        }
        Matrix(const Matrix &m) {
            this->resize(m.size1(), m.size2(), false);
            for (size_type i = 0; i < m_size1; i++) {
                for (size_type j = 0; j < m_size2; j++) {
                    this->insert_element(i, j, m.const_at(i*m_size2 + j));
                }
            }
        }
        ~Matrix() {
        }
        /*	Valid expressions for Matrix Expression concept	*/
    public:
        /*	Assignment	*/
        Matrix& assign(const Matrix &m) {
            /*	just copy region of memory	*/
            this->_init(m.m_size1*m.m_size2, m.m_data);
            return *this;
        }
        Matrix& operator=(const Matrix &m) {
            this->assign(m);
        }
        /*	Computed assignment	*/
        Matrix& plus_assign(const Matrix &m) {
            for (size_type i = 0; i < m_size1; i++) {
                for (size_type j = 0; j < m_size2; j++) {
                    size_t off = i * m_size2 + j;
                    reference ref = *(this->m_data + off);
                    ref += m.const_at(off);
                }
            }
            return *this;
        }
        Matrix& operator+=(const Matrix &m) {
            return this->plus_assign(m);
        }
        Matrix& minus_assign(const Matrix &m) {
            for (size_type i = 0; i < m_size1; i++) {
                for (size_type j = 0; j < m_size2; j++) {
                    size_t off = i * m_size2 + m_size1;
                    reference ref = *(this->m_data + off);
                    ref -= m.at(off);
                }
            }
            return *this;
        }
        Matrix& operator-=(const Matrix &m) {
            return this->minus_assign(m);
        }
        Matrix& operator*=(T t) {
            /*	not realized yet	*/
            return *this;
        }
        /*	Size	*/
        size_type size1()const {
            return m_size1;
        }
        size_type size2()const {
            return m_size2;
        }
        void swap(Matrix &m) {
            /*	not realized yet	*/
        }
        /*	element access	*/
        reference operator()(size_type i, size_type j) {
            /*	precondition	*/
            if (!m_size1 || !m_size2 || m_size1 <= i || m_size2 <= j)
                return this->m_default;
            return this->at(i*m_size2 + j);
        }
        const_reference operator()(size_type i, size_type j) const {
            if (!m_size1 || !m_size2 || m_size1 <= i || m_size2 <= j)
                return this->m_default;
            return this->const_at(i*m_size2 + j);
        }
        /*	Valid expressions for Matrix concept	*/
    public:
        void insert_element(size_type i, size_type j, const_reference t) {
            this->at(i*m_size2 + j) = t;
        }
        void erase_element(size_type i, size_type j) {
            /*	default constructible	*/
            this->at(i*m_size2 + j) = value_type();
        }
        const array_type & data()const {
            return *this;
        }
        array_type & data() {
            return *this;
        }
        void resize(size_type size1, size_type size2, bool preserve = true) {
            m_size1 = size1;
            m_size2 = size2;
            array_type::resize(m_size1*m_size2);
            /*	preservation is not realized yet	*/
        }
        void clear() {
            array_type::clear();
        }
    public:
        //	add to i-th row j-th row multiplied on constant
        void addRow(int i, int j, const T &t) {
        }
        //	add to i-th column j-th column multiplied on constant
        void addColumn(int i, int j, const T &t) {
        }
        //	maximum of row numbers and column numbers
        unsigned int order()const {
            return m_size1 < m_size2 ? m_size1 : m_size2;
        }

        /*	Convert to string representation	*/
        std::string toString() const {
            size_type size1 = this->size1();
            size_type size2 = this->size2();
            if (!size1 || !size2) {
                return "[]";
            }
            std::stringbuf buf;
            std::ostream os(&buf);
            os << "[\n";
            for (size_type i = 0; i < size1; i++) {
                /*	add first element without preceeding comma	*/
                os << "\t" << this->const_at(i*m_size2);
                for (size_type j = 1; j < size2; j++) {
                    os << "," << this->const_at(i*m_size2 + j);
                }
                os << "\n";
            }
            os << "]";
            return buf.str();
        }
        std::string toJSON() const {
            /*	not realized yet	*/
            return "";
        }
    protected:
        size_type m_size1;
        size_type m_size2;
    };

    /*
     * External algebraic operations.
     * Template parameter _Matrix should satistfy to Matrix Expression Concept.
     */

    /*	transposition	*/
    /*template<typename _Matrix>
    _Matrix transpose(const _Matrix &m) {
        typedef _Matrix matrix_type;
        typedef typename matrix_type::value_type value_type;
        typedef typename matrix_type::size_type size_type;
        size_type size1 = m.size1();
        size_type size2 = m.size2();
        matrix_type result(size2, size1);
        for (size_type i = 0; i < size2; i++) {
            for (size_type j = 0; j < size1; j++) {
                result.insert_element(i, j, m(j, i));
            }
        }
        return result;
    }

    template <class _Matrix>
    _Matrix operator~(const _Matrix &m) {
        return transpose(m);
    }

    template <typename _Matrix>
    _Matrix pseudoinverse(const _Matrix &m) {
        typedef _Matrix matrix_type;
        return inverse(transpose(m)*m)*transpose(m);
    }*/

    template<class _Matrix>
    _Matrix operator +(const _Matrix &a, const _Matrix& b) {
        typedef _Matrix matrix_type;
        typedef typename matrix_type::value_type value_type;
        typedef typename matrix_type::size_type size_type;
        /*	addition	*/
        if (a.size1() != b.size1() || a.size2() != b.size2())
            /*	DefaultConstructible	*/
            return matrix_type();
        size_type size1 = a.size1();
        size_type size2 = a.size2();
        matrix_type result(size1, size2);
        for (size_type i = 0; i < size1; i++) {
            for (size_type j = 0; j < size2; j++) {
                result.insert_element(i, j, a(i, j) + b(i, j));
            }
        }
        return result;
    }

    template<typename _Matrix>
    _Matrix operator *(const _Matrix &a, const _Matrix &b) {
        typedef _Matrix matrix_type;
        typedef typename matrix_type::value_type value_type;
        typedef typename matrix_type::size_type size_type;
        /*	multiplication	*/
        if (a.size1() != b.size2())
            return matrix_type();
        /*	the sum of products	*/
        value_type value = value_type();
        unsigned int M = a.size1();
        unsigned int K = a.size2();
        unsigned int N = b.size2();
        matrix_type result(M, N);
        for (size_type i = 0; i < M; i++) {
            for (size_type j = 0; j < N; j++) {
                for (size_type k = 0; k < K; k++) {
                    value += a(i, k)*b(k, j);
                }
                result.insert_element(i, j, value);
                /*	nulling the sum of products	*/
                value = value_type();
            }
        }
        return result;
    }

    /*
     * External functions to create rotation matrices around specified Cartesian axis.
     * Angles MUST be specified in radians.
     */
    template <typename _Matrix>
    _Matrix rotationZ(double angle){
        double data[16]={cos(angle),sin(angle),0,0,-sin(angle),cos(angle),0,0,0,0,1,0,0,0,0,1};
        return _Matrix(4,4,data);
    }

    template <typename _Matrix>
    _Matrix rotationY(double angle){
        double data[16]={cos(angle),0,sin(angle),0,0,1,0,0,-sin(angle),0,cos(angle),0,0,0,0,1};
        return _Matrix(4,4,data);
    }

    template <typename _Matrix>
    _Matrix rotationX(double angle){
        double data[16]={1,0,0,0,0,cos(angle),sin(angle),0,0,-sin(angle),cos(angle),0,0,0,0,1};
        return _Matrix(4,4,data);
    }

    template <typename _Matrix>
    _Matrix translation(double x,double y,double z){
        double data[16]={0,0,0,x,0,0,0,y,0,0,0,z,0,0,0,0};
        return _Matrix(4,4,data);
    }
}
#endif//__Math__MATRIX__H
