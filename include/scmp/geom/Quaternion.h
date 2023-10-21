///
/// \file include/scmp/geom/Quaternion.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2019-08-05
/// \brief Quaternion implementation suitable for navigation in R3.
///
/// Copyright 2019 K. Isom <kyle@imap.cc>
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

#ifndef SCMATH_GEOM_QUATERNION_H
#define SCMATH_GEOM_QUATERNION_H


#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <ostream>

#include <scmp/Math.h>
#include <scmp/geom/Vector.h>


namespace scmp {
namespace geom {


/// \brief Quaternions provide a representation of Orientation
///        and rotations in three dimensions.
///
/// Quaternions encode rotations in three-dimensional space. While
/// technically a MakeQuaternion is comprised of a real element and a
/// complex vector<3>, for the purposes of this library, it is modeled
/// as a floating point 4D vector of the form <w, x, y, z>, where x, y,
/// and z represent an Axis of rotation in R3 and w the Angle, in
/// radians, of the rotation about that Axis. Where Euler angles are
/// concerned, the ZYX (or yaw, pitch, roll) sequence is used.
///
/// For information on the underlying vector type, see the
/// documentation for scmp::geom::Vector.
///
/// Like vectors, quaternions carry an internal tolerance value ε that
/// is used for floating point comparisons. The math namespace contains
/// the default values used for this; generally, a tolerance of 0.0001
/// is considered appropriate for the uses of this library. The
/// tolerance can be explicitly set with the SetEpsilon method.
template<typename T>
class Quaternion {
public:
	/// \brief Construct an identity MakeQuaternion.
	Quaternion() : v(Vector<T, 3>{0.0, 0.0, 0.0}), w(1.0)
	{
		scmp::DefaultEpsilon(this->eps);
		v.SetEpsilon(this->eps);
	};


	/// \brief Construct a MakeQuaternion with an Axis and Angle of
	///        rotation.
	///
	/// A Quaternion may be initialised with a Vector<T, 3> Axis
	/// of rotation and an Angle of rotation. This doesn't do the
	/// Angle transforms to simplify internal operations.
	///
	/// \param _axis A three-dimensional vector of the same type as
	///              the Quaternion.
	/// \param _angle The Angle of rotation about the Axis of
	///               rotation.
	Quaternion(Vector<T, 3> _axis, T _angle) : v(_axis), w(_angle)
	{
		this->constrainAngle();
		scmp::DefaultEpsilon(this->eps);
		v.SetEpsilon(this->eps);
	};


	/// A Quaternion may be initialised with a Vector<T, 4> comprised of
	/// the Axis of rotation followed by the Angle of rotation.
	///
	/// \param vector A vector in the form <w, x, y, z>.
	Quaternion(Vector<T, 4> vector) :
		v(Vector<T, 3>{vector[1], vector[2], vector[3]}),
		w(vector[0])
	{
		this->constrainAngle();
		scmp::DefaultEpsilon(this->eps);
		v.SetEpsilon(this->eps);
	}

	
	/// \brief An initializer list containing values for w, x, y,
	///        and z.
	///
	/// \param ilst An initial set of values in the form
	///             <w, x, y, z>.
	Quaternion(std::initializer_list<T> ilst)
	{
		auto it = ilst.begin();

		this->v = Vector<T, 3>{it[1], it[2], it[3]};
		this->w = it[0];

		this->constrainAngle();
		scmp::DefaultEpsilon(this->eps);
		v.SetEpsilon(this->eps);
	}

	
	/// \brief Set the comparison tolerance for this MakeQuaternion.
	///
	/// \param epsilon A tolerance value.
	void
	SetEpsilon(T epsilon)
	{
		this->eps = epsilon;
		this->v.SetEpsilon(epsilon);
	}


	/// \brief Return the Axis of rotation of this MakeQuaternion.
	///
	/// \return The Axis of rotation of this MakeQuaternion.
	Vector<T, 3>
	Axis() const
	{
		return this->v;
	}


	/// \brief Return the Angle of rotation of this MakeQuaternion.
	///
	/// \return the Angle of rotation of this MakeQuaternion.
	T
	Angle() const
	{
		return this->w;
	}


