#include <cmath>
#include <scccl/phys/basic/motion2d.h>

namespace scphys {
namespace basic {


scmath::geom::Vector2d
Acceleration(double speed, double heading)
{
	auto dx = std::cos(heading) * speed;
	auto dy = std::sin(heading) * speed;

	return scmath::geom::Vector2d({dx, dy});
}


} // namespace basic
} // namespace phys