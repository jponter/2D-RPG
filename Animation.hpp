#pragma once
#ifndef Animation_hpp
#define Animation_hpp

#include <vector>
#include <functional>
#include <map>
#include "Bitmask.hpp"

using AnimationAction = std::function<void(void)>;

struct FrameData
{
	int id; // Texture id (retrieved from our texture allocator).
	int x; // x position of sprite in the texture.
	int y; // y position of sprite in the texture.
	int width; // Width of sprite.
	int height; // Height of sprite.
	float displayTimeSeconds; // How long to display the frame.
};

enum class FacingDirection
{
	None,
	Left,
	Right,
	Up,
	Down
};


class Animation
{
public:
	Animation();

	void AddFrame(int textureID, int x, int y,
		int width, int height, float frameTime, bool looped);

	const FrameData* GetCurrentFrame() const;

	bool UpdateFrame(float deltaTime);

	void Reset();

	void AddFrameAction(unsigned int frame, AnimationAction action);

	void SetLooped(bool looped);
	bool IsLooped();



private:
	void IncrementFrame();

	// Stores all frames for our animation.
	std::vector<FrameData> frames;

	// Current frame.
	int currentFrameIndex;

	// We use this to decide when to transition to the next frame.
	float currentFrameTime;

	bool releaseFirstFrame;
	//animation actions
	std::map<int, std::vector<AnimationAction>> actions;
	void RunActionForCurrentFrame();
	Bitmask framesWithActions;

	bool isLooped;


};













#endif /* Animation_hpp */