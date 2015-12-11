#include "ggo_delaunay_artist.h"
#include <ggo_kernel.h>
#include <ggo_paint.h>
#include <algorithm>
using namespace std;

const int GGO_MAX_TRIANGLES_COUNT	= 20;
const int GGO_COLOR_NOISE			= 50;

//////////////////////////////////////////////////////////////
class ggo_delaunay_triangle
{
public:

	void	to_triangle(ggo_triangle<float> & triangle) const;
	bool	is_valid() const;

public:

	const ggo_point_float *	_v1;
	const ggo_point_float *	_v2;
	const ggo_point_float *	_v3;
	ggo_color					_color;
};

//////////////////////////////////////////////////////////////
void ggo_delaunay_triangle::to_triangle(ggo_triangle<float> & triangle) const
{
	triangle.v1() = *_v1;
	triangle.v2() = *_v2;
	triangle.v3() = *_v3;
}

//////////////////////////////////////////////////////////////
bool ggo_delaunay_triangle::is_valid() const
{
	if ( _v1 == _v2 ) { return ( false ); }
	if ( _v1 == _v3 ) { return ( false ); }
	if ( _v2 == _v3 ) { return ( false ); }
	return ( true );
}

//////////////////////////////////////////////////////////////
class ggo_delaunay_edge
{
public:

			ggo_delaunay_edge() {};
			ggo_delaunay_edge(const ggo_point_float * v1, const ggo_point_float * v2) { _v1 = v1; _v2 = v2; };

	bool	operator==(const ggo_delaunay_edge & edge) const;

public:

	const ggo_point_float *	_v1;
	const ggo_point_float *	_v2;
};

//////////////////////////////////////////////////////////////
bool ggo_delaunay_edge::operator==(const ggo_delaunay_edge & edge) const
{
	if ( ( _v1 == edge._v1 ) && ( _v2 == edge._v2 ) ) { return ( true ); }
	if ( ( _v1 == edge._v2 ) && ( _v2 == edge._v1 ) ) { return ( true ); }
	return ( false );
}

//////////////////////////////////////////////////////////////
bool find_common_edge(const ggo_delaunay_triangle & triangle1, const ggo_delaunay_triangle & triangle2, ggo_delaunay_edge & common_edge)
{
	ggo_delaunay_edge	edge1_1 = ggo_delaunay_edge( triangle1._v1, triangle1._v2 );
	ggo_delaunay_edge	edge1_2 = ggo_delaunay_edge( triangle1._v2, triangle1._v3 );
	ggo_delaunay_edge	edge1_3 = ggo_delaunay_edge( triangle1._v3, triangle1._v1 );
	ggo_delaunay_edge	edge2_1 = ggo_delaunay_edge( triangle2._v1, triangle2._v2 );
	ggo_delaunay_edge	edge2_2 = ggo_delaunay_edge( triangle2._v2, triangle2._v3 );
	ggo_delaunay_edge	edge2_3 = ggo_delaunay_edge( triangle2._v3, triangle2._v1 );

	if ( ( edge1_1 == edge2_1 ) ||
		 ( edge1_1 == edge2_2 ) ||
		 ( edge1_1 == edge2_3 ) )
	{
		common_edge = edge1_1;
		return ( true );
	}

	if ( ( edge1_2 == edge2_1 ) ||
		 ( edge1_2 == edge2_2 ) ||
		 ( edge1_2 == edge2_3 ) )
	{
		common_edge = edge1_2;
		return ( true );
	}

	if ( ( edge1_3 == edge2_1 ) ||
		 ( edge1_3 == edge2_2 ) ||
		 ( edge1_3 == edge2_3 ) )
	{
		common_edge = edge1_3;
		return ( true );
	}

	return ( false );
}

