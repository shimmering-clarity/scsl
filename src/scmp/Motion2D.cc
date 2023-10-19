#include <cmath>
#include <scmp/Motion2D.h>

namespace scmp {
namespace basic {


scmp::geom::Vector2d
Acceleration(double speed, double heading)
{
	auto dx = std::cos(heading) * speed;
	auto dy = std::sin(heading) * speed;

	return scmp::geom::Vector2d({dx, dy});
}


} // namespace basic
} // namespace phys