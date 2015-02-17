#pragma once

#include <SpacePartitioning/Cullable/Cullable.hh>

namespace AGE
{
	template <typename type_t>
	struct CullableShape : public Cullable
	{
		type_t shape;
		bool checkCollision(Cullable *object);
		CullableShape();
		~CullableShape(){}
	};

	template <typename type_t>
	CullableShape<type_t>::CullableShape() :
		Cullable()
	{
		type = static_cast<CullableType>(type_t::shape_type);
	}

	template <typename type_t>
	bool CullableShape<type_t>::checkCollision(Cullable *object)
	{
		switch (object->type)
		{
		case CULLABLE_BOUNDING_BOX:
			return (shape.checkCollision(static_cast<CullableShape<AABoundingBox> *>(object)->shape));
			break;
		case CULLABLE_FRUSTUM:
			return (shape.checkCollision(static_cast<CullableShape<Frustum> *>(object)->shape));
			break;
		case CULLABLE_SPHERE:
			return (shape.checkCollision(static_cast<CullableShape<Sphere> *>(object)->shape));
			break;
		default:
			assert(!"Cannot check collision with this shape...");
			return (false);
			break;
		}
	}
}