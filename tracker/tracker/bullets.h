#ifndef BULLETS__H
#define BULLETS__H

#include <list>
#include "vector2f.h"
#include "drawable.h"
#include "bullet.h"

class CollisionStrategy;
class Bullets {
public:
  Bullets(const std::string&);
  Bullets(const Bullets&) = default;
  Bullets& operator=(const Bullets&) = default;
  ~Bullets() = default;
  void draw() const;
  void update(int ticks);
  void shoot(const Vector2f& pos, const Vector2f& objVel);

  bool shooting() const { return !bulletList.empty(); }
  bool collidedWith(const Drawable*) const;
private:
  std::string name;
  Vector2f myVelocity;
  std::list<Bullet> bulletList;
  CollisionStrategy* strategy;
};
#endif
