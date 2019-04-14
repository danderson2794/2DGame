// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"

ImageFactory*  ImageFactory::instance = NULL;
RenderContext* RenderContext::instance = NULL;

int main(int, char*[]) {
   bool keepPlaying = false;
   while (!keepPlaying)
   {
      try {
         Engine* engine = new Engine();
         keepPlaying = engine->play();
         delete RenderContext::getInstance();
         delete engine;
      }
      catch (const string& msg) { std::cout << msg << std::endl; }
      catch (...) {
         std::cout << "Oops, someone threw an exception!" << std::endl;
      }
   }
   return 0;
}
