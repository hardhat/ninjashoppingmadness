#include <deque>

class Map;
class Sprite;
class Hud;
typedef std::deque<Sprite *> SpriteList;

class Game
{
public:
    Game();
    bool hitTarget(Sprite *hero,Sprite *target,Map &map,Hud &hud);
    void mapReset(Map &map,Sprite &hero,Sprite &baddie,Sprite &target);
    void newGame(Map &map,Sprite &hero,Sprite &baddie,Sprite &target);

    void draw(Map &map,Sprite &hero,Sprite &baddie,Sprite &target,Hud &hud);
    void update(Map &map,Sprite &hero,Sprite &baddie,Sprite &target,Hud &hud);
    void handleDown(int key);
    void handleUp(int key);
private:
    Map *_map;
    SpriteList _playerList;
    SpriteList _itemList;
};
