/// quaternion.h contains an implementation of quaternions suitable
/// for navigation in R3.
#ifndef SCMATH_QUATERNION_H
#define SCMATH_QUATERNION_H


#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <ostream>

#include <scccl/math/math.h>
#include <scccl/math/geom/vector.h>

/// math contains the shimmering clarity math library.
namespace scmath {
/// geom contains geometric classes and functions.
namespace geom {


/// @brief Quaternions provide a representation of orientation and rotations
/// in three dimensions.
///
/// Quaternions encode rotations in three-dimensional space. While technically
/// a quaternion is comprised of a real element and a complex vector<3>, for
/// the purposes of this library, it is modeled as a floating point 4D vector
/// of the form <w, x, y, z>, where x, y, and z represent an axis of rotation in
/// R3 and w the angle, in radians, of the rotation about that axis. Where Euler
/// angles are concerned, the ZYX (or yaw, pitch, roll) sequence is used.
///
/// For information on the underlying vector type, see the documentation for
/// wr::geom::Vector.
///
/// The constructors are primarily intended for intended operations; in practice,
/// the quaternionf() and quaterniond() functions are more useful for constructing
/// quaternions from vectors and angles.
///
/// Like vectors, quaternions carry an internal tolerance value ε that is used for
/// floating point comparisons. The math namespace contains the default values
/// used for this; generally, a tolerance of 0.0001 is considered appropriate for
/// the uses of this library. The tolerance can be explicitly set with the
/// setEpsilon method.
template<typename T>
class Quaternion {
public:
	/// The default Quaternion constructor returns an identity quaternion.
	Quaternion() : v(Vector<T, 3>{0.0, 0.0, 0.0}), w(1.0)
	{
		scmath::DefaultEpsilon(this->eps);
		v.setEpsilon(this->eps);
	};

	
	/// A Quaternion may be initialised with a Vector<T, 3> axis of rotation
	/// and an angle of rotation. This doesn't do the angle transforms to simplify
	/// internal operations.
	///
	/// @param _axis A three-dimensional vector of the same type as the Quaternion.
	/// @param _angle The angle of rotation about the axis of rotation.
	Quaternion(Vector<T, 3> _axis, T _angle) : v(_axis), w(_angle)
	{
		this->constrainAngle();
		scmath::DefaultEpsilon(this->eps);
		v.setEpsilon(this->eps);
	};


	/// A Quaternion may be initialised with a Vector<T, 4> comprised of
	/// the axis of rotation followed by the angle of rotation.
	///
	/// @param vector A vector in the form <w, x, y, z>.
	Quaternion(Vector<T, 4> vector) :
		v(Vector<T, 3>{vector[1], vector[2], vector[3]}),
		w(vector[0])
	{
		this->constrainAngle();
		scmath::DefaultEpsilon(this->eps);
		v.setEpsilon(this->eps);
	}

	
	/// A Quaternion may be constructed with an initializer list of
	/// type T, which must have exactly N elements.
	///
	/// @param ilst An initial set of values in the form <w, x, y, z>.
	Quaternion(std::initializer_list<T> ilst)
	{
		auto it = ilst.begin();

		this->v = Vector<T, 3>{it[1], it[2], it[3]};
		this->w = it[0];

		this->constrainAngle();
		scmath::DefaultEpsilon(this->eps);
		v.setEpsilon(this->eps);
	}

	
	/// Set the comparison tolerance for this quaternion.
	///
	/// @param epsilon A tolerance value.
	void
	setEpsilon(T epsilon)
	{
		this->eps = epsilon;
		this->v.setEpsilon(epsilon);
	}


	/// Return the axis of rotation of this quaternion.
	///
	/// @return The axis of rotation of this quaternion.
	Vector<T, 3>
	axis() const
	{
		return this->v;
	}


	/// Return the angle of rotation of this quaternion.
	///
	/// @return the angle of rotation of this quaternion.
	T
	angle() const
	{
		return this->w;
	}


	/// Compute the dot product of two quaternions.
	///
	/// \param other Another quaternion.
	/// \return The dot product between the two quaternions.
	T
	dot(const Quaternion<T> &other) const
	{
		double	innerProduct = this->v[0] * other.v[0];

		innerProduct += (this->v[1] * other.v[1]);
		innerProduct += (this->v[2] * other.v[2]);
		innerProduct += (this->w * other.w);
		return innerProduct;
	}


	/// Compute the norm of a quaternion. Treating the Quaternion as a
	/// Vector<T, 4>, it's the same as computing the magnitude.
	///
	/// @return A non-negative real number.
	T
	norm() const
	{
		T n = 0;

		n += (this->v[0] * this->v[0]);
		n += (this->v[1] * this->v[1]);
		n += (this->v[2] * this->v[2]);
		n += (this->w * this->w);

		return std::sqrt(n);
	}


