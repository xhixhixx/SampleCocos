#include "Ball.h"

Ball::Ball()
{
	m_VisibleSize = Director::getInstance()->getVisibleSize();
	m_Origin = Director::getInstance()->getVisibleOrigin();
}

Ball::~Ball()
{
}

void Ball::SpawnBall(cocos2d::Layer * layer)
{
	// sprite
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ball.plist");
	auto frames = getAnimation("%02d.png", 32);
	m_Sprite = Sprite::createWithSpriteFrame(frames.front());
	layer->addChild(m_Sprite);
	//sprite->setPosition(background->getAnchorPointInPoints());
	m_Sprite->setPosition(Vec2(m_VisibleSize.width / 2 + m_Origin.x, m_VisibleSize.height / 2 + m_Origin.y));

	//camera follow
	layer->runAction(CCFollow::create(m_Sprite));

	m_Sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("01.png"));
}

void Ball::Update(float delta)
{
	//if there are no more node
	if (m_Path.empty())
	{
		//reset
		m_IsAtDestination = true;
		m_CurrentSpeed = BALL_INITIAL_SPEED;
		m_ProceedMoveByPath = false;
		m_HalfFullDistance = 0.0f;
		return;
	}
	//Move by path
	if (!m_IsAtDestination && m_CurrentMovingDistance > m_HalfFullDistance / 1.1f)  //need to stop + smoothen stopping
	{
		Stop();
	}
	else if (m_ProceedMoveByPath)
	{
		MoveOneStepToward();
	}

	updateAnimationFrame();
}

void Ball::MoveOnPath()
{
	m_ProceedMoveByPath = true;
	if (!m_IsAtDestination)
	{
		return;
	}
	m_CurrentMovingDistance = 0.0f;
	m_IsAtDestination = false;
}

void Ball::BuildPath(Vec2 point)
{
	if (!m_IsAtDestination)
		return;

	if (m_Path.size() == 0)
	{
		m_HalfFullDistance += (m_Sprite->getPosition() - point).getLength() / 2.0f;
	}
	else
	{
		float newPointDistance = (m_Path.back() - point).getLength();
		if (newPointDistance <= 10.0f)//smoothen movement
		{
			return;
		}
		m_HalfFullDistance +=  newPointDistance / 2.0f;
	}
	m_Path.push(point);
}

void Ball::MoveOneStepToward()
{
	ProceedMoveTowardNextDest(BALL_DEFAULT_ACCELERATION);
}

void Ball::Stop()
{
	ProceedMoveTowardNextDest(-BALL_DEFAULT_ACCELERATION);
}

void Ball::ProceedMoveTowardNextDest(float acceleration)
{
	m_CurrentSpeed += acceleration;
	if (m_CurrentSpeed < BALL_MIN_SPEED)
	{
		m_CurrentSpeed = BALL_MIN_SPEED;
	}
	if (m_CurrentSpeed > BALL_MAX_SPEED)
	{
		m_CurrentSpeed = BALL_MAX_SPEED;
	}

	Vec2 direction = m_Path.front() - m_Sprite->getPosition();
	//set rotation
	m_Sprite->setRotation(-CC_RADIANS_TO_DEGREES(direction.getAngle()));

	Vec2 moveStep = direction.getNormalized() * m_CurrentSpeed;
	Vec2 posibleNextPosition = m_Sprite->getPosition() + moveStep;

	//outside boundary
	if (!m_PlayableArea.CheckPositionInArea(posibleNextPosition))
	{
		//stop moving
		//clear path
		while (!m_Path.empty())
		{
			m_Path.pop();
		}
		return;
	}
	//last step too far
	if ((m_Sprite->getPosition() - m_Path.front()).getLength() <= m_CurrentSpeed)
	{
		//set final position
		m_Sprite->setPosition(m_Path.front());
		m_CurrentMovingDistance += (m_Sprite->getPosition() - m_Path.front()).getLength();
	}
	else 
	{
		m_Sprite->setPosition(posibleNextPosition);
		m_CurrentMovingDistance += moveStep.getLength();
	}
	//reached the current node
	if ((m_Sprite->getPosition() - m_Path.front()).getLengthSq() <= 1.0f)
	{
		m_Path.pop();
	}
}

void Ball::SetPlayableArea(float _xMin, float _xMax, float _yMin, float _yMax)
{
	m_PlayableArea.xMin = _xMin;
	m_PlayableArea.xMax = _xMax;
	m_PlayableArea.yMin = _yMin;
	m_PlayableArea.yMax = _yMax;
}

Size Ball::GetBallSize()
{
	return m_Sprite->getTextureRect().size;
}

bool Ball::IsClickedOn(Vec2 clickPosition)
{
	return m_Sprite->getBoundingBox().containsPoint(clickPosition);
}

Vec2 Ball::GetPosition()
{
	return m_Sprite->getPosition();
}

void Ball::updateAnimationFrame()
{
	int frameNumber = int(m_CurrentMovingDistance / 3.1415f);
	++frameNumber;
	frameNumber = frameNumber % 32;
	if (frameNumber == 0) frameNumber = 32;
	char str[100];
	sprintf(str, "%02d.png", frameNumber);
	m_Sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(str));
}

Vector<SpriteFrame*> Ball::getAnimation(const char * format, int count)
{
	auto spritecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> animFrames;
	char str[100];
	for (int i = 1; i <= count; i++)
	{
		sprintf(str, format, i);
		animFrames.pushBack(spritecache->getSpriteFrameByName(str));
	}
	return animFrames;
}