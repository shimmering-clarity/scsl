///
/// \file Madwick.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2019-08-06
/// \brief Implementation of a Madgwick filter.
///
/// See https://courses.cs.washington.edu/courses/cse466/14au/labs/l4/madgwick_internal_report.pdf.
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

/// \file madgwick.h
/// \brief Implementation of a Madgwick filter.
///
/// See
#ifndef SCMP_FILTER_MADGWICK_H
#define SCMP_FILTER_MADGWICK_H


#include <scmp/geom/Vector.h>
#include <scmp/geom/Quaternion.h>


/// scmp contains the wntrmute robotics code.
namespace scmp {
/// filter contains filtering algorithms.
namespace filter {


/// @brief Madgwick implements an efficient Orientation filter for IMUs.
///
/// Madgwick is a  novel  Orientation  filter  applicable  to  IMUs
/// consisting of tri-axis  gyroscopes  and  accelerometers,  and  MARG
/// sensor  arrays  that  also  include tri-axis magnetometers.  The MARG
/// implementation incorporates magnetic distortionand  gyroscope  bias
/// drift  compensation.
///
/// It is described in the paper [An efficient Orientation filter for inertial and inertial/magnetic sensor arrays](http://x-io.co.uk/res/doc/madgwick_internal_report.pdf).
///
/// \tparam T A floating point type.
template <typename T>
class Madgwick {
public:
	/// The Madgwick filter is initialised with an identity quaternion.
	Madgwick() : deltaT(0.0), previousSensorFrame(), sensorFrame() {};


	/// The Madgwick filter is initialised with a sensor frame.
	///
	/// \param sf A sensor frame; if zero, the sensor frame will be
	///           initialised as an identity quaternion.
	Madgwick(scmp::geom::Vector<T, 3> sf) : deltaT(0.0), previousSensorFrame()
	{
		if (!sf.isZero()) {
			sensorFrame = scmp::geom::quaternion(sf, 0.0);
		}
	}


	/// Initialise the filter with a sensor frame quaternion.
	///
	/// \param sf A quaternion representing the current Orientation.
	Madgwick(scmp::geom::Quaternion<T> sf) :
	    deltaT(0.0), previousSensorFrame(), sensorFrame(sf) {};


	/// Return the current Orientation as measured by the filter.
	///
	/// \return The current sensor frame.
	scmp::geom::Quaternion<T>
	Orientation() const
	{
		return this->sensorFrame;
	}


	/// Return the rate of change of the Orientation of the earth frame
	/// with respect to the sensor frame.
	///
	/// \param gyro A three-dimensional vector containing gyro readings
	///             as w_x, w_y, w_z.
	/// \return A quaternion representing the rate of angular change.
	scmp::geom::Quaternion<T>
	AngularRate(const scmp::geom::Vector<T, 3> &gyro) const
	{
		return (this->sensorFrame * 0.5) * scmp::geom::Quaternion<T>(gyro, 0.0);
	}

	/// Update the sensor frame to a new frame.
	///
	/// \param sf The new sensor frame replacing the previous one.
	/// \param delta The time delta since the last update.
	void
	UpdateFrame(const scmp::geom::Quaternion<T> &sf, T delta)
	{
		this->previousSensorFrame = this->sensorFrame;
		this->sensorFrame = sf;
		this->deltaT = delta;
	}


	/// Update the sensor frame with a gyroscope reading.
	///
	/// \param gyro A three-dimensional vector containing gyro readings
	///             as w_x, w_y, w_z.
	/// \param delta The time step between readings. It must not be zero.
	void
	UpdateAngularOrientation(const scmp::geom::Vector<T, 3> &gyro, T delta)
	{
		// Ensure the delta isn't zero within a 100 μs tolerance.
		assert(!scmp::WithinTolerance(delta, 0.0, 0.0001));
		scmp::geom::Quaternion<T>	q = this->AngularRate(gyro) * delta;

		this->UpdateFrame(this->sensorFrame + q, delta);
	}


	/// Retrieve a vector of the Euler angles in ZYX Orientation.
	///
	/// \return A vector of Euler angles as <ψ, θ, ϕ>.
	scmp::geom::Vector<T, 3>
	Euler()
	{
		return this->sensorFrame.euler();
	}

private:
	T				deltaT;
	scmp::geom::Quaternion<T>	previousSensorFrame;
	scmp::geom::Quaternion<T>	sensorFrame;
};


/// Madgwickd is a shorthand alias for a Madgwick<double>.
typedef Madgwick<double>	Madgwickd;

/// Madgwickf is a shorthand alias for a Madgwick<float>.
typedef Madgwick<float>		Madgwickf;


} // namespace filter
} // namespace scmp


#endif // SCMP_FILTER_MADGWICK_H
