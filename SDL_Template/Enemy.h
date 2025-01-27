
#pragma once
#include "AnimatedTexture.h"
#include "BezierPath.h"
#include "Player.h"
#include "PhysEntity.h"
#include "Formation.h"

class Enemy : public PhysEntity {
public:
	enum States { FlyIn, InFormation, Diving, Dead };
	enum Types { Butterfly, Wasp, Boss };

	static void CreatePaths();
	static void SetFormation(Formation* formation);

	bool InDeathAnimation();

	Enemy(int path, int index, bool challenge);
	virtual ~Enemy();

	virtual void Dive(int type = 0);

	States CurrentState();
	static void CurrentPlayer(Player* player);


	Types Type();
	int Index();

	virtual void Hit(PhysEntity* other) override;

	void Update() override;
	void Render() override;

protected:
	static Formation* sFormation;
	static std::vector<std::vector<Vector2>> sPaths;
	static Player* sPlayer;
	Timer* mTimer;

	Texture* mTexture;
	AnimatedTexture* mDeathAnimation;

	States mCurrentState;
	Types mType;

	int mIndex;

	bool mChallengeStage;

	Vector2 mDiveStartPosition;

	unsigned mCurrentPath;
	unsigned mCurrentWaypoint;

	const float EPSILON = 50.0f;
	float mSpeed;

	virtual void PathComplete();
	virtual void FlyInComplete();

	void JoinFormation();

	virtual Vector2 WorldFormationPosition();
	virtual Vector2 LocalFormationPosition() = 0;

	virtual void HandleFlyInState();
	virtual void HandleInFormationState();
	virtual void HandleDiveState() = 0;
	virtual void HandleDeadState();

	void HandleStates();

	virtual void RenderFlyInState();
	virtual void RenderInFormationState();
	virtual void RenderDiveState() = 0;
	virtual void RenderDeadState();

	void RenderStates();
private:
	Enemy* mEnemy;
};
