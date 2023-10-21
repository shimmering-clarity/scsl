///
/// \file include/scmp/geom/Vector.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief Linear algebraic vector class.
///
/// Copyright 2017 K. Isom <kyle@imap.cc>
///
/// Permission to use, copy, modify, and/or distribute this software for
/// any purpose with or without fee is hereby granted, provided that
/// the above copyright notice and this permission notice appear in all /// copies.
///
/// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
/// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
/// AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
/// DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
/// OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
/// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
/// PERFORMANCE OF THIS SOFTWARE.
///

#ifndef SCMATH_GEOM_VECTORS_H
#define SCMATH_GEOM_VECTORS_H


#include <array>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <ostream>
#include <iostream>

#include <scmp/Math.h>


// This implementation is essentially a C++ translation of a Python library
// I wrote for Coursera's "Linear Algebra for Machine Learning" course. Many
// of the test vectors come from quiz questions in the class.


namespace scmp {
namespace geom {


/// \brief Vectors represent a direction and Magnitude.
///
/// Vector provides a standard interface for dimensionless fixed-size
/// vectors. Once instantiated, they cannot be modified.
///
/// Note that while the class is templated, it's intended to be used
/// with floating-point types.
///
/// Vectors can be indexed like arrays, and they contain an epsilon
/// value that defines a tolerance for equality.
template<typename T, size_t N>
class Vector {
public:
	/// \brief Construct a unit vector of a given type and size.
	Vector()
	{
		T           unitLength = (T) 1.0 / std::sqrt(N);
		for (size_t i          = 0; i < N; i++) {
			this->arr[i] = unitLength;
		}

		scmp::DefaultEpsilon(this->epsilon);
	}


	/// \brief Construct a Vector with initial values.
	///
	/// If given an initializer_list, the vector is created with
	/// those values. There must be exactly N elements in the list.
	///
	/// \param ilst An intializer list with N elements of type T.
	Vector(std::initializer_list<T> ilst)
	{
		assert(ilst.size() == N);

		scmp::DefaultEpsilon(this->epsilon);
		std::copy(ilst.begin(), ilst.end(), this->arr.begin());
	}


	/// \brief Return the element At index i.
	///
	/// \throws std::out_of_range if the index is out of bounds.
	///
	/// \param index The index of the item to retrieve.
	/// \return The value At the index.
	T At(size_t index) const
	{
		if (index > this->arr.size()) {
			throw std::out_of_range("index " +
						std::to_string(index) + " > " +
						std::to_string(this->arr.size()));
		}
		return this->arr.at(index);
	}


	/// \brief Set a new value for the vector.
	///
	/// This is used to modify the vector in place.
	///
	/// \throws std::out_of_range if the index is out of bounds.
	///
	/// \param index The index to insert the value At.
	/// \param value
	void Set(size_t index, T value)
	{
		if (index > this->arr.size()) {
			throw std::out_of_range("index " +
						std::to_string(index) + " > " +
						std::to_string(this->arr.size()));
		}

		this->arr[index] = value;
	}





	/// \brief Compute the length of the vector.
	///
	/// \return The length of the vector.
	T Magnitude() const
	{
		T result = 0;

		for (size_t i = 0; i < N; i++) {
			result += (this->arr[i] * this->arr[i]);
		}
		return std::sqrt(result);
	}


	/// \brief Set equivalence tolerance.
	///
	/// Set the tolerance for equality checks. At a minimum, this
	/// accounts for systemic errors in floating math arithmetic.
	///
	/// \param eps is the maximum difference between this vector and
	///            another.
	void
	SetEpsilon(T eps)
	{
		this->epsilon = eps;
	}


	/// \brief Determine whether this is a zero vector.
	///
	/// \return true if the vector is zero.
	bool
	IsZero() const
	{
		for (size_t i = 0; i < N; i++) {
			if (!scmp::WithinTolerance(this->arr[i], (T) 0.0, this->epsilon)) {
				return false;
			}
		}
		return true;
	}


	/// \brief Obtain the unit vector for this vector.
	///
	/// \return The unit vector
	Vector
	UnitVector() const
	{
		return *this / this->Magnitude();
	}


