#pragma once

enum class GAMESTATES
{
	// list of all game states 
	// wave -> mini boss -> wave -> boss
	TEST,
	STAGE1_1,
	STAGE1_MINIBOSS,
	STAGE1_2,
	STAGE1_FINALBOSS,

	REWARD,

	MAINMENU,
	START,
	RESTART,
	QUIT,
	NUM
};