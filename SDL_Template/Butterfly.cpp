#include "Butterfly.h"
#include "BoxCollider.h"
#include "AudioManager.h"

// std::vector<std::vector<Vector2>> Butterfly::sDivePaths; // Remove DivePaths variable

void Butterfly::CreateDivePaths() {
    // Commenting out dive path creation code
    /*
    AddDivePath({
        Vector2(0.0f, 0.0f), Vector2(0.0f, -45.0f), Vector2(-60.0f, -45.0f), Vector2(-60.f, 0.0f),
        Vector2(-60.0f, 0.0f), Vector2(-60.0f, 80.0f), Vector2(200.0f, 125.0f), Vector2(200.0f, 200.0f),
        Vector2(200.0f, 200.0f), Vector2(200.0f, 275.0f), Vector2(175.0f, 250.0f), Vector2(175.0f, 325.0f),
        Vector2(175.0f, 325.0f), Vector2(175.0f, 425.0f), Vector2(375.0f, 425.0f), Vector2(375.0f, 525.0f),
        Vector2(375.0f, 525.0f), Vector2(375.0f, 575.0f), Vector2(300.0f, 625.0f), Vector2(300.0f, 775.0f)
    });

    // Create path 1
    AddDivePath({
        Vector2(0.0f, 0.0f), Vector2(0.0f, -45.0f), Vector2(60.0f, -45.0f), Vector2(60.f, 0.0f),
        Vector2(60.0f, 0.0f), Vector2(60.0f, 80.0f), Vector2(-200.0f, 125.0f), Vector2(-200.0f, 200.0f),
        Vector2(-200.0f, 200.0f), Vector2(-200.0f, 275.0f), Vector2(-175.0f, 250.0f), Vector2(-175.0f, 325.0f),
        Vector2(-175.0f, 325.0f), Vector2(-175.0f, 425.0f), Vector2(-375.0f, 425.0f), Vector2(-375.0f, 525.0f),
        Vector2(-375.0f, 525.0f), Vector2(-375.0f, 575.0f), Vector2(-300.0f, 625.0f), Vector2(-300.0f, 775.0f)
    });

    // Create path 2
    AddDivePath({
        Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(-90.0f, -60.0f), Vector2(-90.0f, 0.0f),
        Vector2(-90.0f, 0.0f), Vector2(-90.0f, 60.0f), Vector2(-100.0f, 272.0f), Vector2(-15.0f, 275.0f),
        Vector2(-15.0f, 275.0f), Vector2(85.0f, 275.0f), Vector2(85.0f, 125.0f), Vector2(-15.0f, 125.0f),
        Vector2(-15.0f, 125.0f), Vector2(-175.0f, 125.0f), Vector2(0.0f, 450.0f), Vector2(125.0f, 450.0f),
        Vector2(120.0f, 450.0f), Vector2(160.0f, 450.0f), Vector2(200.0f, 500.0f), Vector2(200.0f, 550.0f),
        Vector2(200.0f, 550.0f), Vector2(200.0f, 540.0f), Vector2(200.0f, 810.0f), Vector2(200.0f, 800.0f)
    });

    // Create path 3
    AddDivePath({
        Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(90.0f, -60.0f), Vector2(90.0f, 0.0f),
        Vector2(90.0f, 0.0f), Vector2(90.0f, 60.0f), Vector2(100.0f, 272.0f), Vector2(15.0f, 275.0f),
        Vector2(15.0f, 275.0f), Vector2(-85.0f, 275.0f), Vector2(-85.0f, 125.0f), Vector2(15.0f, 125.0f),
        Vector2(15.0f, 125.0f), Vector2(175.0f, 125.0f), Vector2(0.0f, 450.0f), Vector2(-125.0f, 450.0f),
        Vector2(-120.0f, 450.0f), Vector2(-160.0f, 450.0f), Vector2(-200.0f, 500.0f), Vector2(-200.0f, 550.0f),
        Vector2(-200.0f, 550.0f), Vector2(-200.0f, 540.0f), Vector2(-200.0f, 810.0f), Vector2(-200.0f, 800.0f)
    });
    */
}

//void Butterfly::AddDivePath(const std::vector<Vector2>& pathData) {
    // Function is no longer necessary since we commented out the path creation
//}

Vector2 Butterfly::LocalFormationPosition() {
    // Removing the formation-related code entirely since it's no longer needed.
    return Vec2_Zero; // Placeholder for removed functionality
}

bool Butterfly::IgnoreCollisions() {
    return !InDeathAnimation() || !Active();
}

void Butterfly::HandleDiveState() {
//    // Logic for handling dive state remains the same, but now does not reference formation or dive paths
//    Vector2 dist = WorldFormationPosition() - Position();  // Keep this logic as placeholder
//
//    // Handling the movement without referencing dive paths
//    Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
//    Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);
//
//    if (dist.MagnitudeSqr() < EPSILON * mSpeed / 25.0f) {
//        JoinFormation();  // This may also be replaced with other behavior if necessary
//    }
}

void Butterfly::RenderDiveState() {
    mTexture->Render();
}

void Butterfly::RandomlySpawn(Vector2 minBoundary, Vector2 maxBoundary) {
    // Generate random position within the boundaries
    float randomX = RandomFloat(minBoundary.x, maxBoundary.x);
    float randomY = RandomFloat(minBoundary.y, maxBoundary.y);


    Position(Vector2(400, 400));

    std::cout << "Butterfly spawned at position: ("
        << Position().x << ", " << Position().y << ")" << std::endl;
}

//void Butterfly::Dive(int type) {
//    mEscort = type != 0;
//    Enemy::Dive();
//}

void Butterfly::Hit(PhysEntity* other) {
    AudioManager::Instance()->PlaySFX("SFX/ButterflyDestroyed.wav", 0, 3);
    sPlayer->AddScore(mCurrentState == Enemy::InFormation ? 80 : 160);
    Enemy::Hit(other);
}

Butterfly::Butterfly(int index, bool challenge) : Enemy (index, challenge) {
    mTexture = new GLTexture("Pirate.png");
    mTexture->Parent(this);
    mTexture->Position(Vec2_Zero);
    mTexture->Scale(Vector2(0.5f, 0.5f));

    mType = Enemy::Butterfly;

    AddCollider(new BoxCollider(Vector2(16.0f, 67.0f)));
    AddCollider(new BoxCollider(Vector2(10.0f, 24.0f)), Vector2(9.0f, 5.0f));
    AddCollider(new BoxCollider(Vector2(10.0f, 24.0f)), Vector2(-9.0f, 5.0f));
}


Butterfly::~Butterfly() {}