	/// \brief Determine if this is a unit vector.
	///
	/// \return true if the vector is a unit vector.
	bool
	IsUnitVector() const
	{
		return scmp::WithinTolerance(this->Magnitude(), (T) 1.0, this->epsilon);
	}


	/// \brief Compute the Angle between two vectors.
	///
	/// \param other Another vector.
	/// \return The Angle in radians between the two vectors.
	T
	Angle(const Vector<T, N> &other) const
	{
		Vector<T, N> unitA = this->UnitVector();
		Vector<T, N> unitB = other.UnitVector();

		// Can't compute angles with a zero vector.
		assert(!this->IsZero());
		assert(!other.IsZero());
		return static_cast<T>(std::acos(unitA * unitB));
	}


	/// \brief Determine whether two vectors are parallel.
	///
	/// \param other Another vector
	/// \return True if the Angle between the vectors is zero.
	bool
	IsParallel(const Vector<T, N> &other) const
	{
		if (this->IsZero() || other.IsZero()) {
			return true;
		}

		T angle = this->Angle(other);
		if (scmp::WithinTolerance(angle, (T) 0.0, this->epsilon)) {
			return true;
		}

		return false;
	}


	/// \brief Determine if two vectors are orthogonal or
	///        perpendicular to each other.
	///
	/// \param other Another vector
	/// \return True if the two vectors are orthogonal.
	bool
	IsOrthogonal(const Vector<T, N> &other) const
	{
		if (this->IsZero() || other.IsZero()) {
			return true;
		}

		return scmp::WithinTolerance(*this * other, (T) 0.0, this->epsilon);
	}


	/// \brief Project this vector onto some basis vector.
	///
	/// \param basis The basis vector to be projected onto.
	/// \return A vector that is the projection of this onto the basis
	///         vector.
	Vector
	ProjectParallel(const Vector<T, N> &basis) const
	{
		Vector<T, N> unit_basis = basis.UnitVector();

		return unit_basis * (*this * unit_basis);
	}


	/// \brief Project this vector perpendicularly onto some basis vector.
	///
	/// This is also called the *rejection* of the vector.
	///
	/// \param basis The basis vector to be projected onto.
	/// \return A vector that is the orthogonal projection of this onto
	///         the basis vector.
	Vector
	ProjectOrthogonal(const Vector<T, N> &basis)
	{
		Vector<T, N> spar = this->ProjectParallel(basis);
		return *this - spar;
	}


	/// \brief Compute the cross product of two vectors.
	///
	/// This is only defined over three-dimensional vectors.
	///
	/// \throw std::out_of_range if this is not a three-dimensional vector.
	///
	/// \param other Another 3D vector.
	/// \return The Cross product vector.
	Vector
	Cross(const Vector<T, N> &other) const
	{
		assert(N == 3);
		if (N != 3) {
			throw std::out_of_range("Cross-product can only called on Vector<T, 3>.");
		}

		return Vector<T, N>{
		    (this->arr[1] * other.arr[2]) - (other.arr[1] * this->arr[2]),
		    -((this->arr[0] * other.arr[2]) - (other.arr[0] * this->arr[2])),
		    (this->arr[0] * other.arr[1]) - (other.arr[0] * this->arr[1])
		};
	}


	/// \brief Vector addition.
	///
	/// \param other The vector to be added.
	/// \return A new vector that is the result of adding this and the
	///         other vector.
	Vector
	operator+(const Vector<T, N> &other) const
	{
		Vector<T, N> vec;

		for (size_t i = 0; i < N; i++) {
			vec.arr[i] = this->arr[i] + other.arr[i];
		}

		return vec;
	}


	/// \brief Vector subtraction.
	///
	/// \param other The vector to be subtracted from this vector.
	/// \return A new vector that is the result of subtracting the
	///         other vector from this one.
	Vector
	operator-(const Vector<T, N> &other) const
	{
		Vector<T, N> vec;

		for (size_t i = 0; i < N; i++) {
			vec.arr[i] = this->arr[i] - other.arr[i];
		}

		return vec;
	}


