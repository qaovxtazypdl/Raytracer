#pragma once

class Grid
{
public:
	Grid( size_t dim );
	~Grid();

	void reset();

	size_t getDim() const;

	int getHeight( int x, int y ) const;
  int getAnimationFrame( int x, int y) const;
	int getColour( int x, int y ) const;
  int getAnimationFrameType( int x, int y ) const; // 1 - drop block, 2 - fade out, 3 - fade in

  void setHeight( int x, int y, int h );
  void resetAnimationFrame( int x, int y, int type );
	void advanceAnimationFrame( int x, int y );
	void setColour( int x, int y, int c );

private:
	size_t m_dim;
	int *m_heights;
  int *m_cols;
  int *m_frame;
	int *m_frame_type;
};
