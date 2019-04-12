#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "twowayMultisprite.h"
#include "player.h"
#include "viewport.h"

// class Player;

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

  Player* player;
  std::vector<TwowayMultiSprite*> aliens;
  int currentSprite;

  bool makeVideo;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&);
  Engine& operator=(const Engine&);
  void printScales() const;
  void checkForCollisions();
};
