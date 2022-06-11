#include "engine.h"
#include "sprite.h"
#include "text.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "sound.h"

// Forward function declarations
void Update(float dt);
void RenderFrame(float dt);  
void EnemyPhase();
void BossPhase();
double getAngle(int startx, int starty, int endx, int endy);
void Tutorial();

#define WW 1920
#define WH 1080


bool bossPhase = false, showTutorial = false, showTutMoving = true, showTutShooting = false, inmainmenu = true, inEndMenu = false;
int Hp = 5, toBoss = 2, bossHp = 10, enemy_speed_multiplier = 1, enemies_killed = 0, score = 0, proj_dmg = 1, move = 0;
int bossDestination = WW / 2, highlightedThrust = rand() % 4, Clock = 0, Clock2 = 0;
const int speedOfProj = WH / 135, player_speed = WW / 240, numOfProjs = 10;
bool enemy1destroyed = false, enemy2destroyed = false, rocketFlying = false, shootLaser = false, explode = false;
int firstFreeProj = 0;
bool projFlying[numOfProjs];


SDL_Rect player = { WW / 2, WH - WH / 11 , WW / 21, WH / 17 };
SDL_Rect enemy1 = { rand() % (WW - WW / 55),  WH / 18 , WW / 42, WH / 34 };
SDL_Rect enemy2 = { rand() % (WW - WW / 55),  WH / 11 , WW / 42, WH / 34 };
SDL_Rect proj[numOfProjs] = { SDL_WINDOWPOS_UNDEFINED, player.x + WH / 180 , WW / 240, WH / 67 };
SDL_Rect boss = { WW / 2, 0, 105 * 4, 48 * 4 };
SDL_Rect heart = { WW / 384, WH / 216, WW / 64, WH / 43 };
SDL_Rect bossThrust = { SDL_WINDOWPOS_UNDEFINED, boss.y + WH / 8, WW / 80, WH / 39 };
SDL_Rect rocketshooter = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WW / 74, WH / 42 };
SDL_Rect lasershooter1 = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WW / 74, WH / 42 };
SDL_Rect lasershooter2 = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WW / 74, WH / 42 };
SDL_Rect laser1 = { lasershooter1.x, lasershooter1.y, WW / 120, WH };
SDL_Rect laser2 = { lasershooter2.x, lasershooter2.y, WW / 120, WH };
SDL_Rect rocket = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WW / 60, WH / 135 * 4 };
SDL_Rect rocketDestination = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0 };
SDL_Rect explosion = rocket;
SDL_Rect tutorialRect = { WW / 2 - tutorialRect.w / 2, 200, 1500, 200 };
SDL_Rect backgroundRect = { 0, -2160, WW, 3 * WH };
SDL_Rect mainmenuRect[4];
SDL_Rect endscreenRect = { WW / 2 - 300, WH / 2 - 300, 600, 200 };
SDL_Rect digit1 = { WW / 2 - 90, WH / 2 - 50, 60, 90 };
SDL_Rect digit2 = { WW / 2 - 30, WH / 2 - 50, 60, 90 };
SDL_Rect digit3 = { WW / 2 + 30, WH / 2 - 50, 60, 90 };
SDL_Point mousePosition = { NULL };

Sprite main_menu_sprite, background, ship_sprite, ship_sprite2, ship_sprite3, enemy_sprite, projectile_sprite, heart_sprite;
Sprite boss_sprite, rocketshooter_sprite, rocket_sprite, lasershooter_sprite, laser_sprite, boss_thrust_sprite, explosion_sprite;
Text tutorial_mv, tutorial_sh, tutorial_ch, MainMenu[4], numbers[10], endscreen;
Sound Crash, PowerUp, Explosion, Charging, Shoot;