	/// Return the unit quaternion.
	///
	/// \return The unit quaternion.
	Quaternion
	unitQuaternion()
	{
		return *this / this->norm();
	}

	/// Compute the conjugate of a quaternion.
	///
	/// @return The conjugate of this quaternion.
	Quaternion
	conjugate() const
	{
		return Quaternion(Vector<T, 4>{this->w, -this->v[0], -this->v[1], -this->v[2]});
	}


	/// Compute the inverse of a quaternion.
	///
	/// @return The inverse of this quaternion.
	Quaternion
	inverse() const
	{
		T _norm = this->norm();

		return this->conjugate() / (_norm * _norm);
	}


	/// Determine whether this is an identity quaternion.
	///
	/// \return true if this is an identity quaternion.
	bool
	isIdentity() const {
		return this->v.isZero() &&
		       scmath::WithinTolerance(this->w, (T)1.0, this->eps);
	}


	/// Determine whether this is a unit quaternion.
	///
	/// @return true if this is a unit quaternion.
	bool
	isUnitQuaternion() const
	{
		return scmath::WithinTolerance(this->norm(), (T) 1.0, this->eps);
	}


	/// Return the quaternion as a Vector<T, 4>, with the axis of rotation
	/// followed by the angle of rotation.
	///
	/// @return A vector representation of the quaternion.
	Vector<T, 4>
	asVector() const
	{
		return Vector<T, 4>{this->w, this->v[0], this->v[1], this->v[2]};
	}


	/// Rotate vector vr about this quaternion.
	///
	/// @param vr The vector to be rotated.
	/// @return The rotated vector.
	Vector<T, 3>
	rotate(Vector<T, 3> vr) const
	{
		return (this->conjugate() * vr * (*this)).axis();
	}


	/// Return the Euler angles for this quaternion as a vector of
	/// <yaw, pitch, roll>. Users of this function should watch out
	/// for gimbal lock.
	///
	/// @return A vector<T, 3> containing <yaw, pitch, roll>
	Vector<T, 3>
	euler() const
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


	/// Perform quaternion addition with another quaternion.
	///
	/// @param other The quaternion to be added with this one.
	/// @return The result of adding the two quaternions together.
	Quaternion
	operator+(const Quaternion<T> &other) const
	{
		return Quaternion(this->v + other.v, this->w + other.w);
	}


	/// Perform quaternion subtraction with another quaternion.
	///
	/// @param other The quaternion to be subtracted from this one.
	/// @return The result of subtracting the other quaternion from this one.
	Quaternion
	operator-(const Quaternion<T> &other) const
	{
		return Quaternion(this->v - other.v, this->w - other.w);
	}


	/// Perform scalar multiplication.
	///
	/// @param k The scaling value.
	/// @return A scaled quaternion.
	Quaternion
	operator*(const T k) const
	{
		return Quaternion(this->v * k, this->w * k);
	}


	/// Perform scalar division.
	///
	/// @param k The scalar divisor.
	/// @return A scaled quaternion.
	Quaternion
	operator/(const T k) const
	{
		return Quaternion(this->v / k, this->w / k);
	}


	/// Perform quaternion Hamilton multiplication with a three-
	/// dimensional vector; this is done by treating the vector
	/// as a pure quaternion (e.g. with an angle of rotation of 0).
	///
	/// @param vector The vector to multiply with this quaternion.
	/// @return The Hamilton product of the quaternion and vector.
	Quaternion
	operator*(const Vector<T, 3> &vector) const
	{
		return Quaternion(vector * this->w + this->v.cross(vector),
				  (T) 0.0);
	}


	/// Perform quaternion Hamilton multiplication.
	///
	/// @param other The other quaternion to multiply with this one.
	/// @result The Hamilton product of the two quaternions.
	Quaternion
	operator*(const Quaternion<T> &other) const
	{
		T angle = (this->w * other.w) -
			  (this->v * other.v);
		Vector<T, 3> axis = (other.v * this->w) +
				    (this->v * other.w) +
				    (this->v.cross(other.v));
		return Quaternion(axis, angle);
	}


	/// Perform quaternion equality checking.
	/// @param other The quaternion to check equality against.
	/// @return True if the two quaternions are equal within their tolerance.
	bool
	operator==(const Quaternion<T> &other) const
	{
		return (this->v == other.v) &&
		       (scmath::WithinTolerance(this->w, other.w, this->eps));
	}


	/// Perform quaternion inequality checking.
	///
	/// @param other The quaternion to check inequality against.
	/// @return True if the two quaternions are unequal within their tolerance.
	bool
	operator!=(const Quaternion<T> &other) const
	{
		return !(*this == other);
	}


