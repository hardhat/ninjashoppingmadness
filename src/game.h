#include <deque>

class Map;
class Sprite;
typedef std::deque<Sprite *> SpriteList;

class Game
{
public:
    Game();
    void draw();
    void update(int elapsed);
    void handleDown(int key);
    void handleUp(int key);
private:
    Map *map;
    SpriteList playerList;
    SpriteList itemList;
};