//=============================================================================
int main(int argc, char* argv[])
{
	srand(time(NULL));

	tutorialRect = { WW / 2 - tutorialRect.w / 2, 200, 1500, 200 };

	for (int i = numOfProjs - 1; i > 0; i--)
		firstFreeProj = (projFlying[i] == false) ? i : firstFreeProj;

	if (!InitSDL())
	{
		return 1;
	}


	if (!CreateWindow("The Game", WW, WH))
	{
		return 1;
	}

	for (int i = 0; i < numOfProjs; i++)
		projFlying[i] = 0;

	mainmenuRect[0] = { 100, 200, 600, 120 };
	mainmenuRect[1] = { 100, 400, 200, 40 };
	mainmenuRect[2] = { 100, 500, 320, 60 };
	mainmenuRect[3] = { 100, 620, 200, 40 };


	main_menu_sprite = LoadSprite("assets/Background2.png");
	background = LoadSprite("assets/Background.png");
	ship_sprite = LoadSprite("assets/Aly-sprite.png");
	ship_sprite2 = LoadSprite("assets/Aly-sprite2.png");
	ship_sprite3 = LoadSprite("assets/Aly-sprite3.png");
	enemy_sprite = LoadSprite("assets/Enemy-sprite.png");
	projectile_sprite = LoadSprite("assets/Projectile-sprite.png");
	heart_sprite = LoadSprite("assets/Hp-sprite.png");
	boss_sprite = LoadSprite("assets/Boss-sprite.png");
	rocketshooter_sprite = LoadSprite("assets/Rocket_shooter-sprite.png");
	rocket_sprite = LoadSprite("assets/Rocket-sprite.png");
	lasershooter_sprite = LoadSprite("assets/Laser_shooter-sprite.png");
	laser_sprite = LoadSprite("assets/Laser-sprite.png");
	boss_thrust_sprite = LoadSprite("assets/Boss_thrust-sprite.png");
	explosion_sprite = LoadSprite("assets/Explosion-sprite.png");
	tutorial_mv = LoadText("assets/FastHand.ttf", 100, "Y o u  c a n   m o v e   w i t h   W A S D   k e y s   o r   a r r o w s !", { 255,255,255,255 });
	tutorial_sh = LoadText("assets/FastHand.ttf", 100, "Y o u   c a n   s h o o t   w i t h   S P A C E !", { 255,255,255,255 });
	tutorial_ch = LoadText("assets/FastHand.ttf", 100, "Y o u   c a n   c h a r g e   y o u r   s h o t   b y   h o l d i n g   d o w n   S P A C E !", { 255,255,255,255 });
	Crash = LoadSound("assets/crash.wav");
	Explosion = LoadSound("assets/explosion.wav");
	Charging = LoadSound("assets/charging.wav");
	Shoot = LoadSound("assets/shoot.wav");
	MainMenu[0] = LoadText("assets/ethnocentric_rg.ttf", 100, "Space Invaders", { 255,255,255,255 });
	MainMenu[1] = LoadText("assets/ethnocentric_rg.ttf", 100, "Start", { 255,255,255,255 });
	MainMenu[2] = LoadText("assets/ethnocentric_rg.ttf", 100, "Start with tutorial", { 255,255,255,255 });
	MainMenu[3] = LoadText("assets/ethnocentric_rg.ttf", 100, "Quit", { 255,255,255,255 });
	endscreen = LoadText("assets/ethnocentric_rg.ttf", 100, "Game Over! Your score is:", { 255, 0, 0, 255 });
	numbers[0] = LoadText("assets/ethnocentric_rg.ttf", 100, "0", { 255, 0, 0,255 });
	numbers[1] = LoadText("assets/ethnocentric_rg.ttf", 100, "1", { 255, 0, 0,255 });
	numbers[2] = LoadText("assets/ethnocentric_rg.ttf", 100, "2", { 255, 0, 0,255 });
	numbers[3] = LoadText("assets/ethnocentric_rg.ttf", 100, "3", { 255, 0, 0,255 });
	numbers[4] = LoadText("assets/ethnocentric_rg.ttf", 100, "4", { 255, 0, 0,255 });
	numbers[5] = LoadText("assets/ethnocentric_rg.ttf", 100, "5", { 255, 0, 0,255 });
	numbers[6] = LoadText("assets/ethnocentric_rg.ttf", 100, "6", { 255, 0, 0,255 });
	numbers[7] = LoadText("assets/ethnocentric_rg.ttf", 100, "7", { 255, 0, 0,255 });
	numbers[8] = LoadText("assets/ethnocentric_rg.ttf", 100, "8", { 255, 0, 0,255 });
	numbers[9] = LoadText("assets/ethnocentric_rg.ttf", 100, "9", { 255, 0, 0,255 });


	StartLoop(Update, RenderFrame);

	FreeSprite(main_menu_sprite);
	FreeSprite(background);
	FreeSprite(ship_sprite);
	FreeSprite(enemy_sprite);
	FreeSprite(projectile_sprite);
	FreeSprite(heart_sprite);
	FreeSprite(boss_sprite);
	FreeSprite(rocketshooter_sprite);
	FreeSprite(rocket_sprite);
	FreeSprite(lasershooter_sprite);
	FreeSprite(laser_sprite);
	FreeSprite(boss_thrust_sprite);
	FreeText(tutorial_mv);
	FreeText(tutorial_sh);
	FreeText(tutorial_ch);
	CleanUp();
	return 0;
}

