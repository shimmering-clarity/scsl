///
/// \file include/scmp/geom/Orientation.h
/// \author K. Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief Orientation of vectors w.r.t. a reference plane, assumed to
///        be the Earth.
///
/// Copyright 2023 K. Isom <kyle@imap.cc>
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

#include <cstdint>

#include <scmp/geom/Vector.h>


#ifndef SCMATH_GEOM_ORIENTATION_H
#define SCMATH_GEOM_ORIENTATION_H


namespace scmp {
namespace geom {


/// \defgroup basis Basis vector indices.
/// The following constants are provided as a convenience for indexing two-
/// and three-dimensional vectors.

/// \ingroup basis
/// \brief Convenience constant for the x index.
constexpr uint8_t BasisX = 0;

/// \ingroup basis
/// \brief Convenience constant for the y index.
constexpr uint8_t BasisY = 1;

/// \ingroup basis
/// \brief Convenience constant for the z index.
constexpr uint8_t BasisZ = 2;


/// \brief Basis2D provides basis vectors for Vector2ds.
static const Vector2D Basis2D[] = {
    Vector2D{1, 0},
    Vector2D{0, 1},
};


/// \brief Basis2D provides basis vectors for Vector2fs.
static const Vector2F Basis2F[] = {
    Vector2F{1, 0},
    Vector2F{0, 1},
};


/// \brief Basis2D provides basis vectors for Vector3ds.
static const Vector3D Basis3D[] = {
    Vector3D{1, 0, 0},
    Vector3D{0, 1, 0},
    Vector3D{0, 0, 1},
};


/// \brief Basis2D provides basis vectors for Vector3fs.
static const Vector3F Basis3F[] = {
    Vector3F{1, 0, 0},
    Vector3F{0, 1, 0},
    Vector3F{0, 0, 1},
};


/// \brief Compass heading for a Vector2F.
///
/// \param vec A vector Orientation.
/// \return The compass heading of the vector in radians.
float	Heading2F(Vector2F vec);

/// \brief Compass heading for a Vector2D.
///
/// \param vec A vector Orientation.
/// \return The compass heading of the vector in radians.
double	Heading2d(Vector2D vec);

/// \brief Compass heading for a Vector2F.
///
/// \param vec A vector Orientation.
/// \return The compass heading of the vector in radians.
float	Heading3f(Vector3F vec);

/// Heading3d returns a compass heading for a Vector2F.
///
/// \param vec A vector Orientation.
/// \return The compass heading of the vector in radians.
double	Heading3d(Vector3D vec);


} // namespace geom
} // namespace scmp


#endif // SCMATH_GEOM_ORIENTATION_H