//////////////////////////////////////////////////////////////
float compute_angle(const ggo_point_float * common, const ggo_point_float * pt1, const ggo_point_float * pt2)
{
	float	dx1, dy1, dx2, dy2, len1, len2, dot;

	dx1 	= pt1->x()-common->x();
	dy1 	= pt1->y()-common->y();
	dx2 	= pt2->x()-common->x();
	dy2 	= pt2->y()-common->y();

	len1	= sqrt(dx1*dx1+dy1*dy1);
	len2	= sqrt(dx2*dx2+dy2*dy2);

	// Normalize.
	dx1 /= len1; dy1 /= len1;
	dx2 /= len2; dy2 /= len2;

	dot = dx1*dx2+dy1*dy2;

	return ( acos(dot) );
}

//////////////////////////////////////////////////////////////
float compute_opposite_angle(const ggo_delaunay_triangle & triangle, const ggo_delaunay_edge & opposite_edge)
{
	if ( ggo_delaunay_edge( triangle._v1, triangle._v2 ) == opposite_edge )
	{
		return ( compute_angle( triangle._v3, triangle._v1, triangle._v2 ) );
	}
	else
	if ( ggo_delaunay_edge( triangle._v2, triangle._v3 ) == opposite_edge )
	{
		return ( compute_angle( triangle._v1, triangle._v2, triangle._v3 ) );
	}
	else
	if ( ggo_delaunay_edge( triangle._v3, triangle._v1 ) == opposite_edge )
	{
		return ( compute_angle( triangle._v2, triangle._v3, triangle._v1 ) );
	}
	else
	{
		GGO_FAIL();
		return ( -1 );
	}
}

//////////////////////////////////////////////////////////////
bool are_triangles_valid(const ggo_delaunay_triangle & triangle1, const ggo_delaunay_triangle & triangle2, const ggo_delaunay_edge & common_edge)
{
	float	angle1, angle2;

	angle1 = compute_opposite_angle( triangle1, common_edge );
	angle2 = compute_opposite_angle( triangle2, common_edge );

	return ( angle1+angle2 < GGO_PI+0.1 );
}

