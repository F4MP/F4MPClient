//
// Created by again on 8/19/2020.
//

#ifndef F4MPCLIENT_GAME_H
#define F4MPCLIENT_GAME_H

#include <Windows.h>
#include <iostream>


//TODO : We don't care about this because we are going to have a entity framework
class Game {

public:

    Game();

    bool isRunning();

    void BeginPlay();

    void Update();


};


#endif //F4MPCLIENT_GAME_H
