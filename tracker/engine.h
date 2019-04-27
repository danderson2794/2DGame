#include <SDL.h>
#include "renderContext.h"
//#include "clock.h"
#include "world.h"
#include "twowayMultisprite.h"
#include "player.h"
#include "viewport.h"
#include "hud.h"
#include "sound.h"
#include "menuEngine.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World space;
  World mountains;
  World lines;
  Viewport& viewport;
  MenuEngine menuEngine;

  SDLSound sound;

  Player* player;
  int score;
  std::vector<TwowayMultiSprite*> aliens;
  Hud& hud;
  bool HUDon;
  bool makeVideo;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&);
  Engine& operator=(const Engine&);
  void printScales() const;
  void checkForCollisions();
};
