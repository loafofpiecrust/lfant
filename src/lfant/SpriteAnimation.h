
#pragma once

// internal
#include <lfant/Animation.h>
#include <lfant/Rect.h>

// external

namespace lfant {

class Sprite;

class SpriteAnimation : public Animation
{

	class Clip : public Animation::Clip
	{
	public:

		uint16_t columns = 1;
		uint16_t rows = 1;

	protected:

	private:
	};

public:
	SpriteAnimation();
	~SpriteAnimation();

	virtual void Play();
	virtual void Pause();
	virtual void Stop();

	virtual void Init();
	virtual void Update();

protected:

	bool playing = false;
	bool playingReverseAnim = false;
	Rect currentFrame {0,0,50,50};
	Clip* currentAnim = nullptr;
	Sprite* sprite = nullptr;

private:
};

} // namespace lfant
