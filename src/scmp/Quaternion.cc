#include <iostream>

#include <scmp/geom/Quaternion.h>


namespace scmp {
namespace geom {


Quaternionf
MakeQuaternion(Vector3F axis, float angle)
{
	return Quaternionf(axis.UnitVector() * std::sin(angle / 2.0),
			   std::cos(angle / 2.0));
}


Quaterniond
MakeQuaternion(Vector3D axis, double angle)
{
	return Quaterniond(axis.UnitVector() * std::sin(angle / 2.0),
			   std::cos(angle / 2.0));
}


Quaternionf
FloatQuaternionFromEuler(Vector3F euler)
{
	float x, y, z, w;
	euler = euler / 2.0;

	float cos_yaw = std::cos(euler[0]);
	float cos_pitch = std::cos(euler[1]);
	float cos_roll = std::cos(euler[2]);
	float sin_yaw = std::sin(euler[0]);
	float sin_pitch = std::sin(euler[1]);
	float sin_roll = std::sin(euler[2]);

	x = (sin_yaw * cos_pitch * cos_roll) + (cos_yaw * sin_pitch * sin_roll);
	y = (sin_yaw * cos_pitch * sin_roll) - (cos_yaw * sin_pitch * cos_roll);
	z = (cos_yaw * cos_pitch * sin_roll) + (sin_yaw * sin_pitch * cos_roll);
	w = (cos_yaw * cos_pitch * cos_roll) - (sin_yaw * sin_pitch * sin_roll);

	return Quaternionf(Vector4F{w, x, y, z});
}


Quaterniond
DoubleQuaternionFromEuler(Vector3D euler)
{
	double x, y, z, w;
	euler = euler / 2.0;

	double cos_yaw = std::cos(euler[0]);
	double cos_pitch = std::cos(euler[1]);
	double cos_roll = std::cos(euler[2]);
	double sin_yaw = std::sin(euler[0]);
	double sin_pitch = std::sin(euler[1]);
	double sin_roll = std::sin(euler[2]);

	x = (sin_yaw * cos_pitch * cos_roll) + (cos_yaw * sin_pitch * sin_roll);
	y = (sin_yaw * cos_pitch * sin_roll) - (cos_yaw * sin_pitch * cos_roll);
	z = (cos_yaw * cos_pitch * sin_roll) + (sin_yaw * sin_pitch * cos_roll);
	w = (cos_yaw * cos_pitch * cos_roll) - (sin_yaw * sin_pitch * sin_roll);

	return Quaterniond(Vector4D{w, x, y, z});
}


void
QuaternionSelfTest()
{
#ifndef NDEBUG
	Vector3F		v {1.0, 0.0, 0.0};
	Vector3F		yAxis {0.0, 1.0, 0.0};
	float			angle = M_PI / 2;

	Quaternionf		p = quaternionf(yAxis, angle);
	Quaternionf		q;
	Vector3F		vr {0.0, 0.0, 1.0};

	assert(p.isUnitQuaternion());
	std::cerr << p.rotate(v) << std::endl;
	assert(p.rotate(v) == vr);
	assert(p * q == p);
#endif
}


} // namespace geom
} // namespace scmp
