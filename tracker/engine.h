#include <vector>
#include <SDL.h>
// #include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "twowayMultisprite.h"
#include "player.h"
#include "viewport.h"
#include "sound.h"
#include "hud.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
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
  Hud& hud;
  bool hudOn;
  bool gameOver;
  SDLSound sound;

  Player* player;
  int score;
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