//////////////////////////////////////////////////////////////
void flip_triangle(ggo_delaunay_triangle & triangle1, ggo_delaunay_triangle & triangle2, const ggo_delaunay_edge & common_edge)
{
	GGO_ASSERT( triangle1.is_valid() == true );
	GGO_ASSERT( triangle2.is_valid() == true );
	GGO_ASSERT( common_edge._v1 != common_edge._v2 );
	GGO_ASSERT( ( triangle1._v1 == common_edge._v1 ) || ( triangle1._v2 == common_edge._v1 ) || ( triangle1._v3 == common_edge._v1 ) );
	GGO_ASSERT( ( triangle1._v1 == common_edge._v2 ) || ( triangle1._v2 == common_edge._v2 ) || ( triangle1._v3 == common_edge._v2 ) );
	GGO_ASSERT( ( triangle2._v1 == common_edge._v1 ) || ( triangle2._v2 == common_edge._v1 ) || ( triangle2._v3 == common_edge._v1 ) );
	GGO_ASSERT( ( triangle2._v1 == common_edge._v2 ) || ( triangle2._v2 == common_edge._v2 ) || ( triangle2._v3 == common_edge._v2 ) );

	GGO_TRACE( "flip_triangle\n" );

	const ggo_point_float *	opposite1 = NULL;
	const ggo_point_float *	opposite2 = NULL;
	uint8_t					r1, g1, b1, r2, g2, b2;

	// Build new triangles.
	if ( ( triangle1._v1 != common_edge._v1 ) && ( triangle1._v1 != common_edge._v2 ) ) { opposite1 = triangle1._v1; }
	if ( ( triangle1._v2 != common_edge._v1 ) && ( triangle1._v2 != common_edge._v2 ) ) { opposite1 = triangle1._v2; }
	if ( ( triangle1._v3 != common_edge._v1 ) && ( triangle1._v3 != common_edge._v2 ) ) { opposite1 = triangle1._v3; }

	if ( ( triangle2._v1 != common_edge._v1 ) && ( triangle2._v1 != common_edge._v2 ) ) { opposite2 = triangle2._v1; }
	if ( ( triangle2._v2 != common_edge._v1 ) && ( triangle2._v2 != common_edge._v2 ) ) { opposite2 = triangle2._v2; }
	if ( ( triangle2._v3 != common_edge._v1 ) && ( triangle2._v3 != common_edge._v2 ) ) { opposite2 = triangle2._v3; }

	GGO_ASSERT( opposite1 != NULL );
	GGO_ASSERT( opposite2 != NULL );

	triangle1._v1	= common_edge._v1;
	triangle1._v2	= opposite1;
	triangle1._v3	= opposite2;

	triangle2._v1	= common_edge._v2;
	triangle2._v2	= opposite1;
	triangle2._v3	= opposite2;

	GGO_ASSERT( triangle1.is_valid() == true );
	GGO_ASSERT( triangle2.is_valid() == true );
	GGO_ASSERT( are_triangles_valid( triangle1, triangle2, ggo_delaunay_edge( opposite1, opposite2 ) ) == true );

	// Mix colors.
	r1 = triangle1._color._r;
	g1 = triangle1._color._g;
	b1 = triangle1._color._b;

	r2 = triangle2._color._r;
	g2 = triangle2._color._g;
	b2 = triangle2._color._b;

	triangle1._color._r = ggo_clamp( (r1+r2)/2+ggo::rand_int(-GGO_COLOR_NOISE,GGO_COLOR_NOISE), 0, 0xFF );
	triangle1._color._g = ggo_clamp( (g1+g2)/2+ggo::rand_int(-GGO_COLOR_NOISE,GGO_COLOR_NOISE), 0, 0xFF );
	triangle1._color._b = ggo_clamp( (b1+b2)/2+ggo::rand_int(-GGO_COLOR_NOISE,GGO_COLOR_NOISE), 0, 0xFF );

	triangle2._color._r = ggo_clamp( (r1+r2)/2+ggo::rand_int(-GGO_COLOR_NOISE,GGO_COLOR_NOISE), 0, 0xFF );
	triangle2._color._g = ggo_clamp( (g1+g2)/2+ggo::rand_int(-GGO_COLOR_NOISE,GGO_COLOR_NOISE), 0, 0xFF );
	triangle2._color._b = ggo_clamp( (b1+b2)/2+ggo::rand_int(-GGO_COLOR_NOISE,GGO_COLOR_NOISE), 0, 0xFF );
}

//////////////////////////////////////////////////////////////
void add_new_triangle(list<ggo_delaunay_triangle> & triangles, const ggo_point_float * v1, const ggo_point_float * v2, const ggo_point_float * v3, uint8_t r, uint8_t g, uint8_t b)
{
	ggo_delaunay_triangle	triangle;

	triangle._v1		= v1;
	triangle._v2		= v2;
	triangle._v3		= v3;
	triangle._color._r	= ggo_clamp( r+ggo::rand_int(-GGO_COLOR_NOISE,GGO_COLOR_NOISE), 0, 0xFF );
	triangle._color._g	= ggo_clamp( g+ggo::rand_int(-GGO_COLOR_NOISE,GGO_COLOR_NOISE), 0, 0xFF );
	triangle._color._b	= ggo_clamp( b+ggo::rand_int(-GGO_COLOR_NOISE,GGO_COLOR_NOISE), 0, 0xFF );

	GGO_ASSERT( triangle.is_valid() == true );

	triangles.push_back( triangle );
}

