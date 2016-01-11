#include <algorithm>

#include "grid.hpp"

Grid::Grid( size_t d )
	: m_dim( d ), m_totalCubes(0)
{
	m_heights = new int[ d * d ];
	m_cols = new int[ d * d ];

	reset();
}

void Grid::reset()
{
	size_t sz = m_dim*m_dim;
	std::fill( m_heights, m_heights + sz, 0 );
	std::fill( m_cols, m_cols + sz, 0 );
	m_totalCubes = 0;
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

int Grid::getColour( int x, int y ) const
{
	return m_cols[ y * m_dim + x ];
}

unsigned Grid::getTotalCubes() const
{
	return m_totalCubes;
}

void Grid::setHeight( int x, int y, int h )
{
	unsigned prevHeight = m_heights[ y * m_dim + x ];
	m_heights[ y * m_dim + x ] = h;
	m_totalCubes = m_totalCubes + h - prevHeight;
}

void Grid::setColour( int x, int y, int c )
{
	m_cols[ y * m_dim + x ] = c;
}

