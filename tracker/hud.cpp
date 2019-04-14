#include "hud.h"

//Singleton Implementation
Hud& Hud::getInstance()
{
    static Hud instance;
    return instance;
}

Hud::Hud() :
    text(Gamedata::getInstance().getXmlStr("HUD/message")),
    width(Gamedata::getInstance().getXmlInt("HUD/width")),
    height(Gamedata::getInstance().getXmlInt("HUD/height")),
    pos(Vector2f(Gamedata::getInstance().getXmlInt("HUD/position/x"),
                 Gamedata::getInstance().getXmlInt("HUD/position/y"))),
    power(true),
    //Default main color is black, you can change this in the game.xml file
    backColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/backColor/r")),
              static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/backColor/g")),
              static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/backColor/b")),
              static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/backColor/a"))}),
                      //Default main color is cyan, you can change this in the game.xml file
                      mainColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/mainColor/r")),
                                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/mainColor/g")),
                                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/mainColor/b")),
                                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/mainColor/a"))})
{ }

void Hud::draw(int score)
{
    if (turnedOn())
    {
        // will be used to draw the actual hud
        SDL_Rect box;

        //grabbing starting position from our pos vect2f
        box.x = getPosition()[0];
        box.y = getPosition()[1];

        box.w = getWidth();
        box.h = getHeight();

        //The code below came from Dr. Malloy's example. I just modified it to work for a class in this project.
        SDL_Renderer* renderer = IoMod::getInstance().getRenderer();

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);

        SDL_RenderFillRect(renderer, &box);
        SDL_SetRenderDrawColor(renderer, mainColor.r, mainColor.g, mainColor.b, mainColor.a);
        SDL_RenderDrawRect(renderer, &box);

        //Creates stream to be printed as the HUD.
        std::ostringstream gameInfo;
        gameInfo << getMessage();

        IoMod::getInstance().writeText(gameInfo.str(), getPosition()[0]+30, getPosition()[1]+10, getMainColor()),
              getWidth(),
              getMainColor();
        
        gameInfo.str("");
        gameInfo.clear();
        gameInfo << "Keep shooting the aliens until you die!";

        IoMod::getInstance().writeText(gameInfo.str(), getPosition()[0]+30, getPosition()[1]+50, getMainColor()),
              getWidth(),
              getMainColor();




        gameInfo.str("");
        gameInfo.clear();
        gameInfo << "Your score is: " << score;

        IoMod::getInstance().writeText(gameInfo.str(), getPosition()[0]+30, getPosition()[1]+90, getMainColor()),
              getWidth(),
              getMainColor();

        gameInfo.str("");
        gameInfo.clear();

        gameInfo << "Press F1 to turn toggle this HUD\n";
        IoMod::getInstance().writeText(gameInfo.str(), getPosition()[0]+30, getPosition()[1]+130, getMainColor()),
              getWidth(),
              getMainColor();
        


    }
}