//////////////////////////////////////////////////////////////
void add_new_vertex(list<ggo_delaunay_triangle> & triangles, vector<ggo_point_float*> & vertices)
{
	GGO_TRACE( "add_new_vertex\n" );

	float									v_x, v_y;
	ggo_point_float *						new_vertex;
	ggo_triangle<float>						triangle;
	list<ggo_delaunay_triangle>::iterator	it;

	// The new vertex.
	v_x			= static_cast<float>(rand())/static_cast<float>(RAND_MAX);
	v_y 		= static_cast<float>(rand())/static_cast<float>(RAND_MAX);
	new_vertex	= new ggo_point_float( v_x, v_y );

	// Look the the triangle in which the new vertex is.
	for ( it = triangles.begin(); it != triangles.end(); ++it )
	{
		it->to_triangle( triangle );

		if ( triangle.is_point_inside( new_vertex->x(), new_vertex->y() ) == true )
		{
			// Add new vertex.
			vertices.push_back( new_vertex );

			// Add new ones.
			add_new_triangle( triangles, it->_v1, it->_v2, new_vertex, it->_color._r, it->_color._g, it->_color._b );
			add_new_triangle( triangles, it->_v2, it->_v3, new_vertex, it->_color._r, it->_color._g, it->_color._b  );
			add_new_triangle( triangles, it->_v3, it->_v1, new_vertex, it->_color._r, it->_color._g, it->_color._b  );

			// Remove current triangle.
			triangles.erase( it );

			return;
		}
	}

	GGO_FAIL();
	delete new_vertex;
}


//////////////////////////////////////////////////////////////
bool update_triangulation(list<ggo_delaunay_triangle> & triangles)
{
	list<ggo_delaunay_triangle>::iterator	it1, it2;
	ggo_delaunay_edge						common_edge;

	for ( it1 = triangles.begin(); it1 != triangles.end(); ++it1 )
	{
		for ( it2 = triangles.begin(); it2 != triangles.end(); ++it2 )
		{
			if ( ( it1 != it2 ) &&
				 ( find_common_edge( *it1, *it2, common_edge ) == true ) &&
				 ( are_triangles_valid( *it1, *it2, common_edge ) == false ) )
			{
				flip_triangle( *it1, *it2, common_edge );
				return ( true );
			}
		}
	}

	return ( false );
}

//////////////////////////////////////////////////////////////
ggo_delaunay_artist::ggo_delaunay_artist(int render_size)
:
ggo_artist_abc( render_size )
{
}

//////////////////////////////////////////////////////////////
ggo_delaunay_artist::~ggo_delaunay_artist()
{
}

//////////////////////////////////////////////////////////////
void ggo_delaunay_artist::randomize_sub(int render_size)
{
	GGO_TRACE( "ggo_delaunay_artist::randomize\n" );

	ggo_point_float * 						v1 = new ggo_point_float( 0, 0 );
	ggo_point_float * 						v2 = new ggo_point_float( 0, 1 );
	ggo_point_float * 						v3 = new ggo_point_float( 1, 0 );
	ggo_point_float * 						v4 = new ggo_point_float( 1, 1 );
	vector<ggo_point_float*>				vertices;
	ggo_delaunay_edge						edge;
	vector<ggo_delaunay_edge>				edges;
	list<ggo_delaunay_triangle>::iterator	it_triangle;
	list<ggo_delaunay_triangle>				triangles;
	ggo_color_triangle						color_triangle;

	// Clear everything.
	_triangles.clear();
	_edges.clear();

	// Init with 2 triangles.
	vertices.push_back( v1 );
	vertices.push_back( v2 );
	vertices.push_back( v3 );
	vertices.push_back( v4 );

	add_new_triangle( triangles, v1, v2, v3, ggo::rand_int(0,255), ggo::rand_int(0,255), ggo::rand_int(0,255) );
	add_new_triangle( triangles, v2, v3, v4, ggo::rand_int(0,255), ggo::rand_int(0,255), ggo::rand_int(0,255) );

	// Perform the Delaunay triangulation.
	while ( true )
	{
		if ( distance( triangles.begin(), triangles.end() ) >= GGO_MAX_TRIANGLES_COUNT )
		{
			break;
		}

		if ( update_triangulation( triangles ) == false )
		{
			add_new_vertex( triangles, vertices );
		}
	}

	// Fill the color triangles and the edges.
	for ( it_triangle = triangles.begin(); it_triangle != triangles.end(); ++it_triangle )
	{
		// The triangle.
		color_triangle.color	= it_triangle->_color;
		color_triangle.triangle	= ggo_triangle<float>( *(it_triangle->_v1), *(it_triangle->_v2), *(it_triangle->_v3) );
		_triangles.push_back( color_triangle );

		// The 3 edges of the current triangle. Store them only once.
		edge = ggo_delaunay_edge( it_triangle->_v1, it_triangle->_v2 );
		if ( find( edges.begin(), edges.end(), edge ) == edges.end() )
		{
			edges.push_back( edge );
			_edges.push_back( ggo_segment_float( *(edge._v1), *(edge._v2) ) );
		}
		edge = ggo_delaunay_edge( it_triangle->_v2, it_triangle->_v3 );
		if ( find( edges.begin(), edges.end(), edge ) == edges.end() )
		{
			edges.push_back( edge );
			_edges.push_back( ggo_segment_float( *(edge._v1), *(edge._v2) ) );
		}
		edge = ggo_delaunay_edge( it_triangle->_v3, it_triangle->_v1 );
		if ( find( edges.begin(), edges.end(), edge ) == edges.end() )
		{
			edges.push_back( edge );
			_edges.push_back( ggo_segment_float( *(edge._v1), *(edge._v2) ) );
		}
	}

	// Don't forget to delete the vertices.
	for_each( vertices.begin(), vertices.end(), ggo_delete_ptr<ggo_point_float*> );
}

