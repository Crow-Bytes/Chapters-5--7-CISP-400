// Chapter 7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Bat.h"
#include "Ball.h"
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    int lM = 1600; //Set Monitor length
    int wM = 900;  //Set Montior width / height

    VideoMode vm(lM, wM);
    RenderWindow window(vm, "Pong", Style::Default);
    int score = 0;
    int lives = 3;

    Bat bat(lM / 2, wM - 20);
    Ball ball(lM / 2, 0);

    Text hud;
    Font font;
    font.loadFromFile("fonts/DS-DIGI.ttf");

    //Set font to retro-style
    hud.setFont(font);
    hud.setCharacterSize(75);
    hud.setPosition(20, 20);
    hud.setFillColor(Color::White);

    Clock clock;
    bool hitBat = false; //Prevents multiple points from one hit.

    while (window.isOpen())
    {
        //Handle Player Input//////////////////////////
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left) && bat.getPosition().left > 0)
        {
            bat.moveLeft();
        }
        else
        {
            bat.stopLeft();
        }

        if (Keyboard::isKeyPressed(Keyboard::Right) && (bat.getPosition().left + bat.getPosition().width) < lM)
        {
            bat.moveRight();
        }
        else
        {
            bat.stopRight();
        }

        ///Update objects////////////////////

        Time dt = clock.restart();

        bat.update(dt);
        ball.update(dt);

        stringstream ss;
        ss << "Score: " << score << "   Lives: " << lives;
        hud.setString(ss.str());

        //handle ball hitting the bottom
        if (ball.getPosition().top > window.getSize().y)
        {
            ball.reboundBottom();
            lives--;

            if (lives < 1) //GAME OVER
            {
                score = 0; //Set score to 0
                lives = 3; //Reset life count with score
            }
            hitBat = false;
        }

        //Handle ball hitting the top
        if (ball.getPosition().top < 0)
        {
            ball.reboundBatOrTop();
            hitBat = false;
        }

        //Handle ball hitting the sides
        if (ball.getPosition().left < 0 || (ball.getPosition().left + ball.getPosition().width) > window.getSize().x)
        {
            ball.reboundSides();
            hitBat = false;

        }

        //has the ball hit the bat?
        if (ball.getPosition().intersects(bat.getPosition()) && !hitBat)
        {
            hitBat = true;
            //Hit detected so reverse the ball and score a point
            ball.reboundBatOrTop();
            score++;
        }


        //Draw

        window.clear();
        window.draw(hud);
        window.draw(bat.getShape());
        window.draw(ball.getShape());
        window.display();
    }
}


