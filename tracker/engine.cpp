#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>

#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
// #include "twowayMultisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"

Engine::~Engine() { 
  
  for (auto it : aliens)
  {
      delete it;
  }

  delete player;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  space("space", Gamedata::getInstance().getXmlInt("space/factor") ),
  mountains("mountains", Gamedata::getInstance().getXmlInt("mountains/factor") ),
  lines("lines", Gamedata::getInstance().getXmlInt("lines/factor") ),
  viewport( Viewport::getInstance() ),
  hud( Hud::getInstance() ), //using Singleton
  hudOn(true),
  gameOver(false),
  sound(),
  player(new Player("plane")),
  score(0),
  aliens(),
  currentSprite(0),
  makeVideo( false )
{
  player->setScale(.8);
  for (int i = 0; i < Gamedata::getInstance().getXmlInt("numberOfAliens"); i++)
  {
    sound.toggleMusic();
    aliens.emplace_back( new TwowayMultiSprite("alien", player));
    aliens.at(i)->setScale(.8);
    player->attach(aliens.at(i));
  }


  
  sound.toggleMusic();  
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  space.draw();
  mountains.draw();
  lines.draw();
  if (gameOver)
  {
    //io.deathText("Oh dear, you've died!", 600, 300, {255, 0, 0, 0});
    //clock.pause();
  }
  //iterate through the vector and draw.
  for(TwowayMultiSprite* s : aliens)
  {
    s->draw();
  }
  if(hudOn) 
  {
    hud.draw(score);
  }
  player->draw();

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {

  for(TwowayMultiSprite* a : aliens) a->update(ticks, player);
  if(gameOver)
  {
    score = 0;
    gameOver = false;
    //sound.toggleMusic();
  }
  player->update(ticks);
  
  space.update();
  mountains.update();
  lines.update();
  viewport.update(); // always update viewport last
}

void Engine::checkForCollisions() {
  for(TwowayMultiSprite* s : aliens)
  {
    if ( player->collidedWith(s) ) {
      player->explode();
      s->explode();
      gameOver = true;
      
    }
    if ( player->shot(s) ) {
      s->explode();
      score++;
    }
  }
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % 2;
  if ( currentSprite ) {
    Viewport::getInstance().setObjectToTrack(player);
  }
  else {
    Viewport::getInstance().setObjectToTrack(aliens.at(0));
  }
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
      if(keystate[SDL_SCANCODE_Y])
      {
        clock.unpause();    
      }
      if(keystate[SDL_SCANCODE_F1])
      {
        if (hudOn) {hudOn = false;}
        else { hudOn = true;}
      }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      
      if (keystate[SDL_SCANCODE_SPACE]) {
        static_cast<Player*>(player)->shoot();
      }
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }
      checkForCollisions();
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
