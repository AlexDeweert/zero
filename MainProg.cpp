#include "Player.h"
#include "Global.h"
#include "Level0.h"
#include "Level1.h"
#include "Enemy.h"

char tileArray[TILE_W][TILE_H];
list<Tile> tileList;
list<Tile> destructableTileList;

//PROTOTYPES
void populateTileList(int& currentLevel, int level[][32]);
void populateDestructableTileList(int& currentLevel, int level[][32]);
void drawEnvironment(int currentLevel, int level[][32], ALLEGRO_BITMAP *env);
void cameraUpdate(float *cameraPosition, float x, float y, int width, int height, Player &plr);
void updateMovementFrameCount(int &curFrame, const int &maxFrame, int &frameCount, int &frameDelay);

int main(void)
{
	bool transitioning = false;

	//moving animations 7 frames
	const int maxFrame = 2;
	int curFrame = 0;
	int frameCount = 0;
	int frameDelay = 6;

	//camera zoom scale
	float scale = 1.0;
	
	//starting location
	int currentLevel = 0;
	populateTileList(currentLevel, level0);
	Player &plr = Player(934, 304, PLAYERWIDTH, PLAYERHEIGHT);
	Enemy &enemy1 = Enemy(SCREENWIDTH/2 + 20, SCREENHEIGHT/2 + 20, 50, 50, 3, 20, 1);
	
	bool done = false;
	bool redraw = true;
	bool stopped = true;
	int FPS = 60;

	bool keys[6] = {false, false, false, false, false, false};
	bool direction[4] = {false, false, false, false};
	int wasFacing = 0;

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_TRANSFORM camera;

	//SOUNDS
	ALLEGRO_SAMPLE *overworld = NULL;
	ALLEGRO_SAMPLE *sword = NULL;

	ALLEGRO_BITMAP *link;
	ALLEGRO_BITMAP *environment;
	ALLEGRO_BITMAP *env;
	ALLEGRO_BITMAP *ui;

	//CAMERA
	float cameraPosition[2] = {currentLevel*SCREENWIDTH,0};

	if(!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(SCREENWIDTH, SCREENHEIGHT);			//create our display object

	if(!display)										//test display object
		return -1;

	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	al_init_image_addon();

	link = al_load_bitmap("legendofzelda_link_sheet.png");
	ui = al_load_bitmap("ui.png");

	environment = al_load_bitmap("ps2-town-tiles.png");
	env = al_load_bitmap("environment.png");
	


	ALLEGRO_FONT *font24 = al_load_font("arial.ttf",20,0);


	al_reserve_samples(2);

	//SOUNDS - Load
	overworld = al_load_sample("Overworld_Theme.ogg");
	sword = al_load_sample("LOZ_Sword.wav");
	
	if(currentLevel == 0)
	al_play_sample(overworld, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_start_timer(timer);
	while(!done)
	{
		char xPtr[32];
		sprintf_s(xPtr, "%.0f", plr.x);
		
		char yPtr[32];
		sprintf_s(yPtr, "%.0f", plr.y);

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_W:
					keys[UP] = true;
					break;
				case ALLEGRO_KEY_S:
					keys[DOWN] = true;
					break;
				case ALLEGRO_KEY_D:
					keys[RIGHT] = true;
					break;
				case ALLEGRO_KEY_A:
					keys[LEFT] = true;
					break;
				case ALLEGRO_KEY_SPACE:
					keys[SPACE] = true;
					break;
				case ALLEGRO_KEY_N:
					keys[N] = true;
					break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_W:
					keys[UP] = false;
					break;
				case ALLEGRO_KEY_S:
					keys[DOWN] = false;
					break;
				case ALLEGRO_KEY_D:
					keys[RIGHT] = false;
					break;
				case ALLEGRO_KEY_A:
					keys[LEFT] = false;
					break;
				case ALLEGRO_KEY_SPACE:
					keys[SPACE] = false;
					break;
				case ALLEGRO_KEY_N:
					keys[N] = false;
					break;
				case ALLEGRO_KEY_ESCAPE:
					done = true;
					break;
			}
		}

		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		//ACTIONS
		else if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			//Check Player Location - Change the collision tile list<>
			if(currentLevel != 0 && plr.x > 0 && plr.x + plr.bx < SCREENWIDTH)
			{
				currentLevel = 0;
				tileList.clear();
				populateTileList(currentLevel, level0);
			}

			if(currentLevel != 1 && plr.x > SCREENWIDTH && plr.x + plr.bx < SCREENWIDTH*2)
			{
				currentLevel = 1;
				tileList.clear();
				populateTileList(currentLevel, level1);
			}

			//Camera Update
			cameraUpdate(cameraPosition, plr.x, plr.y, plr.bx, plr.by, plr);
			al_identity_transform(&camera);
			al_translate_transform(&camera, -cameraPosition[0], -cameraPosition[1]);
			al_scale_transform(&camera, scale, scale);
			al_use_transform(&camera);

			int i = int(cameraPosition[0]);
			if(i % SCREENWIDTH == 0) //If camera position is zero or a multiple of SCREENWIDTH
				transitioning = false; //then there is no screen transition
			else
				transitioning = true; //Otherwise transitioning is true

			if(transitioning == true) //while screen moving make link run in place
				updateMovementFrameCount(curFrame, maxFrame, frameCount, frameDelay);
			
			//If camera is not moving do everything else
			if(transitioning == false)
			{
				//ATTACKS
				plr.attackRight(keys, sword);
				plr.attackLeft(keys, sword);
				plr.attackDown(keys, sword);
				plr.attackUp(keys, sword);
			
				//MOVING LEFT
				if(keys[LEFT] && !keys[UP] && !keys[DOWN] && !keys[RIGHT] && !plr.attacking)
				{
					plr.moving = true;
					updateMovementFrameCount(curFrame, maxFrame, frameCount, frameDelay);
					direction[NORTH] = false;
					direction[SOUTH] = false;
					direction[EAST] = false;
					direction[WEST] = true;
					plr.goLeft(tileList, destructableTileList);
				}
			
				//MOVING RIGHT
				if(keys[RIGHT] && !keys[UP] && !keys[DOWN] && !keys[LEFT] && !plr.attacking)
				{
					plr.moving = true;
					updateMovementFrameCount(curFrame, maxFrame, frameCount, frameDelay);
					direction[NORTH] = false;
					direction[SOUTH] = false;
					direction[EAST] = true;
					direction[WEST] = false;
					plr.goRight(tileList, destructableTileList);
				}
				
				//MOVING UP
				if(keys[UP] && !keys[RIGHT] && !keys[DOWN] && !keys[LEFT] && !plr.attacking)
				{
					plr.moving = true;
					updateMovementFrameCount(curFrame, maxFrame, frameCount, frameDelay);
					direction[NORTH] = true;
					direction[SOUTH] = false;
					direction[EAST] = false;
					direction[WEST] = false;
					plr.goUp(tileList, destructableTileList);
				}

				//MOVING DOWN
				if(keys[DOWN] && !keys[UP] && !keys[RIGHT] && !keys[LEFT] && !plr.attacking)
				{
					plr.moving = true;
					updateMovementFrameCount(curFrame, maxFrame, frameCount, frameDelay);
					direction[NORTH] = false;
					direction[SOUTH] = true;
					direction[EAST] = false;
					direction[WEST] = false;
					plr.goDown(tileList, destructableTileList);
				}

				if(!keys[DOWN] && !keys[UP] && !keys[RIGHT] && !keys[LEFT])
				{
					plr.moving = false;
					updateMovementFrameCount(curFrame, maxFrame, frameCount, frameDelay);
				}
			}//end if transitioning
				redraw = true;
			
		}
			

		//DRAW HERE
		if(redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
			


			al_draw_text(font24, al_map_rgb(255, 0, 255), 20, 15, 0, "x: ");
			al_draw_text(font24, al_map_rgb(255, 0, 255), 20 + 20, 15, 0, xPtr);

			al_draw_text(font24, al_map_rgb(255, 0, 255), 20, 35, 0, "y: ");
			al_draw_text(font24, al_map_rgb(255, 0, 255), 20 + 20, 35, 0, yPtr);


				

			//ENVIRONMENT DRAW LEVEL
			switch(currentLevel)
			{
				case 0:
					if(!transitioning) //only draw current region if screen isn't moving between levels
					{
						al_draw_bitmap_region(ui, 32, 36, 856, 128, 84 + cameraPosition[0], 16, 0); 
						drawEnvironment(0, level0, env);
					}
					else //otherwise draw up, down, left, and right of current level
					{
						al_draw_bitmap_region(ui, 32, 36, 856, 128, 84 + cameraPosition[0], 16, 0); 
						drawEnvironment(0, level0, env);
						drawEnvironment(1, level1, env);
					}
					
					break;
				case 1:
					if(!transitioning)
					{
						al_draw_bitmap_region(ui, 32, 36, 856, 128, 84 + cameraPosition[0] * currentLevel, 16, 0); 
						drawEnvironment(1, level1, env);
					}
					else
					{
						al_draw_bitmap_region(ui, 32, 36, 856, 128, 84 + cameraPosition[0] * currentLevel, 16, 0); 
						drawEnvironment(0, level0, env);	
						drawEnvironment(1, level1, env);
					}
					break;
			}



			//Attack Right Draw
			if(plr.facing == RIGHT && plr.attacking)
				plr.attackRightAnimation(link, plr);

			//Attack Left Draw
			if(plr.facing == LEFT && plr.attacking)
				plr.attackLeftAnimation(link, plr);

			//Attack Up Draw
			if(plr.facing == UP && plr.attacking)
				plr.attackUpAnimation(link, plr);

			//Attack Down Draw
			if(plr.facing == DOWN && plr.attacking)
				plr.attackDownAnimation(link, plr);

			//Move Right Draw
			if(plr.facing == RIGHT && plr.moving == true && plr.attacking == false)
				plr.rightAnimation(link, plr, curFrame);

			//Move Left Draw
			if(plr.facing == LEFT && plr.moving == true && plr.attacking == false)
				plr.leftAnimation(link, plr, curFrame);
			
			//Move Up Draw
			if(plr.facing == UP && plr.moving == true && plr.attacking == false)
				plr.upAnimation(link, plr, curFrame);
				
			//Move Down Draw
			if(plr.facing == DOWN && plr.moving == true && plr.attacking == false)
				plr.downAnimation(link, plr, curFrame);

			//Standing Facing Down Draw
			if(plr.facing == DOWN && plr.moving == false && plr.attacking == false)
				plr.standingFacingDown(link, plr, curFrame);

			//Standing Facing Up Draw
			if(plr.facing == UP && plr.moving == false && plr.attacking == false)
				plr.standingFacingUp(link, plr, curFrame);				

			//Standing Facing Right Draw
			if(plr.facing == RIGHT && plr.moving == false && plr.attacking == false && transitioning == false)
				plr.standingFacingRight(link, plr, curFrame);

			//Standing Facing Left Draw
			if(plr.facing == LEFT && plr.moving == false && plr.attacking == false && transitioning == false)
				plr.standingFacingLeft(link, plr,curFrame);	
			
			if(plr.facing == LEFT && transitioning == true)
				plr.leftAnimation(link, plr, curFrame);

			if(plr.facing == RIGHT && transitioning == true)
				plr.rightAnimation(link, plr, curFrame);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));

		}
	}

	al_destroy_bitmap(ui);
	al_destroy_bitmap(link);
	al_destroy_bitmap(environment);
	al_destroy_font(font24);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);


	return 0;
}