//=============================================================================

void Update(float dt)
{
	

	if (IsKeyDown(SDL_SCANCODE_ESCAPE))
		ExitGame();

	if (inEndMenu)
		return;

	else if (inmainmenu)
	{
		if (IsMousePressed(SDL_BUTTON_LMASK))
			GetMousePosition(&mousePosition.x, &mousePosition.y);

		if (SDL_PointInRect(&mousePosition, &mainmenuRect[1]))
		{
			inmainmenu = false;
		}
		else if (SDL_PointInRect(&mousePosition, &mainmenuRect[2]))
		{
			inmainmenu = false; showTutorial = true;
		}
		else if (SDL_PointInRect(&mousePosition, &mainmenuRect[3]))
		{
			ExitGame();
		}


		return;
	}

	SDL_ShowCursor(SDL_FALSE);


	if (move >= 1)
	{
		backgroundRect.y++;
		move = 0;
	}
	else
	{
		move++;
	}
	if (backgroundRect.y >= 0)
		backgroundRect.y = -2160;


	//player movement
	if (IsKeyDown(SDL_SCANCODE_RIGHT) || IsKeyDown(SDL_SCANCODE_D))
	{
		player.x += player_speed; showTutShooting = true;
	}
	if (IsKeyDown(SDL_SCANCODE_LEFT) || IsKeyDown(SDL_SCANCODE_A))
	{
		player.x -= player_speed; showTutShooting = true;
	}
	if (IsKeyDown(SDL_SCANCODE_W) || IsKeyDown(SDL_SCANCODE_UP))
	{
		player.y -= player_speed; showTutShooting = true;
	}
	if (IsKeyDown(SDL_SCANCODE_S) || IsKeyDown(SDL_SCANCODE_DOWN))
	{
		player.y += player_speed; showTutShooting = true;
	}


	player.x = (player.x <= 0) ? 0 : player.x;
	player.x = (player.x >= WW - player.w) ? WW - player.w : player.x;
	player.y = (player.y <= WH - WH / 2) ? WH - WH / 2 : player.y;
	player.y = (player.y >= WH - WH / 10) ? WH - WH / 10 : player.y;


	//shooting
	if (IsKeyPressed(SDL_SCANCODE_SPACE))
	{
		projFlying[firstFreeProj] = true;

		PlaySound(Shoot);

		for (int i = numOfProjs - 1; i > 0; i--)
			firstFreeProj = (!projFlying[i]) ? i : firstFreeProj;
		showTutorial = false;
	}



	//projectile movement
	for (int i = 0; i < numOfProjs; i++)
	{
		proj[i].x = (projFlying[i]) ? proj[i].x : player.x + (player.w - proj[i].w) / 2;
		proj[i].y -= (projFlying[i]) ? speedOfProj : player.y + (player.h - 2 * proj[i].h) / 2;
	}


	//tutorial
	if (!showTutorial)
	{
		if (bossPhase)
			BossPhase();

		else
			EnemyPhase();
	}


	//projectile reset
	for (int i = 0; i < numOfProjs; i++)
	{
		if (proj[i].y <= -15 || (proj[i].y + WH / 20 < enemy1.y && proj[i].y + WH / 20 < enemy2.y))
		{
			proj[i].y = player.y + WH / 180;
			proj[i].w = WW / 240; proj[i].h = WH / 67;
			projFlying[i] = false;
		}
	}


	if (Hp <= 0)
		inEndMenu = true;
}

