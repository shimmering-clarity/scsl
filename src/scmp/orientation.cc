#include <scccl/math/geom/vector.h>
#include <scccl/math/geom/orientation.h>


namespace scmath {
namespace geom {


float
Heading2f(Vector2f vec)
{
	return vec.angle(Basis2f[Basis_x]);
}


float
Heading3f(Vector3f vec)
{
	Vector2f	vec2f {vec[0], vec[1]};
	return Heading2f(vec2f);
}


double
Heading2d(Vector2d vec)
{
	return vec.angle(Basis2d[Basis_x]);
}


double
Heading3d(Vector3d vec)
{
	Vector2d	vec2d {vec[0], vec[1]};
	return Heading2d(vec2d);
}


} // namespace geom
} // namespace math
