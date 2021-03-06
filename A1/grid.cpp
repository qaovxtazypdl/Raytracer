#include <algorithm>

#include "grid.hpp"

Grid::Grid( size_t d )
	: m_dim( d )
{
	m_heights = new int[ d * d ];
  m_cols = new int[ d * d ];
  m_frame = new int[ d * d ];
	m_frame_type = new int[ d * d ];

	reset();
}

void Grid::reset()
{
	size_t sz = m_dim*m_dim;
	std::fill( m_heights, m_heights + sz, 0 );
  std::fill( m_cols, m_cols + sz, 0 );
	std::fill( m_frame, m_frame + sz, 0 );
}

Grid::~Grid()
{
	delete [] m_heights;
	delete [] m_cols;
}

size_t Grid::getDim() const
{
	return m_dim;
}

int Grid::getHeight( int x, int y ) const
{
	return m_heights[ y * m_dim + x ];
}

int Grid::getAnimationFrame( int x, int y ) const
{
  return m_frame[ y * m_dim + x ];
}

int Grid::getAnimationFrameType( int x, int y ) const
{
  return m_frame_type[ y * m_dim + x ];
}

int Grid::getColour( int x, int y ) const
{
	return m_cols[ y * m_dim + x ];
}

void Grid::setHeight( int x, int y, int h )
{
	unsigned prevHeight = m_heights[ y * m_dim + x ];
	m_heights[ y * m_dim + x ] = h;
}

void Grid::resetAnimationFrame( int x, int y, int type )
{
  m_frame[ y * m_dim + x ] = 20;
  m_frame_type[ y * m_dim + x ] = type;
}

void Grid::advanceAnimationFrame( int x, int y )
{
  if (m_frame[ y * m_dim + x ] > 0) m_frame[ y * m_dim + x ]--;
}

void Grid::setColour( int x, int y, int c )
{
	m_cols[ y * m_dim + x ] = c;
}