void RenderFrame(float interpolation)
{
	// Clear screen
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);

	SDL_RenderCopy(gRenderer, background.texture, 0, &backgroundRect);

	if (inmainmenu)
	{
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, background.texture, 0, &backgroundRect);
		SDL_RenderCopy(gRenderer, MainMenu[0].text_texture, 0, &mainmenuRect[0]);
		SDL_RenderCopy(gRenderer, MainMenu[1].text_texture, 0, &mainmenuRect[1]);
		SDL_RenderCopy(gRenderer, MainMenu[2].text_texture, 0, &mainmenuRect[2]);
		SDL_RenderCopy(gRenderer, MainMenu[3].text_texture, 0, &mainmenuRect[3]);
		return;
	}

	else if (inEndMenu)
	{
		unsigned int temp = score;
		unsigned int hundreds = temp / 100; temp %= 100;
		unsigned int tens = temp / 10; temp %= 10;
		unsigned int units = temp;

		SDL_RenderCopy(gRenderer, endscreen.text_texture, 0, &endscreenRect);

		SDL_RenderCopy(gRenderer, numbers[hundreds].text_texture, 0, &digit1);
		SDL_RenderCopy(gRenderer, numbers[tens].text_texture, 0, &digit2);
		SDL_RenderCopy(gRenderer, numbers[units].text_texture, 0, &digit3);

		return;
	}

	heart.x = 0;
	for (int i = 0; i < Hp; i++)
	{
		SDL_RenderCopy(gRenderer, heart_sprite.texture, 0, &heart);
		heart.x += WW / 55;
	}

	SDL_RenderCopy(gRenderer, ship_sprite.texture, 0, &player);

	//players projectile
	for (int i = 0; i < numOfProjs; i++)
	{
		if (projFlying[i])
			SDL_RenderCopy(gRenderer, projectile_sprite.texture, 0, &proj[i]);
	}
		

	if (showTutorial)
		Tutorial();

	else if (bossPhase)
	{
		//boss
		SDL_RenderCopy(gRenderer, boss_sprite.texture, 0, &boss);

		//highlighted thrust
		SDL_RenderCopy(gRenderer, boss_thrust_sprite.texture, 0, &bossThrust);

		//rocketshooter
		SDL_RenderCopy(gRenderer, rocketshooter_sprite.texture, 0, &rocketshooter);

		//lasershooters
		SDL_RenderCopy(gRenderer, lasershooter_sprite.texture, 0, &lasershooter1);
		SDL_RenderCopy(gRenderer, lasershooter_sprite.texture, 0, &lasershooter2);

		//rocket
		SDL_RenderCopy(gRenderer, rocket_sprite.texture, &backgroundRect, &rocket);

		//lasers
		SDL_RenderCopy(gRenderer, laser_sprite.texture, 0, &laser1);
		SDL_RenderCopy(gRenderer, laser_sprite.texture, 0, &laser2);

		//explotion
		SDL_RenderCopy(gRenderer, explosion_sprite.texture, 0, &explosion);
	}
	else
	{
		//enemy1 ship
		SDL_RenderCopy(gRenderer, enemy_sprite.texture, 0, &enemy1);

		//enemy2 ship
		SDL_RenderCopy(gRenderer, enemy_sprite.texture, 0, &enemy2);
	}
}


