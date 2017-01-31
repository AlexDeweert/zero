#ifndef PLAYER_INCL
#define PLAYER_INCL
#include "Tile.h"
#include "Global.h"
using namespace std;

class Player
{
public:
	Player(float x, float y, float bx, float by);
	
	//movement
	void goUp(list<Tile>& tList, list<Tile>& dtList);
	void goDown(list<Tile>& tList, list<Tile>& dtList);
	void goRight(list<Tile>& tList, list<Tile>& dtList);
	void goLeft(list<Tile>& tList, list<Tile>& dtList);

	//attack logic
	void attackRight(bool keys[], ALLEGRO_SAMPLE *sword);
	void attackLeft(bool keys[], ALLEGRO_SAMPLE *sword);
	void attackUp(bool keys[], ALLEGRO_SAMPLE *sword);
	void attackDown(bool keys[], ALLEGRO_SAMPLE *sword);

	//movement animations
	void rightAnimation(ALLEGRO_BITMAP *link, Player &plr, int &curFrame);
	void leftAnimation(ALLEGRO_BITMAP *link, Player &plr, int &curFrame);
	void upAnimation(ALLEGRO_BITMAP *link, Player &plr, int &curFrame);
	void downAnimation(ALLEGRO_BITMAP *link, Player &plr, int &curFrame);

	//attack animations
	void attackLeftAnimation(ALLEGRO_BITMAP *link, Player &plr);
	void attackRightAnimation(ALLEGRO_BITMAP *link, Player &plr);
	void attackUpAnimation(ALLEGRO_BITMAP *link, Player &plr);
	void attackDownAnimation(ALLEGRO_BITMAP *link, Player &plr);

	//standing animations
	void standingFacingUp(ALLEGRO_BITMAP *link, Player &plr, int &curFrame);
	void standingFacingDown(ALLEGRO_BITMAP *link, Player &plr, int &curFrame);
	void standingFacingLeft(ALLEGRO_BITMAP *link, Player &plr, int &curFrame);
	void standingFacingRight(ALLEGRO_BITMAP *link, Player &plr, int &curFrame);

	//void drawPlayer();
	void resolveCollisions(list<Tile>&);
	void resolveDestructableTileCollisions(list<Tile>& dtList);
	void updateAttackFrameCount(ALLEGRO_SAMPLE *sword);

	float V;

	//player x, y, and bounds
	float x;
	float y;
	float bx;
	float by;

	//right box
	int rbx;
	int rby;
	int rbw;
	int rbh;

	//left box
	int lbx;
	int lby;
	int lbw;
	int lbh;

	//top box
	int tbx;
	int tby;
	int tbw;
	int tbh;

	//bottom box
	int bbx;
	int bby;
	int bbw;
	int bbh;
	int facing;

	//attack animations
	const int atk_maxFrame;
	int atk_curFrame;
	int atk_frameCount;
	int atk_frameDelay;
	int atk_iterations;

	bool moving;
	bool attacking;
	bool hitOnce;
	bool playedHitSound;
	bool completedAttack;
	bool atkButtonPressed;
};

Player::Player(float x, float y, float bx, float by) //CTOR
	:atk_maxFrame (5)
{
	this->atk_curFrame = 0;
	this->atk_frameCount = 0;
	this->atk_frameDelay = 10;
	this->atk_iterations = 0;
	this->atkButtonPressed = false;
	this->completedAttack = false;
	this->playedHitSound = false;
	this->hitOnce = false;
	this->attacking = false;
	this->moving = false;
	this->V = 4.0;
	this->x = x;
	this->y = y;
	this->bx = bx;
	this->by = by;
	this->facing = DOWN;

	//right box
	this->rbx = this->x + this->bx;
	this->rby = this->y;
	this->rbw = PLAYERWIDTH;
	this->rbh = PLAYERHEIGHT;

	//left box
	this->lbx = this->x - this->bx;
	this->lby = this->y;
	this->lbw = PLAYERWIDTH;
	this->lbh = PLAYERHEIGHT;

	//top box
	this->tbx = this->x;
	this->tby = this->y - this->by;
	this->tbw = PLAYERWIDTH;
	this-> tbh = PLAYERHEIGHT;

	//bottom box
	this->bbx = this->x;
	this->bby = this->y + this->by;
	this->bbw = PLAYERWIDTH;
	this->bbh = PLAYERHEIGHT;
}
void Player::goUp(list<Tile>& tList, list<Tile>& dtList)
{
	if(this->facing != UP)
		this->facing = UP;

	this->rby -= V;
	this->lby -= V;
	this->bby -= V;
	this->tby -= V;

	this->y -= V; //player Y position minusEQ V (go up)
	resolveCollisions(tList);
	resolveDestructableTileCollisions(dtList);
}

