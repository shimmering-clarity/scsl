#include <cmath>
#include <scmp/Motion2D.h>

namespace scmp {
namespace basic {


scmp::geom::Vector2D
Acceleration(double speed, double heading)
{
	auto dx = std::cos(heading) * speed;
	auto dy = std::sin(heading) * speed;

	return scmp::geom::Vector2D({dx, dy});
}


} // namespace basic
} // namespace phys