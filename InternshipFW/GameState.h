#pragma once
class GameState
{
public:
    GameState() { };
    virtual ~GameState() {};
    
    virtual void Update(){};
    virtual void Render() = 0;
    
    virtual void Init(const char * path){};
    virtual void Destroy(){};

};