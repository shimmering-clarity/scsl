/**
 * orientation.h concerns itself with computing the Orientation of some
 * vector with respect to a reference plane that is assumed to be the
 * of the Earth.
 */


#ifndef SCMATH_GEOM_ORIENTATION_H
#define SCMATH_GEOM_ORIENTATION_H


namespace scmp {
namespace geom {


/// \defgroup basis Basis vector indices.
/// The following constants are provided as a convenience for indexing two-
/// and three-dimensional vectors.

/// \ingroup basis
/// Convenience constant for the x index.
constexpr uint8_t Basis_x = 0;

/// \ingroup basis
/// Convenience constant for the y index.
constexpr uint8_t Basis_y = 1;

/// \ingroup basis
/// Convenience constant for the z index.
constexpr uint8_t Basis_z = 2;


/// @brief Basis2d provides basis vectors for Vector2ds.
static const Vector2d Basis2d[] = {
	Vector2d{1, 0},
	Vector2d{0, 1},
};


/// @brief Basis2d provides basis vectors for Vector2fs.
static const Vector2f Basis2f[] = {
	Vector2f{1, 0},
	Vector2f{0, 1},
};


/// @brief Basis2d provides basis vectors for Vector3ds.
static const Vector3d Basis3d[] = {
	Vector3d{1, 0, 0},
	Vector3d{0, 1, 0},
	Vector3d{0, 0, 1},
};


/// @brief Basis2d provides basis vectors for Vector3fs.
static const Vector3f Basis3f[] = {
	Vector3f{1, 0, 0},
	Vector3f{0, 1, 0},
	Vector3f{0, 0, 1},
};


/// Heading2f returns a compass heading for a Vector2f.
/// @param vec A vector Orientation.
/// @return The compass heading of the vector in radians.
float	Heading2f(Vector2f vec);

/// Heading2d returns a compass heading for a Vector2d.
/// @param vec A vector Orientation.
/// @return The compass heading of the vector in radians.
double	Heading2d(Vector2d vec);

/// Heading3f returns a compass heading for a Vector2f.
/// @param vec A vector Orientation.
/// @return The compass heading of the vector in radians.
float	Heading3f(Vector3f vec);

/// Heading3d returns a compass heading for a Vector2f.
/// @param vec A vector Orientation.
/// @return The compass heading of the vector in radians.
double	Heading3d(Vector3d vec);


} // namespace geom
} // namespace scmp


#endif // __WRMATH_ORIENTATION_H
