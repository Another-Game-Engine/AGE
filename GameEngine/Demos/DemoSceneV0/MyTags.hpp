#pragma once

#define BIT(x) (1<<(x))

enum MyTags
{
	BALL_TAG = 0
	, HEROS_TAG = 1
};

enum PhysicTags
{
	COL_NOTHING = 0
	, COL_HOTZONE = BIT(0)
	, COL_WORLD = BIT(1)
	, COL_CHARACTER = BIT(2)
};