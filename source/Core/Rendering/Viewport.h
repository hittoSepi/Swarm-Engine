#pragma once

#include "Utils/Math/Rect.h"


class Viewport
{
public:
	Viewport(const iRect& viewArea, const float2& depth)
	{
		setViewArea(viewArea);
		setDepth(depth);
	}

	Viewport(iRect viewArea, float depth_min, float depth_max)
	{
		setViewArea(viewArea);
		setDepth(depth_min, depth_max);
	}

	virtual ~Viewport() {}
	

	void setViewArea(iRect _view_area)
	{
		view_area = _view_area;
		setViewAreaImpl();
		
	}

	void setPosition(const float2& pos) { setPosition(pos.x, pos.y); }
	void setPosition(const float& x, const float& y) { view_area.x = x; view_area.y = y; setPositionImpl(); }

	void setSize(const float2& newSize) { setSize(newSize.x, newSize.y); }
	void setSize(float width, float height) { view_area.width = width; view_area.height = height; setSizeImpl(); }

	void setDepth(const float2& depthMinMax) { setDepth(depthMinMax.x, depthMinMax.y); }
	void setDepth(float min, float max)
	{
		min_depth = min;
		max_depth = max;
		setDepthImpl();
	}


	int		getWidth() { return view_area.width; }
	int		getHeight() { return view_area.height; }
	float2	getPosition() { return float2(view_area.x, view_area.y); }
	float2	getSize() { return float2(view_area.width, view_area.y); }
	float2	getDepth() { return float2(min_depth, max_depth); }

	

protected:

	virtual void setPositionImpl() = 0;
	virtual void setSizeImpl() = 0;
	virtual void setDepthImpl()  {  }
	virtual void setViewAreaImpl() {  }

	iRect view_area = iRect::zero();
	float min_depth = 0.0f;
	float max_depth = 1.0f;
};


