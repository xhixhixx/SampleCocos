#include "Playground.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio::timeline;

cocos2d::Scene * Playground::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Playground::create();

	// add layer as a child to scene
	if (layer)
	{
		scene->addChild(layer);
	}

	// return the scene
	return scene;
}

bool Playground::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_VisibleSize = Director::getInstance()->getVisibleSize();
	m_Origin = Director::getInstance()->getVisibleOrigin();

	Size halfVisibleSize = m_VisibleSize / 2;

	//Render background
	auto background = Sprite::create("bg.jpg");
	background->setPosition(Vec2(halfVisibleSize.width + m_Origin.x, halfVisibleSize.height + m_Origin.y));
	this->addChild(background);

	//create renderer for path drawing
	m_PathDrawNode = DrawNode::create();
	this->addChild(m_PathDrawNode);
	//create and render the ball object
	m_Ball.SpawnBall(this);
	//setup playable area for the ball
	float bgHalfWidth = background->getTextureRect().size.width / 2;
	float bgHalfHeight = background->getTextureRect().size.height / 2;
	Size HalfBallSize = m_Ball.GetBallSize() / 2;
	m_Ball.SetPlayableArea(-bgHalfWidth + halfVisibleSize.width + HalfBallSize.width,
							bgHalfWidth + halfVisibleSize.width - HalfBallSize.width,
						   -bgHalfHeight + halfVisibleSize.height + HalfBallSize.height,
							bgHalfHeight + halfVisibleSize.height - HalfBallSize.height);
	
	//setup mouse input listener
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(Playground::onMouseDown, this);
	mouseListener->onMouseUp   = CC_CALLBACK_1(Playground::onMouseUp, this);
	mouseListener->onMouseMove = CC_CALLBACK_1(Playground::onMouseMove, this);
	_eventDispatcher->addEventListenerWithFixedPriority(mouseListener, 1);

	//setup update loop
	this->scheduleUpdate();

	return true;
}

void Playground::update(float delta)
{
	m_Ball.Update(delta);
}

void Playground::onMouseDown(Event* event)
{
	EventMouse* e = (EventMouse*)event;
	Vec2 clickViewPosition = Vec2(e->getCursorX(), e->getCursorY());
	Vec2 clickWorldPosition = ConvertScreenPositionToWorldPosition(clickViewPosition);
	if (m_Ball.IsClickedOn(clickWorldPosition))
	{
		m_StartDragging = true;
		m_LastDragPosition = m_Ball.GetPosition();
		return;
	}

	m_Ball.BuildPath(clickWorldPosition);
}

void Playground::onMouseUp(Event * event)
{
	m_StartDragging = false;
	m_Ball.MoveOnPath();
}

void Playground::onMouseMove(Event * event)
{
	if (m_StartDragging)
	{
		EventMouse* e = (EventMouse*)event;
		//build point list
		Vec2 mousePosition = Vec2(e->getCursorX(), e->getCursorY());
		Vec2 mouseWorldPos = ConvertScreenPositionToWorldPosition(mousePosition);
		
		m_PathDrawNode->drawLine(m_LastDragPosition, mouseWorldPos, Color4F(0, 1, 0, 100));
		m_LastDragPosition = mouseWorldPos;
		//build Path
		m_Ball.BuildPath(mouseWorldPos);
	}
}

Vec2 Playground::ConvertScreenPositionToWorldPosition(const Vec2 screenPos)
{
	Vec2 screenTranslation = m_Ball.GetPosition() - Vec2(m_VisibleSize.width / 2 + m_Origin.x, m_VisibleSize.height / 2 + m_Origin.y);
	return screenPos + screenTranslation;
}