	/// \brief Compute the Dot product of two quaternions.
	///
	/// \param other Another MakeQuaternion.
	/// \return The Dot product between the two quaternions.
	T
	Dot(const Quaternion<T> &other) const
	{
		double	innerProduct = this->v[0] * other.v[0];

		innerProduct += (this->v[1] * other.v[1]);
		innerProduct += (this->v[2] * other.v[2]);
		innerProduct += (this->w * other.w);
		return innerProduct;
	}


	/// \brief Compute the Norm of a MakeQuaternion.
	///
	/// Treating the Quaternion as a Vector<T, 4>, this is the same
	/// process as computing the Magnitude.
	///
	/// \return A non-negative real number.
	T
	Norm() const
	{
		T n = 0;

		n += (this->v[0] * this->v[0]);
		n += (this->v[1] * this->v[1]);
		n += (this->v[2] * this->v[2]);
		n += (this->w * this->w);

		return std::sqrt(n);
	}


	/// \brief Return the unit MakeQuaternion.
	///
	/// \return The unit MakeQuaternion.
	Quaternion
	UnitQuaternion()
	{
		return *this / this->Norm();
	}

	/// \brief Compute the Conjugate of a MakeQuaternion.
	///
	/// \return The Conjugate of this MakeQuaternion.
	Quaternion
	Conjugate() const
	{
		return Quaternion(Vector<T, 4>{this->w, -this->v[0], -this->v[1], -this->v[2]});
	}


	/// \brief Compute the Inverse of a MakeQuaternion.
	///
	/// \return The Inverse of this MakeQuaternion.
	Quaternion
	Inverse() const
	{
		T _norm = this->Norm();

		return this->Conjugate() / (_norm * _norm);
	}


	/// \brief Determine whether this is an identity MakeQuaternion.
	///
	/// \return true if this is an identity MakeQuaternion.
	bool
	IsIdentity() const {
		return this->v.IsZero() &&
		       scmp::WithinTolerance(this->w, (T)1.0, this->eps);
	}


	/// \brief Determine whether this is a unit MakeQuaternion.
	///
	/// \return true if this is a unit MakeQuaternion.
	bool
	IsUnitQuaternion() const
	{
		return scmp::WithinTolerance(this->Norm(), (T) 1.0, this->eps);
	}


	/// \brief Convert to Vector form.
	///
	/// Return the MakeQuaternion as a Vector<T, 4>, with the Axis of
	/// rotation followed by the Angle of rotation.
	///
	/// \return A vector representation of the MakeQuaternion.
	Vector<T, 4>
	AsVector() const
	{
		return Vector<T, 4>{this->w, this->v[0], this->v[1], this->v[2]};
	}


	/// \brief Rotate Vector vr about this MakeQuaternion.
	///
	/// \param vr The vector to be rotated.
	/// \return The rotated vector.
	Vector<T, 3>
	Rotate(Vector<T, 3> vr) const
	{
		return (this->Conjugate() * vr * (*this)).Axis();
	}


	/// \brief Return Euler angles for this MakeQuaternion.
	///
	/// Return the Euler angles for this MakeQuaternion as a vector of
	/// <yaw, pitch, roll>.
	///
	/// \warning Users of this function should watch out for gimbal
	///          lock.
	///
	/// \return A vector<T, 3> containing <yaw, pitch, roll>
	Vector<T, 3>
	Euler() const
	{
		T yaw, pitch, roll;
		T a = this->w, a2 = a * a;
		T b = this->v[0], b2 = b * b;
		T c = this->v[1], c2 = c * c;
		T d = this->v[2], d2 = d * d;

		yaw = std::atan2(2 * ((a * b) + (c * d)), a2 - b2 - c2 + d2);
		pitch = std::asin(2 * ((b * d) - (a * c)));
		roll = std::atan2(2 * ((a * d) + (b * c)), a2 + b2 - c2 - d2);

		return Vector<T, 3>{yaw, pitch, roll};
	}


	/// \brief Quaternion addition.
	///
	/// \param other The MakeQuaternion to be added with this one.
	/// \return The result of adding the two quaternions together.
	Quaternion
	operator+(const Quaternion<T> &other) const
	{
		return Quaternion(this->v + other.v, this->w + other.w);
	}