void drawEnvironment(int currentLevel, int level[][32], ALLEGRO_BITMAP *env)
{
	for(int height = 0; height < TILE_H; height++)
	{
		for(int width = 0; width < TILE_W; width++)
		{
			int tileValue = level[height][width];
			switch(tileValue)
			{
			case 0: //ground
				al_draw_bitmap_region(env, 68, 102, 32, 32, width*32 + SCREENWIDTH*currentLevel, height*32 + UI_OFFSET, 0);
				break;
			case 1: //water
				al_draw_bitmap_region(env, 68, 68, 32, 32, width*32 + SCREENWIDTH*currentLevel, height*32 + UI_OFFSET, 0);
				break;
			case 2: //RED ROCK walls (not corners) collidable
				al_draw_bitmap_region(env, 0, 0, 32, 32, width*32 + SCREENWIDTH*currentLevel, height*32 + UI_OFFSET, 0);
				break;
			case 3: //trees
				al_draw_bitmap_region(env, 68, 0, 32, 32, width*32 + SCREENWIDTH*currentLevel, height*32 + UI_OFFSET, 0);
				break;
			case 4: //green statues 34 102
				al_draw_bitmap_region(env, 34, 102, 32, 32, width*32 + SCREENWIDTH*currentLevel, height*32 + UI_OFFSET, 0);
				break;
			case 5: //RED ROCK top left outside corner
				al_draw_bitmap_region(env, 0, 68, 32, 32, width*32 + SCREENWIDTH*currentLevel, height*32 + UI_OFFSET, 0);
				break;
			case 6: //RED ROCK top right inside corner
				al_draw_bitmap_region(env, 0, 102, 32, 32, width*32 + SCREENWIDTH*currentLevel, height*32 + UI_OFFSET, 0);
				break;
			case 7: //RED ROCK bottom left inside corner
				al_draw_bitmap_region(env, 0, 102, 32, 32, width*32 + SCREENWIDTH*currentLevel, height*32 + UI_OFFSET, 0);
				break;
			case 8: //RED ROCK bottom right inside corner
				al_draw_bitmap_region(env, 0, 102, 32, 32, width*32 + SCREENWIDTH*currentLevel, height*32 + UI_OFFSET, 0);
				break;
			case 9: //RED ROCK walls (not corners) non-collidable
				al_draw_bitmap_region(env, 0, 0, 32, 32, width*32 + SCREENWIDTH*currentLevel, height*32 + UI_OFFSET, 0);
				break;
			}
		}
	}
}

