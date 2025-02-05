#include "Pirate.h"
#include "BoxCollider.h"
#include "AudioManager.h"

void Pirate::CreateDivePaths() {

}

bool Pirate::IgnoreCollisions() {
    return !InDeathAnimation() || !Active();
}

void Pirate::HandleDiveState() {
}

void Pirate::RenderDiveState() {
    mTexture->Render();
}

void Pirate::Hit(PhysEntity* other) {
    AudioManager::Instance()->PlaySFX("SFX/ButterflyDestroyed.wav", 0, 3);
    sPlayer->AddScore(mCurrentState == Enemy::InFormation ? 80 : 160);
    Enemy::Hit(other);
}

Pirate::Pirate(int index, bool challenge) : Enemy(index, challenge) {
    mTexture = new GLTexture("Pirate.png");
    mTexture->Parent(this);
    mTexture->Position(Vec2_Zero);
    mTexture->Flip(false, true);
    mTexture->Scale(Vector2(0.5f, 0.5f));

    mType = Enemy::Pirate;

    AddCollider(new BoxCollider(Vector2(16.0f, 67.0f)));
    AddCollider(new BoxCollider(Vector2(10.0f, 24.0f)), Vector2(9.0f, 5.0f));
    AddCollider(new BoxCollider(Vector2(10.0f, 24.0f)), Vector2(-9.0f, 5.0f));
}


Pirate::~Pirate() {}