void BossPhase()
{
	Clock2++;
	explode = true;
	if (Clock2 >= 120)
	{
		explode = false;
		Clock2 = 0;
	}

	//boss movement
	if (boss.x + boss.w < bossDestination)
	{
		boss.x += enemy_speed_multiplier;
	}
	else if (boss.x> bossDestination)
	{
		boss.x -= enemy_speed_multiplier;
	}
	else
		bossDestination = rand() % (WW - boss.w);
	if (boss.y < WH / 27)
		boss.y++, bossThrust.y++;

	//projectile reset
	for (int i = 0; i < numOfProjs; i++)
	{
		if (proj[i].y <= boss.y + boss.h / 2)
		{
			proj[i].y = player.y + WH / 180;
			proj[i].w = WW / 240; proj[i].h = WH / 67;
			projFlying[i] = false;
		}
	}
	


	//damaging boss
	switch (highlightedThrust)
	{
	case 0: bossThrust.x = boss.x + 76; break;
	case 1: bossThrust.x = boss.x + 156; break;
	case 2: bossThrust.x = boss.x + 244; break;
	case 3: bossThrust.x = boss.x + 320; break;
	default: break;
	}

	for (int i = 0; i < numOfProjs; i++)
	{
		if (SDL_HasIntersection(&proj[i], &bossThrust))
		{
			highlightedThrust = rand() % 4;
			bossHp--;

			PlaySound(Crash);
		}
	}
	

	//boss attacks

	//only rocket attack
	if (bossHp <= 8)
	{
		rocketshooter.x = boss.x + boss.w / 2 - rocketshooter.w / 2;
		rocketshooter.y = boss.y + boss.h * 2 / 3 + rocketshooter.h / 2;

		if (rocketFlying)
		{
			if (rocket.x >= rocketDestination.x - 5 && rocket.x <= rocketDestination.x + 5 && rocket.y >= rocketDestination.y - 5 && rocket.y <= rocketDestination.y + 5)
			{
				explosion = rocket;
				rocketFlying = false;

				PlaySound(Explosion);
			}

			if (rocket.x <= rocketDestination.x)
			{
				if (rocket.y <= rocketDestination.y)
				{
					rocket.x += 2; rocket.y += 2;
				}
				else if (rocket.y > rocketDestination.y)
				{
					rocket.x += 2; rocket.y -= 2;
				}
			}

			else if (rocket.x > rocketDestination.x)
			{
				if (rocket.y < rocketDestination.y)
				{
					rocket.x -= 2; rocket.y += 2;
				}
				else if (rocket.y > rocketDestination.y)
				{
					rocket.x -= 2; rocket.y -= 2;
				}
			}
		}
		else
		{
			rocket.x = rocketshooter.x + rocketshooter.w / 5;
			rocket.y = rocketshooter.y + rocketshooter.h;

			rocketDestination.x = player.x + player.w / 2;
			rocketDestination.y = player.y + player.h / 2;

			rocketFlying = true;
		}

		//explosion of rocket
		if (SDL_HasIntersection(&player, &rocket))
		{
			explosion = rocket;
			rocketFlying = false;
		}

		if (!explode)
		{
			explosion.x = -999;
			explosion.y = -999;
		}
	}

	//laser attack as well
	if (bossHp <= 4)
	{
		Clock++;
		if (Clock >= 300)
			shootLaser = true;
		if (Clock >= 600)
		{
			shootLaser = false;
			Clock = 0;
		}

		lasershooter1.x = boss.x + lasershooter1.w * 15 / 8;
		lasershooter1.y = boss.y + boss.h * 2 / 3 + lasershooter1.h / 2;

		lasershooter2.x = boss.x + boss.w - lasershooter1.w * 5 / 2;
		lasershooter2.y = boss.y + boss.h * 2 / 3 + lasershooter2.h / 2;


		if (shootLaser)
		{
			laser1.x = lasershooter1.x + lasershooter1.w / 5;
			laser1.y = lasershooter1.y + lasershooter1.h;

			laser2.x = lasershooter2.x + lasershooter2.w / 5;
			laser2.y = lasershooter2.y + lasershooter2.h;
		}
		else
		{
			laser1.x = -999;
			laser1.y = -999;

			laser2.x = -999;
			laser2.y = -999;
		}
	}



	//damaging player
	if (SDL_HasIntersection(&player, &laser1) || SDL_HasIntersection(&player, &laser2))
	{
		Hp--;
		Clock = 0;
		shootLaser = false;
		laser1.x = -999;
		laser1.y = -999;

		laser2.x = -999;
		laser2.y = -999;

		PlaySound(Crash);
	}

	if (SDL_HasIntersection(&player, &explosion))
	{
		Hp--;
		explode = false;

		explosion.x = -999;
		explosion.y = -999;

		PlaySound(Crash);
	}

	//end of boss phase
	if (bossHp <= 0)
	{
		bossPhase = false;
		score += 15;
	}
}


