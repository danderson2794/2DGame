#ifndef BULLET__H
#define BULLET__H

#include "sprite.h"

class Bullet : public Sprite {
public:
  Bullet( const std::string&, const Vector2f&, const Vector2f& );
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset();

private:
  float distance;
  float maxDistance;
  bool tooFar;
};
#endif
