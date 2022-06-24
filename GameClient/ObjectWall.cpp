#include "ObjectWall.h"

WallMgr::WallMgr()
{
	Vector2 v1 = { 0, 0 };
	Vector2 v2 = { 614, 23 };
	this->v.push_back(Rect(v1, v2));
}
