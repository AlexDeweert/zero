#ifndef ENEMY_INCL
#define ENEMY_INCL
#include "Global.h"

class Enemy
{
public:
		Enemy(int xStart, int yStart, int width, int height, int speed, int hp, int damage);

		void moveRight();
		void moveLeft();
		void moveUp();
		void moveDown();

		void attackRight();
		void attackLeft();
		void attackUp();
		void attackDown();

		int x;
		int y;
		int width;
		int height;

		int speed;
		int hp;
		int damage;
};
#endif

Enemy::Enemy(int xStart, int yStart, int width, int height, int speed, int hp, int damage)
{
	x = xStart;
	y = yStart;
	this->width = width;
	this->height = height;
	this->speed = speed;
	this->hp = hp;
	this->damage = damage;
}

void Enemy::moveRight()
{
	x += speed;
}

void Enemy::moveLeft()
{
	x -= speed;
}

void Enemy::moveUp()
{
	y -= speed;
}

void Enemy::moveDown()
{
	y += speed;
}

void Enemy::attackRight()
{
	
}
void Enemy::attackLeft()
{
	
}

void Enemy::attackUp()
{
	
}

void Enemy::attackDown()
{
	
}