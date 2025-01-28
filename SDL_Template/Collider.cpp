#include "Collider.h"

void Collider::SetDebugTexture(Texture * tex) {
	delete mDebugTexture;
	mDebugTexture = tex;
	mDebugTexture->Parent(this);
}

Collider::Collider(ColliderType type)
	: mType(type) {
	mDebugTexture = new GLTexture("BoxCollider.png");
}

Collider::~Collider() {
	delete mDebugTexture;
	mDebugTexture = nullptr;
}


Collider::ColliderType Collider::GetType() {
	return mType;
}

void Collider::Render() {
	if (DEBUG_COLLIDERS) {
		mDebugTexture->Render();
	}
}