	/// Support stream output of a quaternion in the form `a + <i, j, k>`.
	/// \todo improve the formatting.
	///
	/// @param outs An output stream
	/// @param q A quaternion
	/// @return The output stream
	friend std::ostream &
	operator<<(std::ostream &outs, const Quaternion<T> &q)
	{
		outs << q.w << " + " << q.v;
		return outs;
	}

private:
	static constexpr T minRotation = -4 * M_PI;
	static constexpr T maxRotation = 4 * M_PI;

	Vector<T, 3> v; // axis of rotation
	T w; // angle of rotation
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
/// Type alias for a float Quaternion.
typedef Quaternion<float> Quaternionf;

/// \ingroup quaternion_aliases
/// Type alias for a double Quaternion.
typedef Quaternion<double> Quaterniond;


/// Return a float quaternion scaled appropriately from a vector and angle,
/// e.g. angle = cos(angle / 2), axis.unitVector() * sin(angle / 2).
///
/// @param axis The axis of rotation.
/// @param angle The angle of rotation.
/// @return A quaternion.
/// @relatesalso Quaternion
Quaternionf	quaternionf(Vector3f axis, float angle);


/// Return a double quaternion scaled appropriately from a vector and angle,
/// e.g. angle = cos(angle / 2), axis.unitVector() * sin(angle / 2).
///
/// @param axis The axis of rotation.
/// @param angle The angle of rotation.
/// @return A quaternion.
/// @relatesalso Quaternion
Quaterniond	quaterniond(Vector3d axis, double angle);


/// Return a double quaternion scaled appropriately from a vector and angle,
/// e.g. angle = cos(angle / 2), axis.unitVector() * sin(angle / 2).
///
/// @param axis The axis of rotation.
/// @param angle The angle of rotation.
/// @return A quaternion.
/// @relatesalso Quaternion
template <typename T>
Quaternion<T>
quaternion(Vector<T, 3> axis, T angle)
{
	return Quaternion<T>(axis.unitVector() * std::sin(angle / (T)2.0),
			     std::cos(angle / (T)2.0));
}


/// Given a vector of Euler angles in ZYX sequence (e.g. yaw, pitch, roll),
/// return a quaternion.
///
/// @param euler A vector Euler angle in ZYX sequence.
/// @return A Quaternion representation of the orientation represented
///         by the Euler angles.
/// @relatesalso Quaternion
Quaternionf	quaternionf_from_euler(Vector3f euler);


/// Given a vector of Euler angles in ZYX sequence (e.g. yaw, pitch, roll),
/// return a quaternion.
///
/// @param euler A vector Euler angle in ZYX sequence.
/// @return A Quaternion representation of the orientation represented
///         by the Euler angles.
/// @relatesalso Quaternion
Quaterniond	quaterniond_from_euler(Vector3d euler);


/// LERP computes the linear interpolation of two quaternions at some
/// fraction of the distance between them.
///
/// \tparam T
/// \param p The starting quaternion.
/// \param q The ending quaternion.
/// \param t The fraction of the distance between the two quaternions to
///	     interpolate.
/// \return A Quaternion representing the linear interpolation of the
///	    two quaternions.
template <typename T>
Quaternion<T>
LERP(Quaternion<T> p, Quaternion<T> q, T t)
{
	return (p + (q - p) * t).unitQuaternion();
}


/// ShortestSLERP computes the shortest distance spherical linear
/// interpolation between two quaternions at some fraction of the
/// distance between them.
///
/// \tparam T
/// \param p The starting quaternion.
/// \param q The ending quaternion.Short
/// \param t The fraction of the distance between the two quaternions
///	     to interpolate.
/// \return A Quaternion representing the shortest path between two
///         quaternions.
template <typename T>
Quaternion<T>
ShortestSLERP(Quaternion<T> p, Quaternion<T> q, T t)
{
	assert(p.isUnitQuaternion());
	assert(q.isUnitQuaternion());

	T	dp = p.dot(q);
	T	sign = dp < 0.0 ? -1.0 : 1.0;
	T	omega = std::acos(dp * sign);
	T	sin_omega = std::sin(omega); // Compute once.

	if (dp > 0.99999) {
		return LERP(p, q * sign, t);
	}

	return (p * std::sin((1.0 - t) * omega) / sin_omega) +
	       (q * sign * std::sin(omega*t) / sin_omega);
}


/// Run a quick self test to exercise basic functionality of the Quaternion
/// class to verify correct operation. Note that if \#NDEBUG is defined, the
/// self test is disabled.
void		Quaternion_SelfTest();


} // namespace geom
} // namespace wr


#endif // WRMATH_QUATERNION_H
