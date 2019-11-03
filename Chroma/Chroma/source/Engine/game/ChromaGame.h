#ifndef _CHROMA_GAME_
#define _CHROMA_GAME_


class ChromaGame
{

	enum State {GAME_ACTIVE, GAME_MENU, GAME_WIN};
public:
	State GameState { GAME_MENU };

		

	// Game Loop
	void ProcessInput();
	void Update();
	void Render();


};

#endif