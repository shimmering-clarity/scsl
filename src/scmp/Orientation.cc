///
/// \file src/scmp/geom/Orientation.cc
/// \author K. Isom <kyle@imap.cc>
/// \date 2017-06-05
/// \brief Orientation of vectors w.r.t. a reference plane, assumed to
///        be the Earth.
///
/// Copyright 2017 K. Isom <kyle@imap.cc>
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
Heading3F(Vector3F vec)
{
	const Vector2F vec2f {vec.At(BasisX), vec.At(BasisY)};
	return Heading2F(vec2f);
}


double
Heading2D(Vector2D vec)
{
	return vec.Angle(Basis2D[BasisX]);
}


double
Heading3D(Vector3D vec)
{
	const Vector2D vec2d {vec.At(BasisX), vec.At(BasisY)};
	return Heading2D(vec2d);
}


} // namespace geom
} // namespace scmp
