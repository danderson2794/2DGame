#include <iostream>
#include <cmath>
#include "explodingSprite.h"

const int DIRECTION_MOD = 100;
const int MAX_EXPLOSION_SPD = 200;
const int MIN_EXPLOSION_SPD = 40;
const int MAX_DISTANCE = 150;

ExplodingSprite::ExplodingSprite(const Sprite& s) :
  Sprite(s), 
  chunks(), 
  freeList(){
  //std::cout << "scale: " << getScale() << std::endl;
  makeChunks(
    Gamedata::getInstance().getXmlInt(s.getName()+"/chunk/size")
  );
}

ExplodingSprite::~ExplodingSprite() { 
  for ( Chunk* c : chunks ) {
    delete c;
  }
  for ( Chunk* c : freeList ) {
    delete c;
  }
  chunks.clear();     // clearing a vector is a waste of time, but ...
  freeList.clear();   // still ...
}

void ExplodingSprite::draw() const { 
  // We use the draw in Chunk, which derives from Sprite.
  // So the draw we're using is actually in Sprite
  for ( Chunk* chunk : chunks ) {
    chunk->draw();
  }
}

void ExplodingSprite::update(Uint32 ticks) { 
  std::list<Chunk*>::iterator ptr = chunks.begin();
  while (ptr != chunks.end()) {
    (*ptr)->update(ticks);
    if ( (*ptr)->goneTooFar()) {  // Check to see if we should free a chunk
      freeList.push_back(*ptr);
      ptr = chunks.erase(ptr);
    }   
    else ++ptr;
  }
}

// Shamelessly borrowed/stolen from Matt Dabney -- thanks Matt!
Vector2f ExplodingSprite::getNewVelocity() const {
  // These are used to calculate the direction of the chunk velocity 
  // and have no bearing on its magnitude
  float x_dir = (rand() % DIRECTION_MOD) * (rand()%2?-1:1);
  float y_dir = (rand() % DIRECTION_MOD) * (rand()%2?-1:1);

  // The if statement makes sure we don't try to normalize a Vector2f(0,0) 
  // (which will give a runtime crash)
  if (fabs(x_dir) < 0.001 && fabs(y_dir) < 0.001) {
    x_dir += rand()%2?-1:1;
    y_dir += rand()%2?-1:1;
  }

  // This vector will eventually hold the velocity, 
  // it now holds just the direction:
  Vector2f velocity(x_dir, y_dir);   

  // We normalize to get rid of the magnitude of the above calculations, 
  // but keep direction:
  velocity = velocity.normalize();   

  // Now compute a random speed between MIN_EXPLOSION_SPD and
  // MAX_EXPLOSION_SPD
  int speed = (rand() % (MAX_EXPLOSION_SPD - MIN_EXPLOSION_SPD)) 
              + MIN_EXPLOSION_SPD;

  // We now scale the unit-length direction of velocity by the speed above
  velocity *= speed;

  // velocity is now a random Vector2f in 360 degrees,
  // with magnitude between the min and max speeds.
  return velocity;
}

void ExplodingSprite::makeChunks(unsigned int n) { 
  // Break the SDL_Surface into n*n squares; where each square
  // has width and height of imageWidth/n and imageHeight/n
  // Note that "n" s/b a perfect square.
  int chunk_width = std::max(1u, getImage()->getWidth()/n);
  int chunk_height = std::max(1u, getImage()->getHeight()/n);
  int speedx = static_cast<int>(getVelocityX()); // Wanna test for zero...
  int speedy = static_cast<int>(getVelocityY()); // Make sure it's an int.
  if (speedx == 0) speedx = 1; // Make sure it's not 0;
  if (speedy == 0) speedy = 1; // Make sure it's not 0;

  const Image* proto = getImage();

  int source_y = 0;
  while  (source_y+chunk_height < getImage()->getHeight() ) {
    int source_x = 0;
    while ( source_x+chunk_width < getImage()->getWidth() ) {
      Image* image = 
        proto->crop({source_x,source_y,chunk_width,chunk_height});
      Chunk* chunk = new Chunk(
                Vector2f(getX()+getScale()*source_x,   // x coord of dest 
                         getY()+getScale()*source_y),  // y coord of dest
                getNewVelocity(),
                getName()+"/chunk",
                image);
      chunk->setScale( getScale() );
      chunks.push_back(chunk);
      source_x += chunk_width;
    }
    source_y += chunk_height;
  }
}

