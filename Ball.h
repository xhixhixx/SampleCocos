#ifndef __BALLSPRITE_H__
#define __BALLSPRITE_H__

#include "cocos2d.h"

#define BALL_INITIAL_SPEED 1.0f //per frame
#define BALL_DEFAULT_ACCELERATION 0.05f //per frame
#define BALL_MAX_SPEED 10.0f
#define BALL_MIN_SPEED 1.0f

USING_NS_CC;

struct PlayableArea
{
	float xMin = FLT_MIN;
	float xMax = FLT_MAX;
	float yMin = FLT_MIN;
	float yMax = FLT_MAX;

	bool CheckPositionInArea(Vec2 position)
	{
		if (position.x >= xMin && position.x <= xMax && position.y >= yMin && position.y <= yMax)
			return true;
		return false;
	}
};

class Ball
{
public:
	Ball();
	~Ball();

	void SpawnBall(Layer* layer);
	void MoveOnPath();

	void SetPlayableArea(float _xMin, float _xMax, float _yMin, float _yMax);
	void BuildPath(Vec2 point);

	bool IsClickedOn(Vec2 clickPosition);
	Vec2 GetPosition();
	Size GetBallSize();
	//Update function
	//call every frame by the scene
	void Update(float delta);

private:
	bool m_ProceedMoveByPath = false;
	bool m_IsAtDestination = true;

	std::queue<Vec2>	m_Path;
	float		m_CurrentMovingDistance = 0.0f;
	float		m_CurrentSpeed = BALL_INITIAL_SPEED;
	float		m_HalfFullDistance = 0.0f;

	Size		m_VisibleSize;
	Vec2		m_Origin;
	Sprite*		m_Sprite;
	PlayableArea m_PlayableArea;
	Animation*	m_Animation;

//
private:
	void updateAnimationFrame();
	Vector<SpriteFrame*> getAnimation(const char * format, int count);

	void MoveOneStepToward();
	void Stop();
	void ProceedMoveTowardNextDest(float acceleration);
};

#endif // __BALLSPRITE_H__