	/// \brief Quaternion subtraction.
	///
	/// \param other The MakeQuaternion to be subtracted from this one.
	/// \return The result of subtracting the other MakeQuaternion from this one.
	Quaternion
	operator-(const Quaternion<T> &other) const
	{
		return Quaternion(this->v - other.v, this->w - other.w);
	}


	/// \brief Scalar multiplication.
	///
	/// \param k The scaling value.
	/// \return A scaled MakeQuaternion.
	Quaternion
	operator*(const T k) const
	{
		return Quaternion(this->v * k, this->w * k);
	}


	/// \brief Scalar division.
	///
	/// \param k The scalar divisor.
	/// \return A scaled MakeQuaternion.
	Quaternion
	operator/(const T k) const
	{
		return Quaternion(this->v / k, this->w / k);
	}


	/// \brief Quaternion Hamilton multiplication with a three-
	///        dimensional vector.
	///
	/// This is done by treating the vector as a pure MakeQuaternion
	/// (e.g. with an Angle of rotation of 0).
	///
	/// \param vector The vector to multiply with this MakeQuaternion.
	/// \return The Hamilton product of the MakeQuaternion and vector.
	Quaternion
	operator*(const Vector<T, 3> &vector) const
	{
		return Quaternion(vector * this->w + this->v.Cross(vector),
				  (T) 0.0);
	}


	/// \brief Quaternion Hamilton multiplication.
	///
	/// \param other The other MakeQuaternion to multiply with this one.
	/// @result The Hamilton product of the two quaternions.
	Quaternion
	operator*(const Quaternion<T> &other) const
	{
		T angle = (this->w * other.w) -
			  (this->v * other.v);
		Vector<T, 3> axis = (other.v * this->w) +
				    (this->v * other.w) +
				    (this->v.Cross(other.v));
		return Quaternion(axis, angle);
	}


	/// \brief Quaternion equivalence.
	///
	/// \param other The MakeQuaternion to check equality against.
	/// \return True if the two quaternions are equal within their tolerance.
	bool
	operator==(const Quaternion<T> &other) const
	{
		return (this->v == other.v) &&
		       (scmp::WithinTolerance(this->w, other.w, this->eps));
	}


	/// \brief Quaternion non-equivalence.
	///
	/// \param other The MakeQuaternion to check inequality against.
	/// \return True if the two quaternions are unequal within their tolerance.
	bool
	operator!=(const Quaternion<T> &other) const
	{
		return !(*this == other);
	}


	/// \brief Output a MakeQuaternion to a stream in the form
	///        `a + <i, j, k>`.
	///
	/// \todo improve the formatting.
	///
	/// \param outs An output stream
	/// \param q A MakeQuaternion
	/// \return The output stream
	friend std::ostream &
	operator<<(std::ostream &outs, const Quaternion<T> &q)
	{
		outs << q.w << " + " << q.v;
		return outs;
	}

private:
	static constexpr T minRotation = -4 * M_PI;
	static constexpr T maxRotation = 4 * M_PI;

	Vector<T, 3> v; // Axis of rotation
	T w; // Angle of rotation
	T eps;

