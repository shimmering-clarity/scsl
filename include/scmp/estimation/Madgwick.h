///
/// \file include/scmp/estimation/Madgwick.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2019-08-06
/// \brief Implementation of a Madgwick estimation.
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

#ifndef SCMP_FILTER_MADGWICK_H
#define SCMP_FILTER_MADGWICK_H


#include <scmp/geom/Vector.h>
#include <scmp/geom/Quaternion.h>


/// scmp contains the chimmering clarity math and physics code.
namespace scmp {

namespace estimation {


/// \brief Madgwick implements an efficient Orientation estimation for
///        Intertial Measurement Units (IMUs).
///
/// Madgwick is a  novel  Orientation  estimation  applicable  to  IMUs
/// consisting of tri-Axis  gyroscopes  and  accelerometers,  and  MARG
/// sensor  arrays  that  also  include tri-Axis magnetometers.  The MARG
/// implementation incorporates magnetic distortionand  gyroscope  bias
/// drift  compensation.
///
/// It is described in the paper [An efficient Orientation estimation for inertial and inertial/magnetic sensor arrays](http://x-io.co.uk/res/doc/madgwick_internal_report.pdf).
///
/// \tparam T A floating point type.
template <typename T>
class Madgwick {
public:
	/// \brief The Madgwick estimation is initialised with an identity MakeQuaternion.
	Madgwick() : deltaT(0.0), previousSensorFrame(), sensorFrame()
	{};

	/// \brief The Madgwick estimation is initialised with a sensor frame.
	///
	/// \param sf A sensor frame; if zero, the sensor frame will be
	///           initialised as an identity MakeQuaternion.
	Madgwick(scmp::geom::Vector<T, 3> sf) : deltaT(0.0), previousSensorFrame()
	{
		if (!sf.IsZero()) {
			sensorFrame = scmp::geom::MakeQuaternion<T>(sf, 0.0);
		}
	}

	/// \brief Initialise the estimation with a sensor frame MakeQuaternion.
	///
	/// \param sf A MakeQuaternion representing the current Orientation.
	Madgwick(scmp::geom::Quaternion<T> sf) :
	    deltaT(0.0), previousSensorFrame(), sensorFrame(sf)
	{};

	/// \brief Return the current orientation as measured by the
	///        estimation.
	///
	/// \return The current sensor frame.
	scmp::geom::Quaternion<T>
	Orientation() const
	{
		return this->sensorFrame;
	}

	/// \brief Return the estimation's rate of angular change from a
	///        sensor frame.
	///
	/// Return the rate of change of the Orientation of the earth
	/// frame with respect to the sensor frame.
	///
	/// \param gyro A three-dimensional vector containing gyro
	///             readings as w_x, w_y, w_z.
	/// \return A MakeQuaternion representing the rate of angular
	///         change.
	scmp::geom::Quaternion<T>
	AngularRate(const scmp::geom::Vector<T, 3> &gyro) const
	{
		return (this->sensorFrame * 0.5) * scmp::geom::Quaternion<T>(gyro, 0.0);
	}

	/// \brief Update the sensor frame to a new frame.
	///
	/// \param sf The new sensor frame replacing the previous one.
	/// \param delta The time delta since the last update.
	void
	UpdateFrame(const scmp::geom::Quaternion<T> &sf, T delta)
	{
		this->previousSensorFrame = this->sensorFrame;
		this->sensorFrame         = sf;
		this->deltaT              = delta;
	}

	/// \brief Update the sensor frame to a new frame.
	///
	/// \warning The estimation's default Δt must be set before
	///          calling this.
	///
	/// \param sf The new sensor frame replacing the previous one.
	void
	UpdateFrame(const scmp::geom::Quaternion<T> &sf)
	{
		this->UpdateFrame(sf, this->deltaT);
	}

	/// \brief Update the sensor frame with a gyroscope reading.
	///
	/// This method will assert that the Δt value is not zero
	/// within a 100μs tolerance. This assert is compiled out with
	/// the compile flag NDEBUG, but may be useful to catch
	/// possible errors.
	///
	/// \param gyro A three-dimensional vector containing gyro
	///             readings as w_x, w_y, w_z.
	/// \param delta The time step between readings. It must not
	///              be zero.
	void
	UpdateAngularOrientation(const scmp::geom::Vector<T, 3> &gyro, T delta)
	{
		// Ensure the delta isn't zero within a 100 μs
		// tolerance.
		if (scmp::WithinTolerance<T>(delta, 0.0, 0.00001)) {
			return;
		}
		scmp::geom::Quaternion<T> q = this->AngularRate(gyro) * delta;

		this->UpdateFrame(this->sensorFrame + q, delta);
	}

	/// \brief Update the sensor frame with a gyroscope reading.
	///
	/// If no Δt is provided, the estimation's default is used.
	///
	/// \warning The default Δt must be explicitly set using DeltaT
	///          before calling this.
	///
	/// \param gyro A three-dimensional vector containing gyro
	///             readings as w_x, w_y, w_z.
	void
	UpdateAngularOrientation(const scmp::geom::Vector<T, 3> &gyro)
	{
		this->UpdateAngularOrientation(gyro, this->deltaT);
	}

	/// \brief Retrieve a vector of the Euler angles in ZYX
	///        Orientation.
	///
	/// \return A vector of Euler angles as <ψ, θ, ϕ>.
	scmp::geom::Vector<T, 3>
	Euler()
	{
		return this->sensorFrame.Euler();
	}

	/// \brief Set the default Δt.
	///
	/// \note This must be explicitly called before calling any
	///       method which uses the estimation's internal Δt.
	///
	/// \param newDeltaT The time delta to use when no time delta
	///        is provided.
	void
	DeltaT(T newDeltaT)
	{
		this->deltaT = newDeltaT;
	}

	/// \brief Retrieve the estimation's current ΔT.
	///
	/// \return The current value the estimation will default to using
	///	    if no time delta is provided.
	T	DeltaT() { return this->deltaT; }

private:
	T				deltaT;
	scmp::geom::Quaternion<T>	previousSensorFrame;
	scmp::geom::Quaternion<T>	sensorFrame;
};


/// \brief Madgwickd is a shorthand alias for a Madgwick<double>.
using Madgwickd = Madgwick<double>;

/// \brief Madgwickf is a shorthand alias for a Madgwick<float>.
using Madgwickf = Madgwick<float>;


} // namespace estimation
} // namespace scmp


#endif // SCMP_FILTER_MADGWICK_H
