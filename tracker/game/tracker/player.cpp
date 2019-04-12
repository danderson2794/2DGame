#include "player.h"
#include "gamedata.h"
#include "imageFactory.h"
#include "bullets.h"
#include "collisionStrategy.h"
#include "explodingSprite.h"

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Player::~Player( ) {
  delete collisionStrategy;
  delete explosion;
}

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  imagesRight( ImageFactory::getInstance()->getImages(name) ),
  imagesLeft( ImageFactory::getInstance()->getImages(name+"Left") ),
  images( imagesRight ),
  observers(),
  collide(false),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  initialVelocity(getVelocity()),

  collisionStrategy( new PerPixelCollisionStrategy ),
  explosion( nullptr ),

  bulletName(Gamedata::getInstance().getXmlStr(name+"/bulletName")),
  bullets(bulletName),
  bulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0)
{ }

void Player::draw() const { 
  if ( explosion ) {
    explosion->draw();
    return;
  }
  images[currentFrame]->draw(getX(), getY(), getScale());
  bullets.draw();
}

bool Player::shot(const Drawable* obj ) {
  if ( bullets.collidedWith(obj) ) return true;
  else return false;
}

bool Player::collidedWith(const Drawable* obj ) {
  if ( explosion ) return false;
  if ( collisionStrategy->execute(*this, *obj) ) {
    return true;
  }
  return false;
}

void Player::explode() { 
  if ( !explosion ) {
    Sprite 
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    sprite.setScale( getScale() );
    explosion = new ExplodingSprite(sprite);
  }
}

void Player::shoot() { 
  // See if it's time to shoot a bullet:
	if (timeSinceLastBullet > bulletInterval) {
    Vector2f vel = getVelocity();
    float x; 
    float y = getY()+getScaledHeight()/4+11;
    if ( vel[0] > 0 ) {
      x = getX()+getScaledWidth()-10;
      vel[0] += bulletSpeed;
    }
    else {
      x = getX();
      vel[0] -= bulletSpeed;
    }
    bullets.shoot( Vector2f(x, y), vel ); 
		timeSinceLastBullet = 0;
  }
}

void Player::stop() { 
  //setVelocity( Vector2f(0, 0) );
  setVelocityX( 0.93*getVelocityX() );
  setVelocityY(0);
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    images = imagesRight;
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    images = imagesLeft;
  }
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::update(Uint32 ticks) { 
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);
  timeSinceLastBullet += ticks;
  bullets.update(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    images = imagesRight;
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
    images = imagesLeft;
  }  

  stop();
}
void Player::attach(Drawable* s) { observers.push_back(s); }
void Player::detach(Drawable* s) 
{
    auto it = observers.begin();
    while (it != observers.end() )
    {
        if ( *it == s )
        {
            it = observers.erase(it);
            return;
        }
        it++;
    }
}