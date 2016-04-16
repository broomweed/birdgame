/* Maybe this is a good tile size? I really don't know. */
#define TILE_SIZE 32
#define SCRWIDTH 640
#define SCRHEIGHT 512

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "TileMap.hpp"
#include "SpriteSheet.hpp"
#include "Animation.hpp"
#include "Entity.hpp"

using std::vector;

typedef enum walk {
    nowalk = 0,
    charwalk = 1,
    birdwalk = 2,
} walk;

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "birds");
    sf::View camera(sf::FloatRect(0, 0, SCRWIDTH/2, SCRHEIGHT/2));
    window.setView(camera);
    
    window.setVerticalSyncEnabled(true);

    bool arrows[4] = { false, false, false, false }; // the return of the Spice System

    Entity charac(2, 2, down, 2);
    SpriteSheet ssCharac("guyreal.png", 3, 4);
    Animation aChDS;
    aChDS.addFrame(ssCharac.getSprite(0, 0), 1000);
    Animation aChDW;
    aChDW.addFrame(ssCharac.getSprite(0, 0), 250);
    aChDW.addFrame(ssCharac.getSprite(1, 0), 250);
    aChDW.addFrame(ssCharac.getSprite(0, 0), 250);
    aChDW.addFrame(ssCharac.getSprite(2, 0), 250);
    Animation aChUS;
    aChUS.addFrame(ssCharac.getSprite(0, 1), 1000);
    Animation aChUW;
    aChUW.addFrame(ssCharac.getSprite(0, 1), 250);
    aChUW.addFrame(ssCharac.getSprite(1, 1), 250);
    aChUW.addFrame(ssCharac.getSprite(0, 1), 250);
    aChUW.addFrame(ssCharac.getSprite(2, 1), 250);
    Animation aChRS;
    aChRS.addFrame(ssCharac.getSprite(0, 2), 1000);
    Animation aChRW;
    aChRW.addFrame(ssCharac.getSprite(0, 2), 250);
    aChRW.addFrame(ssCharac.getSprite(1, 2), 250);
    Animation aChLS;
    aChLS.addFrame(ssCharac.getSprite(0, 3), 1000);
    Animation aChLW;
    aChLW.addFrame(ssCharac.getSprite(0, 3), 250);
    aChLW.addFrame(ssCharac.getSprite(1, 3), 250);
    charac.addAnim(down, aChDS);
    charac.addAnim(down, aChDW);
    charac.addAnim(up, aChUS);
    charac.addAnim(up, aChUW);
    charac.addAnim(right, aChRS);
    charac.addAnim(right, aChRW);
    charac.addAnim(left, aChLS);
    charac.addAnim(left, aChLW);
    charac.switchAnim(0);

    Entity bird(7, 1, down, 2);
    sf::Texture tex2;
    if (!tex2.loadFromFile("bird.png")) {
        std::cerr <<  "an error occurred :(" << std::endl;
        return -1;
    }
    sf::Sprite spr2(tex2);
    Animation ani2;
    ani2.addFrame(spr2, 1000);
    bird.addAnim(down, ani2);
    bird.addAnim(up, ani2);
    bird.addAnim(left, ani2);
    bird.addAnim(right, ani2);
    bird.switchAnim(0);

    int level[80] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 0, 0, 0, 2, 2, 2,
        0, 0, 0, 0, 0, 2, 2, 3, 3, 3,
        0, 0, 1, 1, 0, 3, 3, 3, 3, 3,
        7, 4, 4, 4, 6, 3, 3, 0, 0, 0,
        5, 0, 0, 0, 5, 0, 0, 0, 1, 0,
        9,11, 0,10, 8, 2, 2, 0, 0, 0,
        0, 0, 0, 0, 0, 3, 3, 2, 2, 2,
    };

    walk walkable[12] = {
        charwalk, nowalk, birdwalk, birdwalk, nowalk, nowalk, nowalk, nowalk, nowalk, nowalk, nowalk, nowalk
    };

    TileMap tilemap;
    if (!tilemap.load("tiles.png", sf::Vector2u(32, 32), level, 10, 8)) {
        fprintf(stderr, "something went wrong with the tileset!\n");
        return -1;
    }

    bool onbird = false;
    bool gettingonbird = false;

    while (window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            // if the event was a window close, close window. makes sense.
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                switch(event.key.code) {
                    case sf::Keyboard::Up:
                        arrows[0] = true;
                        break;
                    case sf::Keyboard::Left:
                        arrows[1] = true;
                        break;
                    case sf::Keyboard::Down:
                        arrows[2] = true;
                        break;
                    case sf::Keyboard::Right:
                        arrows[3] = true;
                        break;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                switch(event.key.code) {
                    case sf::Keyboard::Up:
                        arrows[0] = false;
                        break;
                    case sf::Keyboard::Left:
                        arrows[1] = false;
                        break;
                    case sf::Keyboard::Down:
                        arrows[2] = false;
                        break;
                    case sf::Keyboard::Right:
                        arrows[3] = false;
                        break;
                }
            }
        }

        // using arrow states, move guy around
        bool go = false;
        direction dirToGo = up;
        if (arrows[0]) {
            go = true;
            dirToGo = up;
        } else if (arrows[1]) {
            go = true;
            dirToGo = left;
        } else if (arrows[2]) {
            go = true;
            dirToGo = down;
        } else if (arrows[3]) {
            go = true;
            dirToGo = right;
        }

        if (go && !charac.moving) {
            charac.dir = dirToGo;
            charac.startMove();
            if (bird.x == charac.nextTileX() && bird.y == charac.nextTileY()) {
                onbird = true;
                gettingonbird = true;
            } else if (!(walkable[tilemap.at(charac.nextTileX(), charac.nextTileY())] & charwalk)) {
                if (!(onbird && (walkable[tilemap.at(charac.nextTileX(), charac.nextTileY())] & birdwalk))) {
                    charac.stopMove();
                }
            }
        }

        if (go && !bird.moving) {
            bird.dir = dirToGo;
            if (!gettingonbird) {
                bird.startMove();
            } else {
                if (charac.x == bird.x && charac.y == bird.y) {
                    gettingonbird = false;
                }
            }
            if (!(walkable[tilemap.at(bird.nextTileX(), bird.nextTileY())] & birdwalk)) {
                bird.stopMove();
                if (onbird && (walkable[tilemap.at(charac.nextTileX(), charac.nextTileY())] & charwalk)) {
                    onbird = false;
                }
            }
        }

        bird.update();
        charac.update();
        if (onbird && !gettingonbird) {
            charac.syncWith(bird);
        } else {
            bird.syncMove(charac);
        }

        window.clear(sf::Color::Blue);
        window.draw(tilemap);
        window.draw(bird);
        window.draw(charac);
        window.display();
    }
}