void Player::goDown(list<Tile>& tList, list<Tile>& dtList)
{
	if(this->facing != DOWN)
		this->facing = DOWN;

	this->rby += V;
	this->lby += V;
	this->bby += V;
	this->tby += V;

	this->y += V;
	resolveCollisions(tList);
	resolveDestructableTileCollisions(dtList);

} 
void Player::goLeft(list<Tile>& tList, list<Tile>& dtList)
{
	if(this->facing != LEFT)
		this->facing = LEFT;

	this->rbx -= V;
	this->lbx -= V;
	this->bbx -= V;
	this->tbx -= V;

	this->x -= V;
	resolveCollisions(tList);
	resolveDestructableTileCollisions(dtList);
	
} 
void Player::goRight(list<Tile>& tList, list<Tile>& dtList)
{
	if(this->facing != RIGHT)
		this->facing = RIGHT;

		this->rbx += V; //right box
		this->lbx += V;
		this->bbx += V;
		this->tbx += V;

		this->x += V; 
		resolveCollisions(tList);
		resolveDestructableTileCollisions(dtList);
} 

void Player::attackRight(bool keys[], ALLEGRO_SAMPLE *sword)
{
			if(keys[N] && facing == RIGHT)
				atkButtonPressed = true;
			
			if(atkButtonPressed == true)
			{
				if(hitOnce == false)
					{
						attacking = true;
						updateAttackFrameCount(sword);//Update attack frame
					}
					else
					{
						atkButtonPressed = false;
						attacking = false;
					}
			}

			else if(!keys[N] && facing == RIGHT)
			{
				if(!attacking)
				{
					atkButtonPressed = false;
					hitOnce = false;
					atk_curFrame = 0;
					atk_frameCount = 0;
				}
				
			}
}
void Player::attackLeft(bool keys[], ALLEGRO_SAMPLE *sword)
{
if(keys[N] && facing == LEFT)
				atkButtonPressed = true;
			
			if(atkButtonPressed == true)
			{
				if(hitOnce == false)
					{
						attacking = true;
						updateAttackFrameCount(sword);//Update attack frame
					}
					else
					{
						atkButtonPressed = false;
						attacking = false;
					}
			}

			else if(!keys[N] && facing == LEFT)
			{
				if(!attacking)
				{
					atkButtonPressed = false;
					hitOnce = false;
					atk_curFrame = 0;
					atk_frameCount = 0;
				}
			}
}
void Player::attackUp(bool keys[], ALLEGRO_SAMPLE *sword)
{
			if(keys[N] && facing == UP)
				atkButtonPressed = true;
			
			if(atkButtonPressed == true)
			{
				if(hitOnce == false)
					{
						attacking = true;
						updateAttackFrameCount(sword);//Update attack frame
					}
					else
					{
						atkButtonPressed = false;
						attacking = false;
					}
			}

			else if(!keys[N] && facing == UP)
			{
				if(!attacking)
				{
					atkButtonPressed = false;
					hitOnce = false;
					atk_curFrame = 0;
					atk_frameCount = 0;
				}
				
			}
}
void Player::attackDown(bool keys[], ALLEGRO_SAMPLE *sword)
{
if(keys[N] && facing == DOWN)
				atkButtonPressed = true;
			
			if(atkButtonPressed == true)
			{
				if(hitOnce == false)
					{
						attacking = true;
						updateAttackFrameCount(sword);//Update attack frame
					}
					else
					{
						atkButtonPressed = false;
						attacking = false;
					}
			}

			else if(!keys[N] && facing == DOWN)
			{
				if(!attacking)
				{
					atkButtonPressed = false;
					hitOnce = false;
					atk_curFrame = 0;
					atk_frameCount = 0;
				}
				
			}
}