void EnemyPhase()
{
	//speed up after every 30 kills(points)
	if (enemies_killed == 30)
	{
		enemy_speed_multiplier++;
		enemies_killed = 0;
		toBoss--;
	}

	if (toBoss == 0)
	{
		bossPhase = true;
	}

	//colision
	for (int i = 0; i < numOfProjs; i++)
	{
		if (SDL_HasIntersection(&proj[i], &enemy1))
		{
			enemy1destroyed = true;
			proj[i].y = player.y + WH / 180;
			proj[i].w = WW / 240; proj[i].h = WH / 67;
			projFlying[i] = false;
			enemies_killed++;
			score++;

			PlaySound(Crash);
		}
		if (SDL_HasIntersection(&proj[i], &enemy2))
		{
			enemy2destroyed = true;
			proj[i].y = player.y + WH / 180;
			proj[i].w = WW / 240; proj[i].h = WH / 67;
			projFlying[i] = false;
			enemies_killed++;
			score++;

			PlaySound(Crash);
		}
	}
	

	enemy1.y += enemy_speed_multiplier;
	enemy2.y += enemy_speed_multiplier;

	if (enemy1.y >= player.y - enemy1.h)
	{
		Hp--;
		enemy1destroyed = true;

		PlaySound(Crash);
	}
	else if (enemy2.y >= player.y - enemy2.h)
	{
		Hp--;
		enemy2destroyed = true;

		PlaySound(Crash);
	}


	if (enemy1destroyed)
	{
		enemy1.x = rand() % (WW - enemy1.w);
		enemy1.y = WH / 11;
		enemy1destroyed = false;
	}
	if (enemy2destroyed)
	{
		enemy2.x = rand() % (WW - enemy2.w);
		enemy2.y = WH / 11;
		enemy2destroyed = false;
	}
}



//Jak rotovat objekt?????????
double getAngle(int startx, int starty, int endx, int endy)
{
	int x = endx - startx; int y = endy - starty;
	double angle = 0;
	if (y != 0)
		angle = atan(x / y) * 180 / M_PI;
	return angle;
}


void Tutorial()
{
	if (showTutShooting)
	{
		SDL_RenderCopy(gRenderer, tutorial_sh.text_texture, 0, &tutorialRect);
	}
	else if (showTutMoving)
	{
		SDL_RenderCopy(gRenderer, tutorial_mv.text_texture, 0, &tutorialRect);
	}
}