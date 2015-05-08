#pragma once

#include "game.hpp"

class State_Intro : public State {
public:
	State_Intro(Game*);
	virtual void    init();
	virtual void    run();
	virtual void    cleanup();
	virtual void    reset();
	virtual void    render();

private:
	sf::Clock       mClock;
	sf::Time        mFadeInTimer;
	sf::Time        mProceedTimer;
	sf::Time        mTimePassed;
	sf::Texture     mInitScreen;
	sf::Sprite      mSprite;
	sf::View        mView;
};
