#include <deque>

class Map;
class Sprite;
class Hud;
typedef std::deque<Sprite *> SpriteList;

class Game
{
public:
    Game();
    bool hitTarget(Sprite *hero,Map &map,Hud &hud);
    bool hitTarget(Sprite *hero,Sprite *target,Map &map,Hud &hud);
    void mapReset(Map &map);
    void newGame(Map &map);

    void draw(Map &map,Hud &hud);
    void update(Map &map,Hud &hud);
    void handleDown(int key);
    void handleUp(int key);
    void addCharSprite(Sprite *spriteToAdd);
    void addItemSprite(Sprite *spriteToAdd);
    SpriteList _playerList;
private:
    Map *_map;
//    SpriteList _playerList;
    SpriteList _itemList;
};