void Player::rightAnimation(ALLEGRO_BITMAP *link, Player &plr, int &curFrame)
{
	switch(curFrame)
	{
	case 0:
		al_draw_bitmap_region(link, 180, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		break;
	case 1:
		al_draw_bitmap_region(link, 178, 62, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		break;
	}
}
void Player::leftAnimation(ALLEGRO_BITMAP *link, Player &plr, int &curFrame)
{
	switch(curFrame)
	{
	case 0:
		al_draw_bitmap_region(link, 58, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		break;
	case 1:
		al_draw_bitmap_region(link, 58, 62, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		break;
	}
}
void Player::downAnimation(ALLEGRO_BITMAP *link, Player &plr, int &curFrame)
{
	switch(curFrame)
	{
	case 0:
		al_draw_bitmap_region(link, 0, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		break;
	case 1:
		al_draw_bitmap_region(link, 0, 60, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		break;
	}
}
void Player::upAnimation(ALLEGRO_BITMAP *link, Player &plr, int &curFrame)
{
	switch(curFrame)
	{
	case 0:
		al_draw_bitmap_region(link, 120, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		break;
	case 1:
		al_draw_bitmap_region(link, 120, 60, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		break;
	}
}

void Player::attackLeftAnimation(ALLEGRO_BITMAP *link, Player &plr)
{	
	switch(atk_curFrame)
	{
		case 0:
			al_draw_bitmap_region(link, 60, 120, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
			break;

		case 1:
			al_draw_bitmap_region(link, 780, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
			al_draw_bitmap_region(link, 771, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x - 9, plr.y, 0);

		case 2:
			al_draw_bitmap_region(link, 72, 180, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
			al_draw_bitmap_region(link, 40, 180, PLAYERWIDTH, PLAYERHEIGHT, plr.x - 32, plr.y, 0);
			break;

		case 3:
			al_draw_bitmap_region(link, 72, 180, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
			al_draw_bitmap_region(link, 40, 180, PLAYERWIDTH, PLAYERHEIGHT, plr.x - 32, plr.y, 0);
			break;

		case 4:
			al_draw_bitmap_region(link, 780, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
			al_draw_bitmap_region(link, 771, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x - 9, plr.y, 0);
	}
}
void Player::attackRightAnimation(ALLEGRO_BITMAP *link, Player &plr)
{
	switch(atk_curFrame)
	{
		case 0:
			al_draw_bitmap_region(link, 178, 120, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
			break;

		case 1:
			al_draw_bitmap_region(link, 718, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
			al_draw_bitmap_region(link, 750, 0, 15, PLAYERHEIGHT, plr.x + 32, plr.y, 0);
			break;
		
		case 2:
			al_draw_bitmap_region(link, 166, 180, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
			al_draw_bitmap_region(link, 198, 180, PLAYERWIDTH, PLAYERHEIGHT, plr.x + 32, plr.y, 0);
			break;

		case 3:
			al_draw_bitmap_region(link, 166, 180, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
			al_draw_bitmap_region(link, 198, 180, PLAYERWIDTH, PLAYERHEIGHT, plr.x + 32, plr.y, 0);
			break;

		case 4:
			al_draw_bitmap_region(link, 718, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
			al_draw_bitmap_region(link, 750, 0, 15, PLAYERHEIGHT, plr.x + 32, plr.y, 0);
			break;
	}
}
void Player::attackUpAnimation(ALLEGRO_BITMAP *link, Player &plr)
{
	switch(atk_curFrame)
	{
		case 0:
		al_draw_bitmap_region(link, 120, 120, PLAYERWIDTH , PLAYERHEIGHT, plr.x, plr.y, 0);
		break;

		case 1:
		al_draw_bitmap_region(link, 719, 60, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		al_draw_bitmap_region(link, 719, 35, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y - 25, 0);
		break;

		case 2:
		al_draw_bitmap_region(link, 120, 192, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		al_draw_bitmap_region(link, 120, 158, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y - 34, 0);
		break;

		case 3:
		al_draw_bitmap_region(link, 120, 192, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		al_draw_bitmap_region(link, 120, 158, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y - 34, 0);
		break;

		case 4:
		al_draw_bitmap_region(link, 719, 60, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		al_draw_bitmap_region(link, 719, 35, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y - 25, 0);
		break;
	}
}
void Player::attackDownAnimation(ALLEGRO_BITMAP *link, Player &plr)
{
	switch(atk_curFrame)
	{
		case 0:
		al_draw_bitmap_region(link, 0, 120, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		break;

		case 1:
		al_draw_bitmap_region(link, 779, 39, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		al_draw_bitmap_region(link, 779, 71, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y + 32, 0);
		break;

		case 2:
		al_draw_bitmap_region(link, 0, 168, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		al_draw_bitmap_region(link, 0, 200, PLAYERWIDTH, PLAYERHEIGHT - 4, plr.x, plr.y + 32, 0);
		break;

		case 3:
		al_draw_bitmap_region(link, 0, 168, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		al_draw_bitmap_region(link, 0, 200, PLAYERWIDTH, PLAYERHEIGHT - 4, plr.x, plr.y + 32, 0);
		break;

		case 4:
		al_draw_bitmap_region(link, 779, 39, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
		al_draw_bitmap_region(link, 779, 71, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y + 32, 0);
		break;
	}
}

void Player::standingFacingUp(ALLEGRO_BITMAP *link, Player &plr, int &curFrame)
{
	al_draw_bitmap_region(link, 120, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
}
void Player::standingFacingDown(ALLEGRO_BITMAP *link, Player &plr, int &curFrame)
{
	al_draw_bitmap_region(link, 0, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
}
void Player::standingFacingLeft(ALLEGRO_BITMAP *link, Player &plr, int &curFrame)
{
	al_draw_bitmap_region(link, 60, 0, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
}
void Player::standingFacingRight(ALLEGRO_BITMAP *link, Player &plr, int &curFrame)
{
	al_draw_bitmap_region(link, 180, 60, PLAYERWIDTH, PLAYERHEIGHT, plr.x, plr.y, 0);
}

void Player::resolveCollisions(list<Tile>& tList)
{

	list<Tile>::iterator tileIterator = tList.begin();
	while(tileIterator != tList.end())
	{
		Tile tempTile = *tileIterator;

		//FACING RIGHT
		if(this->facing == RIGHT)
		{
			if( this->x + this->bx > tempTile.x &&
				this->x + this->bx < tempTile.x + tempTile.width &&
				this->y > tempTile.y &&
				this->y < tempTile.y + tempTile.height ||
				this->x + this->bx > tempTile.x &&
				this->x + this->bx < tempTile.x + tempTile.width &&
				this->y + this->by > tempTile.y &&
				this->y + this->by <= tempTile.y + tempTile.height)
			{
				this->x = tempTile.x - this->bx;
				this->rbx = this->x + this->bx;
				this->lbx = this->x - this->lbw;
				this->tbx = this->x;
				this->bbx = this->x;
			}
		}

		//FACING LEFT
		if(this->facing == LEFT)
		{
			if(	this->x < tempTile.x + tempTile.width && //top left corner left of tile right
				this->x > tempTile.x && //top left corner right of tile left
				this->y > tempTile.y &&
				this->y < tempTile.y + tempTile.height ||
				this->x < tempTile.x + tempTile.width && //top left corner left of tile right
				this->x > tempTile.x && //bottom left corner right of tile left
				this->y + this->by > tempTile.y &&
				this->y + this->by <= tempTile.y + tempTile.height)
			{
				this->x = tempTile.x + tempTile.width;
				this->rbx = this->x + this->bx;
				this->lbx = this->x - this->lbw;
				this->tbx = this->x;
				this->bbx = this->x;
			}
		}

		
		//FACING DOWN
		if(this->facing == DOWN)
		{
			if( this->y + this->by > tempTile.y &&
				this->y + this->by < tempTile.y + tempTile.height &&
				this->x + this->bx > tempTile.x &&
				this->x < tempTile.x + tempTile.width)
			{
				this->y = tempTile.y - this->by;
				this->rby = this->y;
				this->lby = this->y;
				this->tby = this->y - this->tbh;
				this->bby = this->y + this->bbh;
			}
		}

		//FACING UP
		if(this->facing == UP)
		{
			if( this->y < tempTile.y + tempTile.height &&
				this->y + this->by > tempTile.y &&
				this->x + this->bx > tempTile.x &&
				this->x < tempTile.x + tempTile.width)
			{
				this->y = tempTile.y + tempTile.height;
				this->rby = this->y;
				this->lby = this->y;
				this->tby = this->y - this->tbh;
				this->bby = this->y + this->bbh;
			}
		}
		tileIterator++;
	}
}
void Player::resolveDestructableTileCollisions(list<Tile>& dtList)
{

}

void Player::updateAttackFrameCount(ALLEGRO_SAMPLE *sword)
{
		atk_iterations++;
		if(playedHitSound == false)
		{
			al_play_sample(sword, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
			playedHitSound = true;
		}

		if(++atk_frameCount >= atk_frameDelay)
		{
			if(++atk_curFrame >= atk_maxFrame)
			{
				atk_frameCount = 0;
				atk_curFrame = 0;
				playedHitSound = false;
				hitOnce = true;
				attacking = false;
			}
			atk_frameCount = 0;
		}
}
#endif