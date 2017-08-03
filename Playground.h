#ifndef  _PLAYGROUND_SCENE_H
#define _PLAYGROUND_SCENE_H

#include "cocos2d.h"
#include "Ball.h"

USING_NS_CC;

class Playground : public cocos2d::Layer
{
public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Playground);

	void update(float) override;

private:
	Ball m_Ball;

	Size		m_VisibleSize;
	Vec2		m_Origin;

	bool m_StartDragging = false;
	Vec2 m_LastDragPosition;

	DrawNode* m_PathDrawNode = nullptr;

//function
private:
	void onMouseDown(Event* event);
	void onMouseUp(Event* event);
	void onMouseMove(Event* event);

	Vec2 Playground::ConvertScreenPositionToWorldPosition(const Vec2 screenPos);
};

#endif // _PLAYGROUND_SCENE_H