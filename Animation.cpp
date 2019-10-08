#include "Animation.hpp"

Animation::Animation() :
	frames(0),
	currentFrameIndex(0),
	currentFrameTime(0.f),
	releaseFirstFrame(true),
	isLooped(true)
{

}

void Animation::AddFrame(int textureID, int x, int y,
	int width, int height, float frameTime, bool looped)
{
	FrameData data;
	data.id = textureID;
	data.x = x;
	data.y = y;
	data.width = width;
	data.height = height;
	data.displayTimeSeconds = frameTime;

	frames.push_back(data);
	isLooped = looped;
}



const FrameData* Animation::GetCurrentFrame() const
{
	if (frames.size() > 0)
	{
		return &frames[currentFrameIndex];
	}

	return nullptr;
}

bool Animation::UpdateFrame(float deltaTime)
{
	//TODO: A bit cumbersome. Is there another way to do this?
	if (releaseFirstFrame)
	{
		releaseFirstFrame = false;
		RunActionForCurrentFrame();
		return true;
	}


	// Now we check if frame size is greater than 1 instead of 0
	if (frames.size() > 1 &&
		(isLooped ||
			currentFrameIndex < frames.size() -1))
	{
		currentFrameTime += deltaTime;

		if (currentFrameTime >=
			frames[currentFrameIndex].displayTimeSeconds)
		{
			currentFrameTime = 0.f;
			IncrementFrame();
			RunActionForCurrentFrame();
			return true;
		}
	}

	return false;
}

void Animation::IncrementFrame()
{
	// For more information on how this works see here: 
	//https://www.cprogramming.com/tips/tip/increment-and-decrement-counters-with-rollover
	currentFrameIndex = (currentFrameIndex + 1) % frames.size();
}

void Animation::Reset()
{
	currentFrameIndex = 0;
	currentFrameTime = 0.f;
	releaseFirstFrame = true;
}

void Animation::AddFrameAction(unsigned int frame, AnimationAction action)
{
	// If the frame is larger than the number of animation frames 
	// then this request is ignored.
	if (frame < frames.size())
	{
		auto actionKey = actions.find(frame);

		if (actionKey == actions.end())
		{
			// If there is not an existing entry for this frame 
			// we create one.
			framesWithActions.SetBit(frame);
			actions.insert(
				std::make_pair(frame,
					std::vector<AnimationAction>{action})
			);
		}
		else
		{
			// An existing entry was found so we 
			// add the action to the vector
			actionKey->second.emplace_back(action);
		}
	}
}

void Animation::RunActionForCurrentFrame()
{
	if (actions.size() > 0)
	{
		if (framesWithActions.GetBit(currentFrameIndex))
		{
			auto actionsToRun = actions.at(currentFrameIndex);

			for (auto f : actionsToRun)
			{
				f();
			}
		}
	}
}

void Animation::SetLooped(bool looped)
{
	isLooped = looped;
}

bool Animation::IsLooped()
{
	return isLooped;
}