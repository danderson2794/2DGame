#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class CollisionStrategy;
class ExplodingSprite;

class TwowayMultiSprite : public Drawable {
public:
  TwowayMultiSprite(const std::string&, const Drawable* p);

  virtual void draw() const;
  void update(Uint32 ticks, const Drawable* p);
  virtual bool collidedWith(const Drawable*) {throw std::string("I don't collide: ")+getName();}
  virtual void update(Uint32 ticks);

  virtual void explode();

  virtual const Image* getImage() const {return images[currentFrame];}
  int getScaledWidth()  const {return getScale()*images[currentFrame]->getWidth();} 
  int getScaledHeight()  const {return getScale()*images[currentFrame]->getHeight();} 
  virtual const SDL_Surface* getSurface() const {return images[currentFrame]->getSurface();}

  TwowayMultiSprite(const TwowayMultiSprite&) = default;
  TwowayMultiSprite& operator=(const TwowayMultiSprite&) = default;

protected:
  std::vector<Image *> imagesRight;
  std::vector<Image *> imagesLeft;
  std::vector<Image *> images;
  ExplodingSprite* explosion;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  
  //AI methods.
  enum MODE {NORMAL, EVADE};
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float safeDistance;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();

  void advanceFrame(Uint32 ticks);
};
#endif
