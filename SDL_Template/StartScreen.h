#ifndef __STARTSCREEN_H
#define __STARTSCREEN_H
#include "AnimatedGLTexture.h"
#include "InputManager.h"
#include "Scoreboard.h"
#include "AudioManager.h"

using namespace SDLFramework;

class StartScreen : public GameEntity {

private:
	Timer * mTimer;
	InputManager * mInput;
	AudioManager* mAudio;

	//Background
	Texture* mBackground;

	// Top Bar Entities
	GameEntity * mTopBar;
	Texture * mPlayerOne;
	Texture* mPlayerOneExtrude;
	Texture * mQuit;
	Texture * mHiScore;
	Texture* mHiScoreExtrude;
	Scoreboard * mPlayerOneScore;
	Scoreboard * mTopScore;

	// Logo Entities
	Texture* mLogo;
	Texture* mLogoExtrude;
	AnimatedGLTexture* mAnimatedLogo;

	// Play Mode Entities
	GameEntity * mPlayModes;
	Texture * mOnePlayerMode;
	Texture * mCursor;
	Vector2 mCursorStartPos;
	Vector2 mCursorOffset;
	int mSelectedMode;

	// Bottom Bar Entities
	GameEntity * mBottomBar;
	Texture * mNamco;
	Texture * mDates;
	Texture * mRights;

	// Screen Animation Variables
	Vector2 mAnimationStartPos;
	Vector2 mAnimationEndPos;
	float mAnimationTotalTime;
	float mAnimationTimer;
	bool mAnimationDone;

public:
	StartScreen();
	~StartScreen();

	void ResetAnimation();

	int SelectedMode();

	void ChangeSelectedMode(int change);

	void Update() override;
	void Render() override;
};
#endif