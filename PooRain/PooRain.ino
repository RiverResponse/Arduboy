#include <Arduboy2.h>
#include <Tinyfont.h>
//#include <cstdint>
//#include <cstdint>

#define BIG_POO_WIDTH 19
#define BIG_POO_HEIGHT 16
#define BIG_POO_STARTLIFE  60

#define SMALL_POO_WIDTH 9
#define SMALL_POO_HEIGHT 15

//#define GROUND  55
#define GROUND  64
#define BIG_MAX_POO  12
#define SMALL_MAX_POO  32

const uint8_t PROGMEM BigPooSpriteSelfMasked[] = {
19, 16,
0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc0, 0xd0, 0xf8, 0xf8, 0xf8, 0xd0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x1c, 0x1d, 0x3f, 0x33, 0x21, 0x25, 0x33, 0x3f, 0x3f, 0x33, 0x21, 0x25, 0x33, 0x1d, 0x1c, 0x00, 0x00,
	};

const uint8_t PROGMEM BigPooSpriteErease[] = {
19, 16,
0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xf0, 0xf8, 0xfc, 0xfe, 0xfc, 0xf8, 0xf0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00,
0x00, 0x1c, 0x3f, 0x3f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x3f, 0x3f, 0x1c, 0x00,
	};

const uint8_t PROGMEM SmallPooErease[] = {
9, 15,
0x00, 0x00, 0xe0, 0xfc, 0xfe, 0xfe, 0xe2, 0x00, 0x00,
0x00, 0x0e, 0x1f, 0x3f, 0x3f, 0x3f, 0x1f, 0x0e, 0x00,
	};

const uint8_t PROGMEM SmallPooSelfMask[] = {
9, 15,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00,
	};

Arduboy2 arduboy;

uint8_t bigPooCount = 0;
uint8_t smallPooCount = 0;


double RandomDouble(double minf, double maxf);
void DrawScore();
void GeneratePoo();

typedef  struct V2
	{
	double X;
	double Y;

	void Init(double x, double y) {
		X = x;
		Y = y;
		}
	} V2;

typedef  struct Poo
	{
	V2 Position;
	int8_t RemLife = 0;
	bool isBig = false;

	void Init() {
		if (isBig) {
			Position.Init(random(0.0, WIDTH - BIG_POO_WIDTH), -BIG_POO_HEIGHT);
			RemLife = BIG_POO_STARTLIFE;
			bigPooCount++;
			}
		else {
			Position.Init(random(0.0, WIDTH - SMALL_POO_WIDTH), -SMALL_POO_HEIGHT);
			RemLife = 1;
			smallPooCount++;
			}
		}

	void Draw() {
		if (RemLife <= 0)
			return;

		if (isBig) {
			arduboy.fillRect(Position.X, Position.Y, BIG_POO_HEIGHT, BIG_POO_WIDTH, BLACK);
			}
		else {
			arduboy.fillRect(Position.X, Position.Y, SMALL_POO_HEIGHT, SMALL_POO_WIDTH, BLACK);
			}
		}

	void DrawSprite() {
		if (RemLife <= 0)
			return;

		if (isBig) {
			Sprites::drawErase(Position.X, Position.Y, BigPooSpriteErease, 0);
			Sprites::drawSelfMasked(Position.X, Position.Y, BigPooSpriteSelfMasked, 0);
			}
		else {
			Sprites::drawErase(Position.X, Position.Y, SmallPooErease, 0);
			Sprites::drawSelfMasked(Position.X, Position.Y, SmallPooSelfMask, 0);
			}
		}

	void Move() {
		if (RemLife <= 0)
			return;

		if (isBig) {

			if (Position.Y < -(BIG_POO_HEIGHT / 2)) {
				Position.Y += 0.1;;
				}
			else if (Position.Y + BIG_POO_HEIGHT < GROUND) {
				Position.Y += 0.5;
				}
			else {
				RemLife--;
				if (RemLife == 0) {
					bigPooCount--;
					}
				}
			}
		else {
			if (Position.Y < -(SMALL_POO_HEIGHT / 2)) {
				Position.Y += 0.1;;
				}
			else if (Position.Y + SMALL_POO_HEIGHT < GROUND) {
				Position.Y += 1;
				}
			else {
				RemLife--;
				if (RemLife == 0)
					smallPooCount--;
				}
			}
		}
	} Poo;

Poo poo;

Poo bigPooPool[BIG_MAX_POO];
Poo smallPooPool[SMALL_MAX_POO];


void setup() {
	arduboy.begin();
	arduboy.initRandomSeed();
	arduboy.setFrameRate(60);

	arduboy.clear();
	arduboy.display();

	for (size_t i = 0; i < BIG_MAX_POO; i++)
		{
		bigPooPool[i].isBig = true;
		}
	}

void loop() {
	if (!arduboy.nextFrame())
		return;
	arduboy.clear();
	arduboy.fillScreen(WHITE);

	GeneratePoo();

	for (size_t i = 0; i < BIG_MAX_POO; i++)
		{
		bigPooPool[i].Move();
		bigPooPool[i].DrawSprite();;
		}

	for (size_t i = 0; i < SMALL_MAX_POO; i++)
		{
		smallPooPool[i].Move();
		smallPooPool[i].DrawSprite();;
		}

	//DrawScore();

	arduboy.display();
	}

void GeneratePoo() {
	if (random(0, 100) < 5) {
		if (smallPooCount < SMALL_MAX_POO) {
			bool found = false;
			for (size_t i = 0; i < SMALL_MAX_POO && !found; i++)
				{
				if (smallPooPool[i].RemLife == 0) {
					smallPooPool[i].Init();
					found = true;
					}
				}
			}
		}
	else if (random(0, 100) < 3) {
		if (bigPooCount < BIG_MAX_POO) {
			bool found = false;
			for (size_t i = 0; i < BIG_MAX_POO && !found; i++)
				{
				if (bigPooPool[i].RemLife == 0) {
					bigPooPool[i].Init();
					found = true;
					}
				}
			}
		}
	}


double RandomDouble(double minf, double maxf)
	{
	return minf + random(1UL << 31) * (maxf - minf) / (1UL << 31);  // use 1ULL<<63 for max double values)
	}

void DrawScore() {
	arduboy.fillRect(0, HEIGHT - 10, WIDTH, 10, BLACK);
	arduboy.drawRect(0, HEIGHT - 10, WIDTH, 10, WHITE);

	Tinyfont tinyfont = Tinyfont(arduboy.sBuffer, Arduboy2::width(), Arduboy2::height());
	tinyfont.setCursor(3, 57);
	tinyfont.print("Score:");
	tinyfont.print("2512");
	tinyfont.setCursor(62, 57);
	tinyfont.print("NEW HIGHSCORE");
	}