	/// \brief Scalar multiplication.
	///
	/// \param k The scaling value.
	/// \return A new vector that is this vector scaled by k.
	Vector
	operator*(const T k) const
	{
		Vector<T, N> vec;

		for (size_t i = 0; i < N; i++) {
			vec.arr[i] = this->arr[i] * k;
		}

		return vec;
	}


	/// \brief Scalar division.
	///
	/// \param k The scaling value
	/// \return A new vector that is this vector scaled by 1/k.
	Vector
	operator/(const T k) const
	{
		Vector<T, N> vec;

		for (size_t i = 0; i < N; i++) {
			vec.arr[i] = this->arr[i] / k;
		}

		return vec;
	}


	/// \brief Compute the Dot product between two vectors.
	///
	/// \param other The other vector.
	/// \return A scalar value that is the Dot product of the two vectors.
	T
	operator*(const Vector<T, N> &other) const
	{
		T result = 0;

		for (size_t i = 0; i < N; i++) {
			result += (this->arr[i] * other.arr[i]);
		}

		return result;
	}


	/// \brief Vector equivalence
	///
	/// \param other The other vector.
	/// \return Return true if all the components of both vectors are
	///         within the tolerance value.
	bool
	operator==(const Vector<T, N> &other) const
	{
		for (size_t i = 0; i < N; i++) {
			if (!scmp::WithinTolerance(this->arr[i], other.arr[i], this->epsilon)) {
				return false;
			}
		}
		return true;
	}


	/// \brief Vector non-equivalence.
	///
	/// \param other The other vector.
	/// \return Return true if any of the components of both vectors are
	///         not within the tolerance value.
	bool
	operator!=(const Vector<T, N> &other) const
	{
		return !(*this == other);
	}


	/// \brief Array indexing into vector.
	///
	/// Note that the values of the vector cannot be modified.
	/// Instead, something like the following must be done:
	///
	/// ```
	/// Vector3D	a {1.0, 2.0, 3.0};
	/// Vector3D	b {a[0], a[1]*2.0, a[2]};
	/// ```
	///
	/// \param i The component index.
	/// \return The value of the vector component At i.
	const T &
	operator[](size_t i) const
	{
		return this->arr[i];
	}


	/// \brief Write a vector a stream in the form "<i, j, ...>".
	///
	/// \param outs An output stream.
	/// \param vec The vector to be formatted.
	/// \return The output stream.
	friend std::ostream &
	operator<<(std::ostream &outs, const Vector<T, N> &vec)
	{
		outs << "<";
		for (size_t i = 0; i < N; i++) {
			outs << vec.arr[i];
			if (i < (N - 1)) {
				outs << ", ";
			}
		}
		outs << ">";
		return outs;
	}

private:
	static const size_t dim = N;
	T                   epsilon;
	std::array<T, N>    arr;
};

///
/// \defgroup vector_aliases Vector type aliases.
///

/// \ingroup vector_aliases
/// A number of shorthand aliases for vectors are provided. They follow
/// the form of VectorNt, where N is the dimension and t is the type.
/// For example, a 2D float vector is Vector2F.

/// \ingroup vector_aliases
/// \brief Type alias for a two-dimensional float vector.
typedef Vector<float, 2> Vector2F;

/// \ingroup vector_aliases
/// \brief Type alias for a three-dimensional float vector.
typedef Vector<float, 3> Vector3F;

/// \ingroup vector_aliases
/// \brief Type alias for a four-dimensional float vector.
typedef Vector<float, 4> Vector4F;

/// \ingroup vector_aliases
/// \brief Type alias for a two-dimensional double vector.
typedef Vector<double, 2> Vector2D;

/// \ingroup vector_aliases
/// \brief Type alias for a three-dimensional double vector.
typedef Vector<double, 3> Vector3D;

/// \ingroup vector_aliases
/// \brief Type alias for a four-dimensional double vector.
typedef Vector<double, 4> Vector4D;


} // namespace geom
} // namespace scmp


#endif // SCMATH_GEOM_VECTORS_H
