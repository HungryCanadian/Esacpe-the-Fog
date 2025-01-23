#ifndef __PLAYSCREEN_H
#define __PLAYSCREEN_H
#include "Player.h"
#include "SideBar.h"

class PlayScreen : public GameEntity {
private:
	Timer * mTimer;
	AudioManager * mAudio;

	Player * mPlayer;

	Texture* mBackground;
	Texture* mPlaySpace;

	SideBar* mSideBar;

public:
	PlayScreen();
	~PlayScreen();

	void Update() override;
	void Render() override;
};
#endif