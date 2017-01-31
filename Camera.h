#include <allegro5\allegro5.h>
#include "Global.h"

class Camera
{
public:
	Camera();
	~Camera();

	void init();
	void update(int x, int y);
	void draw(BITMAP *buffer);
	void setPosition(int x, int y);

private:
	int cameraX;
	int cameraY;
};

Camera::Camera()
{

}

Camera::~Camera()
{
	
}

void Camera::init()
{
	cameraX = 0;
	cameraY = 0;
}

void Camera::update(int x, int y)
{
	setPosition(x, y);
}

void Camera::draw(BITMAP *buffer)
{

}

void Camera::setPosition(int x, int y)
{
	cameraX = x - SCREENWIDTH / 2;
	cameraY = y - SCREENHEIGHT / 2;

	if(cameraX < 0)
	{
		cameraX = 0;
	}
	
	if(cameraY < 0)
	{
		cameraY = 0;
	}
}