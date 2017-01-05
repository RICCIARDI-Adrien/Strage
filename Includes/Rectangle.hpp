#ifndef HPP_RECTANGLE_HPP
#define HPP_RECTANGLE_HPP

/** @class Rectangle
 * Allow simple operations on rectangles.
 */
class Rectangle
{
	public:
		/** Leftmost coordinate. */
		int x;
		/** Topmost coordinate. */
		int y;
		/** Width. */
		int width;
		/** Height. */
		int height;
		
		/** Check if this rectangle is colliding with another one.
		 * @param pointerRectangle The other rectangle to check with.
		 * @return 0 if the rectangles do not collide,
		 * @return 1 if the rectangles collide.
		 */
		inline int isColliding(Rectangle *pointerRectangle)
		{
			if ((x + width <= pointerRectangle->x) && (y + height <= pointerRectangle->y)) return 1;
			return 0;
		}
};

#endif