//////////////////////////////////////////////////////////////
ggo_oriented_box_float ggo_delaunay_artist::get_render_edge(const ggo_segment_float & edge) const
{
	ggo_oriented_box_float	render_edge;
	ggo_point_float			v1, v2, center;
	ggo_vector_float		direction;
	float					width;

	v1	 		= map_point( edge.from(), 0, 1 );
	v2	 		= map_point( edge.to(), 0, 1 );

	width		= map_value( 0.01, 0, 1 );
	center		= ggo_point_float( (v1.x()+v2.x())/2, (v1.y()+v2.y())/2 );
	direction	= ggo_vector_float( v1.x()-v2.x(), v1.y()-v2.y() );

	render_edge = ggo_oriented_box_float( center, direction, direction.get_length()/2, 1 );

	return ( render_edge );
}

//////////////////////////////////////////////////////////////
void ggo_delaunay_artist::update(ggo_user_feedback_abc * feedback)
{

}

//////////////////////////////////////////////////////////////
void ggo_delaunay_artist::render(uint8_t * buffer)
{
	GGO_TRACE( "ggo_delaunay_artist::render_rgb\n" );

	ggo_triangle<float>							triangle;
	vector<ggo_color_triangle>::const_iterator	it_triangle;
	vector<ggo_segment_float>::const_iterator	it_edge;
	ggo_oriented_box_float						edge;

	// Render the triangles.
	for ( it_triangle = _triangles.begin(); it_triangle != _triangles.end(); ++it_triangle )
	{
		triangle.v1() = map_point( it_triangle->triangle.v1(), 0, 1 );
		triangle.v2() = map_point( it_triangle->triangle.v2(), 0, 1 );
		triangle.v3() = map_point( it_triangle->triangle.v3(), 0, 1 );
		ggo_paint_shape_rgb8( buffer, get_render_size(), get_render_size(), triangle, it_triangle->color, 1 );
	}
	// Render the edges.
	for ( it_edge = _edges.begin(); it_edge != _edges.end(); ++it_edge )
	{
		edge = get_render_edge( *it_edge );
		ggo_paint_shape_rgb8( buffer, get_render_size(), get_render_size(), edge, ggo_color(0,0,0), 1 );
	}
}

//////////////////////////////////////////////////////////////
bool ggo_delaunay_artist::finished() const
{
	GGO_TRACE( "ggo_delaunay_artist::finished\n" );

	return ( true );
}
