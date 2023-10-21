#include <scmp/geom/Vector.h>
#include <scmp/geom/Orientation.h>


namespace scmp {
namespace geom {


float
Heading2F(Vector2F vec)
{
	return vec.Angle(Basis2F[BasisX]);
}


float
Heading3f(Vector3F vec)
{
	Vector2F vec2f {vec[0], vec[1]};
	return Heading2F(vec2f);
}


double
Heading2d(Vector2D vec)
{
	return vec.Angle(Basis2D[BasisX]);
}


double
Heading3d(Vector3D vec)
{
	Vector2D vec2d {vec[0], vec[1]};
	return Heading2d(vec2d);
}


} // namespace geom
} // namespace scmp
