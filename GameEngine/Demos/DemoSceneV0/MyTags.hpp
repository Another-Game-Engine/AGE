#pragma once

#define BIT(x) (1<<(x))

enum MyTags
{
	NO_TAG = 0
	, HEROS_TAG = 1
	, BULLET_TAG = 2
	, SLIDE_TAG = 3
	, ASTEROID_TAG = 4
};

enum PhysicTags
{
	COL_NOTHING = 0
	, COL_HOTZONE = BIT(0)
	, COL_WORLD = BIT(1)
	, COL_CHARACTER = BIT(2)
};