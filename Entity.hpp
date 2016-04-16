typedef enum direction { left, right, up, down } direction;

class Entity : public sf::Drawable {
    public:
        /* The entity's position, measured in tiles. */
        int x;
        int y;
        /* The entity's current facing/moving direction. */
        direction dir;
        /* Whether or not the entity is currently moving between tiles. */
        bool moving;
        /* The speed at which the entity moves, measured in pixels. */
        int speed;

        Entity(int x, int y, direction dir, int speed)
                : x(x), y(y), dir(dir), speed(speed) {
            dist = 0;
            moving = false;
            canmove = true;
        };

        void addAnim(direction way, Animation anim) {
            anims[(int)way].push_back(anim);
        }

        void switchAnim(int newID) {
            // maybe reset current one somehow?
            currentAnimID = newID;
        }

        void update() {
            anims[(int)dir][currentAnimID].update();
            if (moving) {
                if (currentAnimID != 1 && anims[(int)dir].size() > 1) {
                    switchAnim(1);
                }
                dist += speed;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < anims[i].size(); j++) {
                        anims[i][j].setPosition((float)getPixelPosition().x, (float)getPixelPosition().y);
                    }
                }
                if (dist >= TILE_SIZE) {
                    x = nextTileX();
                    y = nextTileY();
                    stopMove();
                }
            } else {
                if (currentAnimID != 0) {
                    switchAnim(0);
                }
            }
        }

        int nextTileX() {
            if (moving) {
                switch(dir) {
                    case left:
                        return x - 1;
                        break;
                    case right:
                        return x + 1;
                        break;
                    default:
                        return x;
                }
            }
        }

        int nextTileY() {
            if (moving) {
                switch(dir) {
                    case up:
                        return y - 1;
                        break;
                    case down:
                        return y + 1;
                        break;
                    default:
                        return y;
                }
            }
        }

        void startMove() {
            if (canmove) {
                moving = true;
            }
        }

        void stopMove() {
            if (moving) {
                dist = 0;
                moving = false;
            }
        }

        void syncWith(Entity other) {
            x = other.x;
            y = other.y;
            dir = other.dir;
            dist = other.dist;
        }

        void syncMove(Entity other) {
            canmove = !other.moving;
            other.canmove = !moving;
        }

        sf::Vector2i getPixelPosition() {
            if (dist == 0) {
                return sf::Vector2i(x * TILE_SIZE, y * TILE_SIZE);
            } else {
                switch(dir) {
                    case left:
                        return sf::Vector2i(x * TILE_SIZE - dist, y * TILE_SIZE);
                        break;
                    case right:
                        return sf::Vector2i(x * TILE_SIZE + dist, y * TILE_SIZE);
                        break;
                    case up:
                        return sf::Vector2i(x * TILE_SIZE, y * TILE_SIZE - dist);
                        break;
                    case down:
                        return sf::Vector2i(x * TILE_SIZE, y * TILE_SIZE + dist);
                        break;
                }
            }
        }

    protected:
        /* The entity's distance along its current path, measured in pixels. */
        int dist;
        /* The various animations that the entity can have -- 4 vectors indexed by direction. */
        vector<Animation> anims[5];
        /* The ID of the animation with which the entity will be displayed. */
        int currentAnimID;
        /* Whether or not this entity can move. Always true, unless it's synced up with another one
         * (e.g. the bird can't start moving when the guy is in the middle of his move.) */
        bool canmove;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            target.draw(anims[(int)dir][currentAnimID]);
        }
};