void populateTileList(int &currentLevel, int level[][32])
{
	for(int h = 0; h < TILE_H; h++)
		{
			for(int w = 0; w < TILE_W; w++)
			{
				if(level[h][w] == 1) //check water tiles
				{
					Tile tempTile(w*32 + currentLevel*SCREENWIDTH, h*32 + UI_OFFSET, 32, 32, 1, false);
					tileList.push_back(tempTile);
				}

				if(level[h][w] == 2) //check red collidable rock
				{
					Tile tempTile(w*32 + currentLevel*SCREENWIDTH, h*32 + UI_OFFSET, 32, 32, 2, false);
					tileList.push_back(tempTile);
				}

							
				if(level[h][w] == 3) //check small bushes
				{
					Tile tempTile(w*32 + currentLevel*SCREENWIDTH, h*32 + UI_OFFSET, 32, 32, 3, false);
					tileList.push_back(tempTile);
				}

				if(level[h][w] == 4) //check green statues
				{
					Tile tempTile(w*32 + currentLevel*SCREENWIDTH, h*32 + UI_OFFSET, 32, 32, 4, false);
					tileList.push_back(tempTile);
				}
			}
		}	
}

void populateDestructableTileList(int& currentLevel, int level[][32])
{
	for(int h = 0; h < TILE_H; h++)
	{
		for(int w = 0; w < TILE_W; w++)
		{		
			if(level[h][w] == 3) //check small bushes
			{
				Tile tempTile(w*32 + currentLevel*SCREENWIDTH, h*32 + UI_OFFSET, 32, 32, 3, false);
				destructableTileList.push_back(tempTile);
			}
		}
	}
}

void cameraUpdate(float *cameraPosition, float x, float y, int width, int height, Player &plr)
{
	//If player within level 0 and camera position is greater than 0 (all the way to the left), move camera left
	if(plr.x + plr.bx < SCREENWIDTH && cameraPosition[0] > 0)
	{
		cameraPosition[0] -= 16;
	}
	
	//If player within level 1 and camera position is less than SCREENWIDTH, move camera right
	if(plr.x > SCREENWIDTH && cameraPosition[0] < SCREENWIDTH)
	{
		cameraPosition[0] += 16;
	}
}

void updateMovementFrameCount(int &curFrame, const int &maxFrame, int &frameCount, int &frameDelay)
{
	if(++frameCount >= frameDelay)
	{
		if(++curFrame >= maxFrame)
			curFrame = 0;

		frameCount = 0;
	}
}