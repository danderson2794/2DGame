#include <cmath>
#include <random>
#include <functional>
#include "twowayMultisprite.h"
#include "gamedata.h"
#include "imageFactory.h"

#include "collisionStrategy.h"
#include "explodingSprite.h"

void TwowayMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

TwowayMultiSprite::TwowayMultiSprite( const std::string& name, const Drawable* p) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  imagesRight( ImageFactory::getInstance()->getImages(name) ),
  imagesLeft( ImageFactory::getInstance()->getImages(name+"Left") ),
  images( imagesRight ),
  explosion( nullptr ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  //below are for the AI. 
  playerPos(p->getPosition()),
  playerWidth(p->getScaledWidth()),
  playerHeight(p->getScaledHeight()),
  currentMode(NORMAL),
  safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDistance"))
  {
    float vx = getVelocityX();
    float vy = getVelocityY();
    float newvx = Gamedata::getInstance().getRandFloat(vx-50, vx+50);
    float newvy = Gamedata::getInstance().getRandFloat(vy-50, vy+50);
    newvx *= []()
    {
        if(rand()%2) return -1;
        else return 1;
    }
    ();
    newvy *= []()
    {
        if(rand()%2) return -1;
        else return 1;
    }
    ();
    setVelocity(Vector2f(newvx, newvy));
  }

void TwowayMultiSprite::explode() {
  if ( !explosion ) {
    Sprite 
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    sprite.setScale( getScale() );
    explosion = new ExplodingSprite(sprite);
  }
}

void TwowayMultiSprite::draw() const { 
  if ( explosion ) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());
}

void TwowayMultiSprite::update(Uint32 ticks) { 
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
}

void TwowayMultiSprite::update(Uint32 ticks, const Drawable* p) { 
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  float x= getX()+getImage()->getWidth()/2;
  float y= getY()+getImage()->getHeight()/2;

  float ex= p->getX()+playerWidth/2;
  float ey= p->getY()+playerHeight/2;


  float distanceToEnemy = hypot((x-ex),(y-ey));
  //::distance( x, y, ex, ey );


  if  ( currentMode == NORMAL )
  {
    if(distanceToEnemy < safeDistance) currentMode = EVADE;
  }
  else if  ( currentMode == EVADE )
  {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else
    {
      if ( x < ex ) goLeft();
      if ( x > ex ) goRight();
      if ( y < ey ) goUp();
      if ( y > ey ) goDown();
    }
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() + getScaledHeight() > worldHeight) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    images = imagesRight;
  }
  if ( getX() + getScaledWidth() > worldWidth) 
  {
    setVelocityX( -fabs( getVelocityX() ) );
    images = imagesLeft;
  }  

}

void TwowayMultiSprite::goLeft()
{
    setVelocityX( -abs(getVelocityX()) );
}
void TwowayMultiSprite::goRight()
{
    setVelocityX( fabs(getVelocityX()) );
}
void TwowayMultiSprite::goUp()
{
    setVelocityY( -fabs(getVelocityY()) );
}
void TwowayMultiSprite::goDown()
{
    setVelocityY( fabs(getVelocityY()) );
}
