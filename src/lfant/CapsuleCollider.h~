#ifndef LFANT_CAPSULECOLLIDER_H
#define LFANT_CAPSULECOLLIDER_H

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

} // namespace lfant

#endif // LFANT_CAPSULECOLLIDER_H
