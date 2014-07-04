#pragma once

#include <lfant/Collider.h>

class btCapsuleShape;

namespace lfant {

class CapsuleCollider : public lfant::Collider
{
public:
	CapsuleCollider();
	virtual ~CapsuleCollider();

	void SetRadius(float radius);
	void SetHeight(float height);

protected:
	btCollisionShape* GetShape();

private:
	ptr<btCapsuleShape> shape;
};

}
