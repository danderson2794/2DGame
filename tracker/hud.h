#ifndef HUD__H
#define HUD__H

#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <algorithm>
#include "gamedata.h"
#include "ioMod.h"
#include "vector2f.h"

class Hud
{
public:
    //Meyer Singleton method used. This will be used to call my constructor
    static Hud& getInstance();

    /**********************************************************************
                        Getters
    ***********************************************************************/

    //SDL getters derived from HUD example, just made modular for a class
    SDL_Color getBackColor()
    {
        return backColor;
    }
    SDL_Color getMainColor()
    {
        return mainColor;   // I will use Cyan for the text and outline.
    }

    const std::string& getMessage() const
    {
        return text;
    }
    int getWidth() const
    {
        return width;
    }
    int getHeight() const
    {
        return height;
    }
    const Vector2f& getPosition() const
    {
        return pos;
    }
    bool turnedOn() const
    {
        return power;
    }

    /**********************************************************************
                         Setters
    ***********************************************************************/
    //This will be the background of the HUD.
    void setBackColor(const SDL_Color& back)
    {
        backColor = back;
    }
    //Main color will be the text and outline color.
    void setMainColor(const SDL_Color& main)
    {
        mainColor = main;
    }


    //This will set the message displayed in the HUD.
    void setText(const std::string& t)
    {
        text = t;
    }
    //This will change the width of the HUD
    void setWidth(const int w)
    {
        width = w;
    }
    //This will change the height of the HUD
    void setHeight(const int h)
    {
        height = h;
    }
    //This will change the position of the HUD
    void setPos(const Vector2f& p)
    {
        pos = p;
    }
    //This will change the status of the HUD being displayed.
    void setPower(bool p)
    {
        power = p;
    }

    //used to draw to terminal.
    void draw(const int score);

private:
    std::string text;
    int width, height;
    Vector2f pos;
    bool power;
    SDL_Color backColor, mainColor;


    //Below are the methods needed by Modern Effective C++ for using singleton
    Hud();
    Hud(const Hud&) = delete;
    Hud& operator=(const Hud&) = delete;
};



#endif