	void
	constrainAngle()
	{
		if (this->w < 0.0) {
			this->w = std::fmod(this->w, this->minRotation);
		}
		else {
			this->w = std::fmod(this->w, this->maxRotation);
		}
	}
};


///
/// \defgroup quaternion_aliases Quaternion type aliases.
/// Type aliases are provided for float and double quaternions.
///

/// \ingroup quaternion_aliases
/// \brief Type alias for a float Quaternion.
typedef Quaternion<float> Quaternionf;

/// \ingroup quaternion_aliases
/// \brief Type alias for a double Quaternion.
typedef Quaternion<double> Quaterniond;


/// \brief Convenience Quaternion construction function.
///
/// Return a float MakeQuaternion scaled appropriately from a vector and
/// Angle, e.g.
///     angle = cos(Angle / 2),
///     Axis.UnitVector() * sin(Angle / 2).
///
/// \param axis The Axis of rotation.
/// \param angle The Angle of rotation.
/// \return A MakeQuaternion.
/// \relatesalso Quaternion
Quaternionf	MakeQuaternion(Vector3F axis, float angle);

/// \brief Convience Quaternion construction function.
///
/// Return a double MakeQuaternion scaled appropriately from a vector and
/// Angle, e.g.
///     Angle = cos(Angle / 2),
///     Axis.UnitVector() * sin(Angle / 2).
///
/// \param axis The Axis of rotation.
/// \param angle The Angle of rotation.
/// \return A MakeQuaternion.
/// \relatesalso Quaternion
Quaterniond	MakeQuaternion(Vector3D axis, double angle);


/// \brief Convience Quaternion construction function.
///
/// Return a double MakeQuaternion scaled appropriately from a vector and
/// Angle, e.g.
///     Angle = cos(Angle / 2),
///     Axis.UnitVector() * sin(Angle / 2).
///
/// \param axis The Axis of rotation.
/// \param angle The Angle of rotation.
/// \return A MakeQuaternion.
/// \relatesalso Quaternion
template <typename T>
Quaternion<T>
MakeQuaternion(Vector<T, 3> axis, T angle)
{
	return Quaternion<T>(axis.UnitVector() * std::sin(angle / (T)2.0),
			     std::cos(angle / (T)2.0));
}


/// \brief COnstruct a Quaternion from Euler angles.
///
/// Given a vector of Euler angles in ZYX sequence (e.g. yaw, pitch,
/// roll), return a Quaternion.
///
/// \param euler A vector Euler Angle in ZYX sequence.
/// \return A Quaternion representation of the Orientation represented
///         by the Euler angles.
/// \relatesalso Quaternion
Quaternionf	FloatQuaternionFromEuler(Vector3F euler);


/// \brief COnstruct a Quaternion from Euler angles.
///
/// Given a vector of Euler angles in ZYX sequence (e.g. yaw, pitch,
/// roll), return a Quaternion.
///
/// \param euler A vector Euler Angle in ZYX sequence.
/// \return A Quaternion representation of the Orientation represented
///         by the Euler angles.
/// \relatesalso Quaternion
Quaterniond	DoubleQuaternionFromEuler(Vector3D euler);


/// \brief Linear interpolation for two Quaternions.
///
/// LERP computes the linear interpolation of two quaternions At some
/// fraction of the distance between them.
///
/// \tparam T
/// \param p The starting MakeQuaternion.
/// \param q The ending MakeQuaternion.
/// \param t The fraction of the distance between the two quaternions to
///	     interpolate.
/// \return A Quaternion representing the linear interpolation of the
///	    two quaternions.
template <typename T>
Quaternion<T>
LERP(Quaternion<T> p, Quaternion<T> q, T t)
{
	return (p + (q - p) * t).UnitQuaternion();
}


/// \brief Shortest distance spherical linear interpolation.
///
/// ShortestSLERP computes the shortest distance spherical linear
/// interpolation between two unit quaternions At some fraction of the
/// distance between them.
///
/// \tparam T
/// \param p The starting MakeQuaternion.
/// \param q The ending MakeQuaternion.Short
/// \param t The fraction of the distance between the two quaternions
///	     to interpolate.
/// \return A Quaternion representing the shortest path between two
///         quaternions.
template <typename T>
Quaternion<T>
ShortestSLERP(Quaternion<T> p, Quaternion<T> q, T t)
{
	assert(p.IsUnitQuaternion());
	assert(q.IsUnitQuaternion());

	T	dp = p.Dot(q);
	T	sign = dp < 0.0 ? -1.0 : 1.0;
	T	omega = std::acos(dp * sign);
	T	sin_omega = std::sin(omega); // Compute once.

	if (dp > 0.99999) {
		return LERP(p, q * sign, t);
	}

	return (p * std::sin((1.0 - t) * omega) / sin_omega) +
	       (q * sign * std::sin(omega*t) / sin_omega);
}


/// \brief Internal consistency check.
///
/// Run a quick self test to exercise basic functionality of the Quaternion
/// class to verify correct operation. Note that if \#NDEBUG is defined, the
/// self test is disabled.
void		QuaternionSelfTest();


} // namespace geom
} // namespace wr


#endif // SCMATH_GEOM_QUATERNION_H