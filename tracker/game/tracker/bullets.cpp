#include <iostream>
#include <cmath>
#include "bullets.h"
#include "imageFactory.h"
#include "collisionStrategy.h"

Bullets::Bullets(const std::string& n) :
  name(n),
  myVelocity(
    Gamedata::getInstance().getXmlInt(name+"/speed/x"), 
    Gamedata::getInstance().getXmlInt(name+"/speed/y")
  ),
  bulletList(),
  strategy(NULL)
{ 
  const string thisStrategy = 
    Gamedata::getInstance().getXmlStr("collisionStrategy");
  if ( thisStrategy == "PerPixel" ) {
    strategy = new PerPixelCollisionStrategy;
  }
  else if ( thisStrategy == "Rectangular" ) {
    strategy = new RectangularCollisionStrategy;
  }
  else if ( thisStrategy == "MidPoint" ) {
    strategy = new MidPointCollisionStrategy;
  }
}

void Bullets::shoot(const Vector2f& pos, const Vector2f& objVel) {
  Bullet b( name,  pos, objVel );
  bulletList.push_back( b );
}

bool Bullets::collidedWith(const Drawable* obj) const { 
  for ( const auto & bullet : bulletList ) {
    if ( strategy->execute(bullet, *obj) ) {
      return true;
    }
  }
  return false;
}

void Bullets::draw() const { 
  for ( const auto& bullet : bulletList ) {
    bullet.draw();
  }
}

void Bullets::update(int ticks) { 
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a chunk
      ptr = bulletList.erase(ptr);
    }   
    else ++ptr;
  }
}
