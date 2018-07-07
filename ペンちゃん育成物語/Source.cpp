#include <fstream>
#include <string>
#include <vector>
#include "XLibrary11.hpp"
using namespace std;
using namespace XLibrary11;

enum Mode
{
	Title,
	Help,
	Help2,
	Help3,
	Help4,
	Interval1,
	Interval2,
	Interval3,
	Interval4,
	Start,
	Start2,
	Start3,
	Start4,
	Game1,
	Game2,
	Game3,
	Game4,
	Goal1,
	Goal2,
	Goal3,
	Goal4,
	Clear1,
	Clear2,
	Clear3,
	Miss,
	Miss2,
	Miss3,
	Miss4,
	Over,
	Over2,
	Over3,
	Over4,
	Result,
	Staff,
};

Sound decisionSound(L"decision.mp3");
Sound jumpSound(L"jump.wav");
Sound hitSound(L"hit.wav");
Sound pointSound(L"point.wav");
Sound startSound(L"start.mp3");
Sound goalSound(L"finish.mp3");
Sound clearSound(L"clear.mp3");
Sound missSound(L"miss.mp3");
Sound overSound(L"over.mp3");

vector<char> map;
int width = 0;
int height = 0;
int stage1score = 0;
int stage2score = 0;
int stage3score = 0;
int bonusscore = 0;
int totalscore = 0;
int highscore = 0;
int counter = 0;
int continueCount = 0;
int playerLife = 0;
int animCount = 0;
bool isGround = false;

struct Character
{
	Float2 position = 0.0f;
	Float2 velocity = 0.0f;
	Float2 scale = 0.4f;
	int counter = 0;
	bool isGround = false;
};

Mode mode = Title;

void UpdateCharacter(Character & character, int width, int height, vector<char>& map)
{
	character.velocity.y -= 0.5f;

	character.isGround = false;

	if (character.velocity.y < -15.0f)
	{
		character.velocity.y = -15.0f;
	}

	int centerX = (int)roundf(character.position.x / 32.0f);
	int centerY = height - (int)roundf(character.position.y / 32.0f);

	for (int x = centerX - 15; x <= centerX + 15; x++)
	{
		for (int y = centerY - 20; y <= centerY + 20; y++)
		{
			if (x < 0 || y < 0 || x >= width || y >= height)
			{
				continue;
			}

			Float2 now = character.position;
			Float2 next = character.position + character.velocity;
		
			Float2 position = Float2(x * 32.0f, (height - y) *  16.0f);

			if (map[x + y * width] == 't')
			{
				if (next.x > position.x - 380 &&
					next.x < position.x + 380 &&
					now.y > position.y - 59 &&
					now.y < position.y + 57)
				{
					character.velocity.y = 0.0f;
					character.velocity.y += 0.1f;
				}
				if (now.x > position.x - 380 &&
					now.x < position.x + 380 &&
					next.y > position.y - 59 &&
					next.y < position.y + 58)
				{
					if (character.velocity.y < 0.0f)
					{
						character.position.y = position.y + 58;
						character.velocity.y = 0.0f;
						character.velocity.y -= 0.2f;
					}
					if (character.velocity.y > 0.0f)
					{
						character.position.y = position.y - 60;
						character.velocity.y = 0.0f;
					}
				}
			}

			if (map[x + y * width] == '0')
			{
				if (now.x > position.x - 16 &&
					now.x < position.x + 48 &&
					now.y > position.y - 64 &&
					now.y < position.y + 64)
				{
					map[x + y * width] = '.';
					stage2score += 20;
					pointSound.Play();
				}
			}

			if (map[x + y * width] == 'O')
			{
				if (now.x > position.x - 32 &&
					now.x < position.x + 48 &&
					now.y > position.y + 12 &&
					now.y < position.y + 132)
				{
					map[x + y * width] = '.';
					bonusscore += 30;
					pointSound.Play();
				}
			}

			if (map[x + y * width] == '<')
			{
				if (now.x > position.x - 68 &&
					now.x < position.x + 76 &&
					now.y > position.y - 36 &&
					now.y < position.y + 36)
				{
					character.velocity.y = 0.0f;
					counter = 0;
					hitSound.Play();
					mode = Miss2;
				}
			}

			if (map[x + y * width] == '#')
			{
				if (next.x > position.x - 380 &&
					next.x < position.x + 380 &&
					now.y > position.y - 98 &&
					now.y < position.y + 22)
				{
					character.velocity.y = 0.0f;
					counter = 0;
					hitSound.Play();
					mode = Miss3;
				}
				if (now.x > position.x - 380 &&
					now.x < position.x + 380 &&
					next.y > position.y - 98 &&
					next.y < position.y + 22)
				{
					if (character.velocity.y < 0.0f)
					{
						character.position.y = position.y + 22;
					}
					character.velocity.y = 0.0f;
					character.isGround = true;
				}
			}
			
			if (map[x + y * width] == 's')
			{
				if (next.x > position.x - 400 &&
					next.x < position.x + 400 &&
					now.y > position.y - 176 &&
					now.y < position.y + 10)
				{
					character.velocity.y = 0.0f;
					counter = 0;
					hitSound.Play();
					mode = Miss3;
				}
				if (now.x > position.x - 400 &&
					now.x < position.x + 400 &&
					next.y > position.y - 176 &&
					next.y < position.y + 10)
				{
					if (character.velocity.y < 0.0f)
					{
						character.position.y = position.y + 10;
					}
					character.velocity.y = 0.0f;
					character.isGround = true;
				}
			}

			if (map[x + y * width] == '1')
			{
				if (now.x > position.x - 108 &&
					now.x < position.x + 108 &&
					now.y > position.y - 76 &&
					now.y < position.y + 76)
				{
					counter = 0;
					hitSound.Play();
					mode = Miss;
				}
			}

			if (map[x + y * width] == '2')
			{
				if (now.x > position.x - 108 &&
					now.x < position.x + 108 &&
					now.y > position.y - 76 &&
					now.y < position.y + 76)
				{
					counter = 0;
					hitSound.Play();
					mode = Miss;
				}
			}

			if (map[x + y * width] == '3')
			{
				if (next.x > position.x - 108 &&
					next.x < position.x + 108 &&
					now.y > position.y - 94 &&
					now.y < position.y + 94)
				{	
					character.velocity.y = 0.0f;
					counter = 0;
					hitSound.Play();
					mode = Miss3;	
				}
				if (now.x > position.x - 108 &&
					now.x < position.x + 108 &&
					next.y > position.y - 94 &&
					next.y < position.y + 94)
				{
					if (character.velocity.y < 0.0f)
					{
						character.position.y = position.y + 94;
					}
					character.velocity.y = 0.0f;
					character.isGround = true;
				}
			}

			if (map[x + y * width] == '4')
			{
				if (next.x > position.x - 156 &&
					next.x < position.x + 156 &&
					now.y > position.y - 126 &&
					now.y < position.y + 126)
				{
					character.velocity.y = 0.0f;
					counter = 0;
					hitSound.Play();
					mode = Miss3;
				}
				if (now.x > position.x - 156 &&
					now.x < position.x + 156 &&
					next.y > position.y - 126 &&
					next.y < position.y + 126)
				{
					if (character.velocity.y < 0.0f)
					{
						character.position.y = position.y + 126;
					}
					character.velocity.y = 0.0f;
					character.isGround = true;
				}
			}
			
			if (map[x + y * width] == '5')
			{
				if (next.x > position.x - 196 &&
					next.x < position.x + 196 &&
					now.y > position.y - 170 &&
					now.y < position.y + 170)
				{
					character.velocity.y = 0.0f;
					counter = 0;
					hitSound.Play();
					mode = Miss3;
				}
				if (now.x > position.x - 196 &&
					now.x < position.x + 196 &&
					next.y > position.y - 170 &&
					next.y < position.y + 170)
				{
					if (character.velocity.y < 0.0f)
					{
						character.position.y = position.y + 116;
					}
					character.velocity.y = 0.0f;
					character.isGround = true;
				}
			}

			if (map[x + y * width] == 'a')
			{
				if (now.x > position.x - 32 &&
					now.x < position.x + 32 &&
					now.y > position.y - 320 &&
					now.y < position.y + 320)
				{
					counter = 0;
					goalSound.Play();
					mode = Goal1;
				}
			}
			if (map[x + y * width] == 'b')
			{
				if (now.x > position.x - 32 &&
					now.x < position.x + 32 &&
					now.y > position.y - 320 &&
					now.y < position.y + 320)
				{
					counter = 0;
					goalSound.Play();
					mode = Goal2;
				}
			}
			if (map[x + y * width] == 'c')
			{
				if (now.x > position.x - 32 &&
					now.x < position.x + 32 &&
					now.y > position.y - 320 &&
					now.y < position.y + 320)
				{
					counter = 0;
					goalSound.Play();
					mode = Goal3;
				}
			}
			if (map[x + y * width] == '@')
			{
				if (now.x > position.x - 32 &&
					now.x < position.x + 32 &&
					now.y > position.y - 320 &&
					now.y < position.y + 320)
				{
					counter = 0;
					goalSound.Play();
					mode = Goal4;
				}
			}
		}
	}
}

void DrawCharacter(Sprite& sprite, Character& character)
{
	sprite.position = character.position;
	sprite.scale = character.scale;
	sprite.Draw();
}


int MAIN()
{
	Camera camera;

	Sprite playerSprite(L"penguin.png");
	Sprite playerSprite2_1(L"penguin01.png");
	Sprite playerSprite2_2(L"penguin02.png");
	Sprite playerSprite3(L"SB-onP.png");
	Sprite playerSprite4(L"BP-05.png");
	Sprite playerSprite5(L"BP-04.png");
	Sprite playerSprite6(L"BP-03.png");
	Sprite playerSprite7(L"BP-02.png");
	Sprite playerSprite8(L"BP-01.png");
	Sprite missSprite1(L"penguin03.png");
	Sprite missSprite2(L"penguin04.png");
	Character player;

	Sprite enemySprite(L"syachi.png");
	Sprite enemySprite2_1(L"taka01.png");
	Sprite enemySprite2_2(L"taka02.png");
	Sprite enemySprite2_3(L"taka03.png");
	Sprite enemySprite2_4(L"taka04.png");
	Sprite enemySprite2_5(L"taka05.png");
	Sprite enemySprite2_6(L"taka06.png");
	vector<Character> enemys;

	Sprite ground(L"3DIceStage.png");
	ground.scale = Float3(1.0f, 1.0f, 0.0f);

	Sprite ceiling(L"3DIceStage2.png");
	ceiling.scale = Float3(1.0f, 1.0f, 0.0f);
	
	Sprite object1(L"3DIceIcicle2.png");
	object1.scale = 0.5f;

	Sprite object2(L"3DIceIcicle.png");
	object2.scale = 0.5f;

	Sprite ground2(L"3DSeaStage.png");
	ground2.scale = Float3(1.0f, 0.8f, 0.0f);

	Sprite seastaging(L"3DIceStage.png");
	seastaging.scale = Float3(1.0f, 0.75f, 0.0f);
	seastaging.color = Float4(0.0f, 0.75f, 0.0f, 1.0f);

	Sprite seaobject(L"3DIceIcicle3.png");
	seaobject.scale = Float3(0.3f,0.2f,0.0f);
	seaobject.color = Float4(0.3f, 0.3f, 0.3f, 1.0f);

	Sprite ground3(L"3DCityStage.png");
	ground3.scale = Float3(1.0f, 1.0f, 0.0f);

	Sprite staging(L"3DCityStage.png");
	staging.scale = Float3(1.0f, 0.5f, 0.0f);

	Sprite object3(L"3DCityGabage.png");
	object3.scale = 0.4f;

	Sprite object4(L"3DCityGabage.png");
	object4.scale = 0.6f;

	Sprite object5(L"3DCityGabage.png");

	Sprite ring(L"ring.png");
	ring.scale = 5.0f;
	
	Sprite ring2(L"ring.png");
	ring2.scale = 6.0f;

	Sprite goal(L"goal.png");
	goal.scale = 2.0f;

	Sprite title1(L"写真 2018-06-08 11 23 33.png");
	title1.scale = 0.4f;
	title1.position = Float3(1000.0f, 330.0f, 0.0f);

	Text title2(L"SPACE Key   GAME START", 20.0f);
	title2.position = Float3(1000.0f, 150.0f, 0.0f);
	title2.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);

	Text title3(L"B Key   HELP", 20.0f);
	title3.position = Float3(1000.0f, 60.0f, 0.0f);
	title3.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);

	Text HelpText1(L"B Key   TITLE", 20.0f);
	HelpText1.position = Float3(1000.0f, 60.0f, 0.0f);
	HelpText1.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);

	Text HelpText2(L"SPACE Key   NEXT", 20.0f);
	HelpText2.position = Float3(1000.0f, 110.0f, 0.0f);
	HelpText2.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);

	Text HelpText3(L"はじめに", 50.0f);
	HelpText3.position = Float3(1000.0f, 460.0f, 0.0f);
	HelpText3.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);

	Text HelpText4(L"あそびかた", 50.0f);
	HelpText4.position = Float3(1000.0f, 460.0f, 0.0f);
	HelpText4.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);

	Text HelpText5(L"スコアについて", 50.0f);
	HelpText5.position = Float3(1000.0f, 460.0f, 0.0f);
	HelpText5.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);
	
	Text HelpText6(L"ゲームオーバーと", 50.0f);
	HelpText6.position = Float3(1000.0f, 460.0f, 0.0f);
	HelpText6.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);

	Text HelpText7(L"コンティニュー", 50.0f);
	HelpText7.position = Float3(1000.0f, 400.0f, 0.0f);
	HelpText7.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);
	
	Text Help1_1(L"これは、", 30.0f);
	Text Help1_2(L"一羽のペンギンが空を飛ぶことを夢見て", 30.0f);
	Text Help1_3(L"各地を旅するゲームである。", 30.0f);
	Help1_1.position = Float3(1000.0f, 300.0f, 0.0f);
	Help1_2.position = Float3(1000.0f, 260.0f, 0.0f);
	Help1_3.position = Float3(1000.0f, 220.0f, 0.0f);

	Text Help2_1(L"・ステージは氷、海、街の３つ", 20.0f);
	Text Help2_2(L"・プレイヤーは自動で右に進む", 20.0f);
	Text Help2_3(L"・SPACEキーでジャンプ（海ステージでは上昇）", 20.0f);
	Text Help2_3_2(L"ジャンプ力は押した長さで変化", 20.0f);
	Text Help2_4(L"・特定の場所まで進むとステージクリア", 20.0f);
	Text Help2_5(L"街ステージまでクリアすると・・・？", 20.0f);
	Help2_1.position = Float3(1000.0f, 360.0f, 0.0f);
	Help2_2.position = Float3(1000.0f, 330.0f, 0.0f);
	Help2_3.position = Float3(1000.0f, 300.0f, 0.0f);
	Help2_3_2.position = Float3(1000.0f, 270.0f, 0.0f);
	Help2_4.position = Float3(1000.0f, 240.0f, 0.0f);
	Help2_5.position = Float3(1000.0f, 180.0f, 0.0f);

	Text Help3_1(L"・氷、街は時間経過（進んだ距離）で上昇", 20.0f);
	Text Help3_2(L"・海はリングをくぐると上昇", 20.0f);
	Text Help3_3(L"・ステージクリアで２００上昇", 20.0f);
	Text Help3_4(L"・街ステージまで一度もコンティニュー（後述）を", 20.0f);
	Text Help3_5(L"せずにクリアで５００上昇", 20.0f);
	Help3_1.position = Float3(1000.0f, 360.0f, 0.0f);
	Help3_2.position = Float3(1000.0f, 330.0f, 0.0f);
	Help3_3.position = Float3(1000.0f, 300.0f, 0.0f);
	Help3_4.position = Float3(1000.0f, 270.0f, 0.0f);
	Help3_5.position = Float3(900.0f, 245.0f, 0.0f);

	Text Help4_1(L"・障害物や敵にぶつかるとゲームオーバー", 20.0f);
	Text Help4_2(L"・海、街でゲームオーバーになった場合は、", 20.0f);
	Text Help4_3(L"Bキーでそのステージの最初からやり直せる", 20.0f);
	Text Help4_4(L"（コンティニュー）", 20.0f);
	Text Help4_5(L"ただし、スコアは０にリセットされる", 20.0f);
	Help4_1.position = Float3(1000.0f, 330.0f, 0.0f);
	Help4_2.position = Float3(1000.0f, 290.0f, 0.0f);
	Help4_3.position = Float3(1000.0f, 265.0f, 0.0f);
	Help4_4.position = Float3(1000.0f, 240.0f, 0.0f);
	Help4_5.position = Float3(1000.0f, 215.0f, 0.0f);

	Sprite Score(L"写真 2018-06-08 17 19 57.png");
	Score.scale = 0.2f;
	Text ScoreText(L"0", 30.0f);
	ScoreText.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	Sprite HighScore(L"写真 2018-06-08 17 19 05.png");
	HighScore.scale = 0.2f;
	Text HighScoreText(L"0", 30.0f);
	HighScoreText.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text interval1(L"STAGE1", 50.0f);
	interval1.position = Float3(1000.0f, 310.0f, 0.0f);
	interval1.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text icestage(L"ICE STAGE", 30.0f);
	icestage.position = Float3(1000.0f, 220.0f, 0.0f);
	icestage.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text interval2(L"STAGE2", 50.0f);
	interval2.position = Float3(1000.0f, 310.0f, 0.0f);
	interval2.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text seastage(L"SEA STAGE", 30.0f);
	seastage.position = Float3(1000.0f, 220.0f, 0.0f);
	seastage.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text interval3(L"STAGE3", 50.0f);
	interval3.position = Float3(1000.0f, 310.0f, 0.0f);
	interval3.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text citystage(L"CITY STAGE", 30.0f);
	citystage.position = Float3(1000.0f, 230.0f, 0.0f);
	citystage.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text cityhelp1(L"【Advise】", 20.0f);
	Text cityhelp2(L"ゴミ箱の上には乗ることができる", 20.0f);
	cityhelp1.position = Float3(1000.0f, 180.0f, 0.0f);
	cityhelp2.position = Float3(1000.0f, 160.0f, 0.0f);
	cityhelp1.color = Float4(1.0f, 1.0f, 0.0f, 1.0f);
	cityhelp2.color = Float4(1.0f, 1.0f, 0.0f, 1.0f);

	Text interval4(L"BONUS GAME", 50.0f);
	interval4.position = Float3(1000.0f, 260.0f, 0.0f);
	interval4.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text interval5(L"3秒後にゲーム開始", 20.0f);
	interval5.position = Float3(1000.0f, 100.0f, 0.0f);
	interval5.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text interval6(L"2秒後にゲーム開始", 20.0f);
	interval6.position = Float3(1000.0f, 100.0f, 0.0f);
	interval6.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text interval7(L"1秒後にゲーム開始", 20.0f);
	interval7.position = Float3(1000.0f, 100.0f, 0.0f);
	interval7.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text interval8(L"ちょっとまってね", 40.0f);
	interval8.position = Float3(1000.0f, 310.0f, 0.0f);
	interval8.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text start1(L"READY", 50.0f);
	Text start2(L"GO!", 50.0f);
	start1.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);
	start2.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

	Text finish(L"FINISH!", 50.0f);
	finish.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

	Sprite clear1(L"写真 2018-06-08 17 09 29.png");
	clear1.scale = 0.4f;
	
	Text clear2(L"SPACEキーで次のステージへ進みます", 20.0f);
	clear2.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

	Text clear3(L"SPACEキーで次に進みます", 20.0f);
	clear3.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

	Text clear4(L"CLEAR BONUS +200", 40.0f);
	clear4.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

	Text clear5(L"NO CONTINUE BONUS +500", 40.0f);
	clear5.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

	Text Bonus1(L"BONUS GAME RESULT", 50.0f);
	Bonus1.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

	Text Bonus2(L"GOAL BONUS +500", 40.0f);
	Bonus2.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

	Text Bonus3_1(L"BALLOON BONUS +1000", 40.0f);
	Text Bonus3_2(L"BALLOON BONUS +800", 40.0f);
	Text Bonus3_3(L"BALLOON BONUS +600", 40.0f);
	Text Bonus3_4(L"BALLOON BONUS +400", 40.0f);
	Text Bonus3_5(L"BALLOON BONUS +200", 40.0f);
	Bonus3_1.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);
	Bonus3_2.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);
	Bonus3_3.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);
	Bonus3_4.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);
	Bonus3_5.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);

	Text Continue(L"B Key   CONTINUE", 20.0f);
	Continue.position = Float3(1000.0f, 100.0f, 0.0f);
	Continue.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Text Return(L"SPACEキーでタイトル画面に戻ります", 20.0f);
	Return.position = Float3(1000.0f, 50.0f, 0.0f);
	Return.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	Sprite Gameover(L"写真 2018-06-08 16 59 04.png");
	Gameover.scale = 0.4f;
	Gameover.position = Float3(1000.0f, 260.0f, 0.0f);

	Text staff1(L"ペンちゃん育成物語", 70.0f);
	Text staff2(L"制作 チームB【ペンちゃん見守り隊】", 30.0f);
	Text staff3(L"メンバー", 50.0f);
	Text staff4(L"ゆうと", 30.0f);
	Text staff5(L"りょうが", 30.0f);
	Text staff6(L"てん", 30.0f);
	Text staff7(L"ヒサミツ＝サン", 30.0f);
	Text staff8(L"Takumi Okazaki", 30.0f);
	Text staff9(L"0371kousuke", 30.0f);
	Text thankyou(L"THANK YOU", 70.0f);
	Text forplaying(L"FOR PLAYING!", 70.0f);

	staff1.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);
	staff2.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);
	staff3.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	staff4.color = Float4(1.0f, 1.0f, 0.0f, 1.0f);
	staff5.color = Float4(1.0f, 1.0f, 0.0f, 1.0f);
	staff6.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);
	staff7.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);
	staff8.color = Float4(0.0f, 0.25f, 1.0f, 1.0f);
	staff9.color = Float4(0.0f, 0.25f, 1.0f, 1.0f);
	thankyou.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	forplaying.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	staff1.position = Float3(1000.0f, -30.0f, 0.0f);
	staff2.position = Float3(1000.0f, -120.0f, 0.0f);
	staff3.position = Float3(1000.0f, -290.0f, 0.0f);
	staff4.position = Float3(1000.0f, -360.0f, 0.0f);
	staff5.position = Float3(1000.0f, -410.0f, 0.0f);
	staff6.position = Float3(1000.0f, -460.0f, 0.0f);
	staff7.position = Float3(1000.0f, -510.0f, 0.0f);
	staff8.position = Float3(1000.0f, -560.0f, 0.0f);
	staff9.position = Float3(1000.0f, -610.0f, 0.0f);
	thankyou.position = Float3(1000.0f, 310.0f, 0.0f);
	forplaying.position = Float3(1000.0f, 220.0f, 0.0f);

	App::SetTitle(L"ペンちゃん育成物語");

	App::SetWindowSize(1024, 576);

	while (App::Refresh())
	{
		switch (mode)
		{

		case Title:

			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.color = Float4(0.5f, 1.0f, 1.0f, 1.0f);
			camera.Update();

			if (App::GetKeyDown(VK_SPACE))
			{
				continueCount = 0;
				mode = Interval1;
			}

			if (App::GetKeyDown('B'))
			{
				mode = Help;
			}

			title1.Draw();
			title2.Draw();
			title3.Draw();

			break;

		case Help:

			camera.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);
			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.Update();

			if (App::GetKeyDown('B'))
			{
				mode = Title;
			}
			if (App::GetKeyDown(VK_SPACE))
			{
				mode = Help2;
			}

			HelpText1.Draw();
			HelpText2.Draw();
			HelpText3.Draw();
			Help1_1.Draw();
			Help1_2.Draw();
			Help1_3.Draw();

			break;

		case Help2:

			camera.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);
			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.Update();

			if (App::GetKeyDown('B'))
			{
				mode = Title;
			}

			if (App::GetKeyDown(VK_SPACE))
			{
				mode = Help3;
			}

			HelpText1.Draw();
			HelpText2.Draw();
			HelpText4.Draw();
			Help2_1.Draw();
			Help2_2.Draw();
			Help2_3.Draw();
			Help2_3_2.Draw();
			Help2_4.Draw();
			Help2_5.Draw();

			break;

		case Help3:

			camera.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);
			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.Update();

			if (App::GetKeyDown('B'))
			{
				mode = Title;
			}

			if (App::GetKeyDown(VK_SPACE))
			{
				mode = Help4;
			}

			HelpText1.Draw();
			HelpText2.Draw();
			HelpText5.Draw();
			Help3_1.Draw();
			Help3_2.Draw();
			Help3_3.Draw();
			Help3_4.Draw();
			Help3_5.Draw();

			break;

		case Help4:

			camera.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);
			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.Update();

			if (App::GetKeyDown('B'))
			{
				mode = Title;
			}

			HelpText1.Draw();
			HelpText6.Draw();
			HelpText7.Draw();
			Help4_1.Draw();
			Help4_2.Draw();
			Help4_3.Draw();
			Help4_4.Draw();
			Help4_5.Draw();

			break;

		case Interval1:

			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.color = Float4(0.0f, 0.0f, 0.0f, 1.0f);
			camera.Update();
			counter++;

			if (counter >= 180)
			{
				player = Character();
				map.clear();
				width = 0;
				height = 0;
				counter = 0;

				// ファイルの読み込み
				wifstream file(L"stage1.txt");
				// ファイルの中身（1行）
				wstring line;

				// ファイルを１行ずつ読み込む
				// 全部の行を読み込むまで繰り返す
				while (getline(file, line))
				{
					// ステージの幅を設定
					width = line.length();
					// ステージの高さを加算
					height++;

					// 一文字ずつステージのデータに追加
					for (int i = 0; i < line.length(); i++)
					{
						map.push_back(line[i]);
					}
				}

				for (int x = 0; x < width; x++)
				{
					for (int y = 0; y < height; y++)
					{
						// もしステージデータが'P'だったら
						if (map[x + y * width] == 'P')
						{
							// プレイヤーの座標を設定する
							player.position = Float3(x * 32.0f, (height - y) * 32.0f, 0.0f);
						}
					}
				}
				mode = Start;
			}
			else if (counter < 60)
			{
				interval5.Draw();
			}
			else if (60 <= counter && counter < 120)
			{
				interval6.Draw();
			}
			else
			{
				interval7.Draw();
			}

			interval1.Draw();
			icestage.Draw();
			
			break;

		case Interval2:

			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.color = Float4(0.0f, 0.0f, 0.0f, 1.0f);
			camera.Update();
			counter++;

			if (counter >= 180)
			{
				player = Character();
				animCount = 0;
				counter = 0;
				
				wifstream file(L"stage2.txt");
				wstring line;

				while (getline(file, line))
				{
					width = line.length();
					height++;

					for (int i = 0; i < line.length(); i++)
					{
						map.push_back(line[i]);
					}
				}
				for (int x = 0; x < width; x++)
				{
					for (int y = 0; y < height; y++)
					{
						if (map[x + y * width] == 'P')
						{
							player.position = Float3(x * 32.0f, (height - y) * 32.0f, 0.0f);
						}
						if (map[x + y * width] == 'E')
						{
							Character enemy;
							enemy.position = Float3(x * 32.0f, (height - y) * 32.0f, 0.0f);
							enemys.push_back(enemy);
						}
					}
				}
				mode = Start2;
			}
			else if (counter < 60)
			{
				interval5.Draw();
			}
			else if (60 <= counter && counter < 120)
			{
				interval6.Draw();
			}
			else
			{
				interval7.Draw();
			}

			interval2.Draw();
			seastage.Draw();

			break;

		case Interval3:

			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.color = Float4(0.0f, 0.0f, 0.0f, 1.0f);
			camera.Update();
			counter++;

			if (counter >= 180)
			{
				player = Character();
				counter = 0;

				wifstream file(L"stage3.txt");
				wstring line;

				while (getline(file, line))
				{
					width = line.length();
					height++;

					for (int i = 0; i < line.length(); i++)
					{
						map.push_back(line[i]);
					}
				}
				for (int x = 0; x < width; x++)
				{
					for (int y = 0; y < height; y++)
					{
						if (map[x + y * width] == 'P')
						{
							player.position = Float3(x * 32.0f, (height - y) * 32.0f, 0.0f);
						}
					}
				}
				mode = Start3;
			}
			else if (counter < 60)
			{
				interval5.Draw();
			}
			else if (60 <= counter && counter < 120)
			{
				interval6.Draw();
			}
			else
			{
				interval7.Draw();
			}

			interval3.Draw();
			citystage.Draw();
			cityhelp1.Draw();
			cityhelp2.Draw();

			break;

		case Interval4:

			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.color = Float4(0.0f, 0.0f, 0.0f, 1.0f);
			camera.Update();
			counter++;

			if (counter >= 180)
			{
				player = Character();
				counter = 0;
				animCount = 0;
				playerLife = 5;

				wifstream file(L"stage4.txt");
				wstring line;

				while (getline(file, line))
				{
					width = line.length();
					height++;

					for (int i = 0; i < line.length(); i++)
					{
						map.push_back(line[i]);
					}
				}
				for (int x = 0; x < width; x++)
				{
					for (int y = 0; y < height; y++)
					{
						if (map[x + y * width] == 'P')
						{
							player.position = Float3(x * 32.0f, (height - y) * 32.0f, 0.0f);
						}
						if (map[x + y * width] == 'E')
						{
							Character enemy;
							enemy.position = Float3(x * 32.0f, (height - y) * 32.0f, 0.0f);
							enemys.push_back(enemy);
						}
					}
				}
				mode = Start4;
			}
			else if (counter < 60)
			{
				interval5.Draw();
			}
			else if (60 <= counter && counter < 120)
			{
				interval6.Draw();
			}
			else
			{
				interval7.Draw();
			}

			interval4.Draw();
			
			break;

		case Start:

			camera.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);
			camera.position.y = 260.0f;

			if (player.position.x <= 700.0f)
			{
				camera.position.x = 1000.0f;
			}
			else
			{
				camera.position.x = player.position.x + 300.0f;
			}

			camera.Update();

			player.position.y += player.velocity.y;

			player.position.x += 8.0f;

			if (player.position.x <= 700.0f)
			{
				start1.position = camera.position;
				start1.Draw();
			}
			else if (counter >= 60)
			{
				counter = 0;
				mode = Game1;
			}
			else
			{
				start2.position = camera.position;
				start2.Draw();
				counter++;
			}

			if (counter == 1)
			{
				startSound.Play();
			}

			if (ground.position.y + 68.0f >= player.position.y)
			{
				player.position.y = ground.position.y + 68.0f;
				isGround = true;
			}

			DrawCharacter(playerSprite, player);
			UpdateCharacter(player, width, height, map);

			break;

		case Start2:

			camera.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);

			if (player.position.x <= 400.0f)
			{
				camera.position.x = 700.0f;
			}
			else
			{
				camera.position.x = player.position.x + 300.0f;
			}

			camera.position.y = 260.0f;

			camera.Update();

			player.position.x += 3.0f;

			player.velocity.y = 0.0f;

			player.position.y += player.velocity.y;

			if (player.position.x <= 400.0f)
			{
				start1.position = camera.position;
				start1.Draw();
			}
			else if (counter >= 60)
			{
				counter = 0;
				mode = Game2;
			}
			else
			{
				start2.position = camera.position;
				start2.Draw();
				counter++;
			}

			if (counter == 1)
			{
				startSound.Play();
			}
			
			DrawCharacter(playerSprite2_1, player);
			UpdateCharacter(player, width, height, map);

			break;

		case Start3:

			camera.color = Float4(1.0f, 0.75f, 0.0f, 1.0f);
			camera.position.y = 260.0f;

			if (player.position.x <= 700.0f)
			{
				camera.position.x = 1000.0f;
			}
			else
			{
				camera.position.x = player.position.x + 300.0f;
			}

			camera.Update();

			player.position.y += player.velocity.y;

			player.position.x += 9.0f;

			if (player.position.x <= 700.0f)
			{
				start1.position = camera.position;
				start1.Draw();
			}
			else if (counter >= 60)
			{
				counter = 0;
				mode = Game3;
			}
			else
			{
				start2.position = camera.position;
				start2.Draw();
				counter++;
			}

			if (counter == 1)
			{
				startSound.Play();
			}

			DrawCharacter(playerSprite3, player);
			UpdateCharacter(player, width, height, map);

			break;

		case Start4:

			camera.color = Float4(0.0f, 0.75f, 1.0f, 1.0f);

			if (player.position.x <= 400.0f)
			{
				camera.position.x = 700.0f;
			}
			else
			{
				camera.position.x = player.position.x + 300.0f;
			}

			camera.position.y = 260.0f;

			camera.Update();

			player.position.x += 4.0f;

			player.position.y += 2.0f;

			if (player.position.x <= 400.0f)
			{
				start1.position = camera.position;
				start1.Draw();
			}
			else if (counter >= 60)
			{
				counter = 0;
				mode = Game4;
			}
			else
			{
				start2.position = camera.position;
				start2.Draw();
				counter++;
			}

			if (counter == 1)
			{
				startSound.Play();
			}

			DrawCharacter(playerSprite4, player);
			player.scale = 0.8f;

			break;

		case Game1:

			camera.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);

			camera.position.y = 260.0f;

			camera.position.x = player.position.x + 300.0f;
			
			camera.Update();

			player.position.y += player.velocity.y;

			player.position.x += 8.0f;

			counter++;

			if (ground.position.y + 68.0f >= player.position.y)
			{
				player.position.y = ground.position.y + 68.0f;
				player.velocity.y = 0.0f;
				isGround = true;
			}
			else
			{
				isGround = false;
			}

			if (App::GetKeyDown(VK_SPACE))
			{
				if (isGround)
				{
					jumpSound.Play();
					player.velocity.y = 17.0f;
					isGround = false;
				}
			}

			if (App::GetKeyUp(VK_SPACE))
			{
				if (player.velocity.y > 0.0)
				{
					player.velocity.y *= 0.5;
				}
			}

			stage1score++;
			stage1score = counter / 12;

			DrawCharacter(playerSprite, player);
			UpdateCharacter(player, width, height, map);

			break;

		case Game2:

			camera.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);

			camera.position.x = player.position.x + 300.0f;

			camera.position.y = 260.0f;

			camera.Update();

			player.velocity.y += 0.4f;

			player.position.y += player.velocity.y;

			player.position.x += 3.0f;

			if (player.velocity.y < -2.5f)
			{
				player.velocity.y = -2.5f;
			}

			if (player.position.y > App::GetWindowSize().y - 96.0f)
			{
				player.position.y = App::GetWindowSize().y - 96.0f;
			}
			if (player.position.y < -App::GetWindowSize().y / 2 + 416.0f)
			{
				player.position.y = -App::GetWindowSize().y / 2 + 416.0f;
			}

			if (App::GetKey(VK_SPACE))
			{	
				animCount++;

				if (animCount == 1)
				{
					jumpSound.Play();
				}
				
				if (animCount >= 0 && animCount < 10)
				{
					DrawCharacter(playerSprite2_2, player);
				}
				else if (animCount >= 20)
				{
					DrawCharacter(playerSprite2_2, player);
					animCount = 0;
				}
				else
				{
					DrawCharacter(playerSprite2_1, player);
				}
				player.velocity.y = 2.0f;
			}
			else
			{
				DrawCharacter(playerSprite2_1, player);
			}
			
			if (App::GetKeyUp(VK_SPACE))
			{
				animCount = 0;
			}

			for (int i = 0; i < enemys.size(); i++)
			{
				if (enemys[i].position.x < App::GetWindowSize().x + player.position.x &&
					enemys[i].position.x > -App::GetWindowSize().x / 2 + player.position.x)
				{
					enemys[i].position.x -= 2.0f;

					UpdateCharacter(enemys[i], width, height, map);
				}
				DrawCharacter(enemySprite, enemys[i]);
			}

			UpdateCharacter(player, width, height, map);

			for (int i = 0; i < enemys.size(); i++)
			{
				if (player.position.x > enemys[i].position.x - 108 &&
					player.position.x < enemys[i].position.x + 64 &&
					player.position.y > enemys[i].position.y - 56 &&
					player.position.y < enemys[i].position.y + 56)
				{
					counter = 0;
					hitSound.Play();
					mode = Miss2;
				}
			}

			break;

		case Game3:

			camera.color = Float4(1.0f, 0.75f, 0.0f, 1.0f);

			camera.position.y = 260.0f;

			camera.position.x = player.position.x + 300.0f;
			
			camera.Update();

			player.position.y += player.velocity.y;

			player.position.x += 9.0f;

			counter++;

			if (App::GetKeyDown(VK_SPACE))
			{
				if (player.isGround)
				{
					player.velocity.y = 17.0f;
					jumpSound.Play();
				}
			}

			if (App::GetKeyUp(VK_SPACE))
			{
				if (player.velocity.y > 0.0)
				{
					player.velocity.y *= 0.5;
				}
			}
	
			stage3score++;
			stage3score = counter / 6;

			DrawCharacter(playerSprite3, player);
			UpdateCharacter(player, width, height, map);

			break;

		case Game4:

			camera.color = Float4(0.0f, 0.75f, 1.0f, 1.0f);

			camera.position.x = player.position.x + 300.0f;

			camera.position.y = 260.0f;

			camera.Update();

			player.velocity.y += 0.4f;

			player.position.y += player.velocity.y;

			player.position.x += 4.0f;

			if (player.velocity.y < -4.0f)
			{
				player.velocity.y = -4.0f;
			}
			if (player.velocity.y > 4.0f)
			{
				player.velocity.y = 4.0f;
			}

			if (player.position.y > App::GetWindowSize().y - 72.0f)
			{
				player.position.y = App::GetWindowSize().y - 72.0f;
			}
			if (player.position.y < -App::GetWindowSize().y / 2 + 144.0f)
			{
				hitSound.Play();
				mode = Miss4;
			}

			if (App::GetKey(VK_SPACE))
			{
				player.velocity.y += 0.3f;
			}

			for (int i = 0; i < enemys.size(); i++)
			{
				animCount++;

				if (animCount >= 0 && animCount < 60)
				{
					DrawCharacter(enemySprite2_1, enemys[i]);
					enemys[i].scale = 0.6f;
				}
				else if (animCount >= 60 && animCount < 120)
				{
					DrawCharacter(enemySprite2_2, enemys[i]);
					enemys[i].scale = 0.6f;
				}
				else if (animCount >= 120 && animCount < 180)
				{
					DrawCharacter(enemySprite2_3, enemys[i]);
					enemys[i].scale = 0.6f;
				}
				else if (animCount >= 180 && animCount < 240)
				{
					DrawCharacter(enemySprite2_4, enemys[i]);
					enemys[i].scale = 0.6f;
				}
				else if (animCount >= 240 && animCount < 300)
				{
					DrawCharacter(enemySprite2_5, enemys[i]);
					enemys[i].scale = 0.6f;
				}
				else if (animCount >= 300 && animCount < 360)
				{
					DrawCharacter(enemySprite2_6, enemys[i]);
					enemys[i].scale = 0.6f;
				}
				else if (animCount >= 360)
				{
					DrawCharacter(enemySprite2_1, enemys[i]);
					enemys[i].scale = 0.6f;
					animCount = 0;
				}

				if (enemys[i].position.x < App::GetWindowSize().x + player.position.x &&
					enemys[i].position.x > -App::GetWindowSize().x / 2 + player.position.x)
				{
					enemys[i].position.x -= 2.0f;

					UpdateCharacter(enemys[i], width, height, map);
				}
			}

			if (playerLife == 5)
			{
				DrawCharacter(playerSprite4, player);
			}
			else if (playerLife == 4)
			{
				DrawCharacter(playerSprite5, player);
			}
			else if (playerLife == 3)
			{
				DrawCharacter(playerSprite6, player);
			}
			else if (playerLife == 2)
			{
				DrawCharacter(playerSprite7, player);
			}
			else if (playerLife == 1)
			{
				DrawCharacter(playerSprite8, player);
			}
			else if (playerLife == 0)
			{
				player.position.y -= 100.0f;
				mode = Miss4;
			}
	
			UpdateCharacter(player, width, height, map);

			for (int i = 0; i < enemys.size(); i++)
			{
				if (player.position.x > enemys[i].position.x - 60 &&
					player.position.x < enemys[i].position.x + 80 &&
					player.position.y > enemys[i].position.y - 124 &&
					player.position.y < enemys[i].position.y + 128)
				{
					if (enemys[i].counter < 1)
					{
						playerLife -= 1;
						hitSound.Play();
						enemys[i].counter++;
					}
				}
				else
				{
					enemys[i].counter = 0;
				}
			}

			break;

		case Miss:

			camera.Update();

			camera.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);

			player.position.y += player.velocity.y;

			if (player.velocity.y < -15.0f)
			{
				player.velocity.y = -15.0f;
			}

			counter++;

			if (counter < 30)
			{
				player.velocity.y = 0.0f;
			}
			else if (counter == 30)
			{
				missSound.Play();
				player.velocity.y = 10.0f;
			}
			else if (counter == 240)
			{
				map.clear();
				width = 0;
				height = 0;
				overSound.Play();
				mode = Over;
			}
			else
			{
				player.velocity.y -= 0.3f;
			}

			DrawCharacter(missSprite2, player);
			player.scale = 0.4f;
			
			break;

		case Miss2:

			camera.Update();

			camera.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);

			player.position.y += player.velocity.y;

			if (player.velocity.y < -15.0f)
			{
				player.velocity.y = -15.0f;
			}

			counter++;

			if (counter < 30)
			{
				player.velocity.y = 0.0f;
			}
			else if (counter == 30)
			{
				missSound.Play();
				player.velocity.y = 10.0f;
			}
			else if (counter == 240)
			{
				map.clear();
				enemys.clear();
				width = 0;
				height = 0;
				overSound.Play();
				mode = Over2;
			}
			else
			{
				player.velocity.y -= 0.3f;
			}

			for (int i = 0; i < enemys.size(); i++)
			{
				DrawCharacter(enemySprite, enemys[i]);
			}

			DrawCharacter(missSprite1, player);

			break;

		case Miss3:

			camera.Update();

			camera.color = Float4(1.0f, 0.75f, 0.0f, 1.0f);

			player.position.y += player.velocity.y;

			if (player.velocity.y < -15.0f)
			{
				player.velocity.y = -15.0f;
			}

			counter++;

			if (counter < 30)
			{
				player.velocity.y = 0.0f;
			}
			else if (counter == 30)
			{
				missSound.Play();
				player.velocity.y = 10.0f;
			}
			else if (counter == 240)
			{
				map.clear();
				width = 0;
				height = 0;
				overSound.Play();
				mode = Over3;
			}
			else
			{
				player.velocity.y -= 0.3f;
			}

			DrawCharacter(missSprite2, player);

			break;

		case Miss4:

			camera.Update();

			camera.color = Float4(0.0f, 0.75f, 1.0f, 1.0f);

			player.position.y += player.velocity.y;

			if (player.velocity.y < -15.0f)
			{
				player.velocity.y = -15.0f;
			}

			counter++;

			if (counter < 30)
			{
				player.velocity.y = 0.0f;
			}
			else if (counter == 30)
			{
				missSound.Play();
			}
			else if (counter == 240)
			{
				map.clear();
				enemys.clear();
				width = 0;
				height = 0;
				overSound.Play();
				mode = Over4;
			}
			else
			{
				player.velocity.y -= 0.3f;
			}

			for (int i = 0; i < enemys.size(); i++)
			{
				if (animCount >= 0 && animCount < 60)
				{
					DrawCharacter(enemySprite2_1, enemys[i]);
					enemys[i].scale = 0.6f;
				}
				else if (animCount >= 60 && animCount < 120)
				{
					DrawCharacter(enemySprite2_2, enemys[i]);
					enemys[i].scale = 0.6f;
				}
				else if (animCount >= 120 && animCount < 180)
				{
					DrawCharacter(enemySprite2_3, enemys[i]);
					enemys[i].scale = 0.6f;
				}
				else if (animCount >= 180 && animCount < 240)
				{
					DrawCharacter(enemySprite2_4, enemys[i]);
					enemys[i].scale = 0.6f;
				}
				else if (animCount >= 240 && animCount < 300)
				{
					DrawCharacter(enemySprite2_5, enemys[i]);
					enemys[i].scale = 0.6f;
				}
				else if (animCount >= 300 && animCount < 360)
				{
					DrawCharacter(enemySprite2_6, enemys[i]);
					enemys[i].scale = 0.6f;
				}
				else if (animCount >= 360)
				{
					DrawCharacter(enemySprite2_1, enemys[i]);
					enemys[i].scale = 0.6f;
					animCount = 0;
				}
			}

			DrawCharacter(missSprite1, player);
			player.scale = 0.4f;

			break;

		case Over:

			camera.color = Float4(0.0f, 0.0f, 0.0f, 1.0f);
			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.Update();
			
			counter++;

			if (counter >= 420)
			{
				Return.Draw();

				if (App::GetKeyDown(VK_SPACE))
				{
					stage1score = 0;
					stage2score = 0;
					stage3score = 0;
					totalscore = 0;
					counter = 0;
					mode = Title;
				}
			}

			Gameover.Draw();

			break;

		case Over2:

			camera.color = Float4(0.0f, 0.0f, 0.0f, 1.0f);
			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.Update();

			counter++;

			if (counter >= 420)
			{
				Continue.Draw();
				Return.Draw();

				if (App::GetKeyDown(VK_SPACE))
				{
					stage1score = 0;
					stage2score = 0;
					stage3score = 0;
					totalscore = 0;
					counter = 0;
					mode = Title;
				}

				if (App::GetKeyDown('B'))
				{
					stage1score = 0;
					stage2score = 0;
					stage3score = 0;
					totalscore = 0;
					counter = 0;
					continueCount++;
					mode = Interval2;
				}
			}

			Gameover.Draw();

			break;

		case Over3:

			camera.color = Float4(0.0f, 0.0f, 0.0f, 1.0f);
			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.Update();

			counter++;

			if (counter >= 420)
			{
				Continue.Draw();
				Return.Draw();

				if (App::GetKeyDown(VK_SPACE))
				{
					stage1score = 0;
					stage2score = 0;
					stage3score = 0;
					totalscore = 0;
					counter = 0;
					mode = Title;
				}

				if (App::GetKeyDown('B'))
				{
					stage1score = 0;
					stage2score = 0;
					stage3score = 0;
					totalscore = 0;
					counter = 0;
					continueCount++;
					mode = Interval3;
				}
			}

			Gameover.Draw();

			break;

		case Over4:

			camera.color = Float4(0.0f, 0.0f, 0.0f, 1.0f);
			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.Update();

			counter++;

			if (counter >= 420)
			{
				clear3.position = Float3(1000.0f, 100.0f, 0.0f);
				clear3.color = Float4(1.0f, 1.0f, 1.0f, 1.0f);
				clear3.Draw();

				if (App::GetKeyDown(VK_SPACE))
				{
					counter = 0;
					mode = Staff;
				}
			}

			Gameover.Draw();

			break;

		case Goal1:
			
			camera.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);

			camera.Update();

			player.velocity.y -= 0.5f;

			if (ground.position.y + 68.0f >= player.position.y)
			{
				player.position.y = ground.position.y + 68.0f;
			}
			else
			{
				player.position.y += player.velocity.y;
			}

			if (player.position.x > goal.position.x + App::GetWindowSize().x - 128.0f)
			{
				player.position.x += 0.0f;
				counter++;
			}
			else
			{
				player.position.x += 8.0f;
			}

			if (counter >= 60)
			{
				clearSound.Play();
				mode = Clear1;
			}

			finish.position = camera.position;
			finish.Draw();
			DrawCharacter(playerSprite, player);

			break;

		case Goal2:

			camera.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);

			camera.Update();

			if (player.position.x > goal.position.x + App::GetWindowSize().x - 128.0f)
			{
				player.position.x += 0.0f;
				counter++;
			}
			else
			{
				player.position.x += 3.0f;
			}

			if (counter >= 60)
			{
				clearSound.Play();
				mode = Clear2;
			}

			finish.position = camera.position;
			finish.Draw();
			DrawCharacter(playerSprite2_1, player);

			break;

		case Goal3:

			camera.color = Float4(1.0f, 0.75f, 0.0f, 1.0f);

			camera.Update();

			player.velocity.y -= 0.5f;

			if (ground3.position.y + 10 >= player.position.y)
			{
				player.position.y = ground3.position.y + 10;
			}
			else
			{
				player.position.y += player.velocity.y;
			}

			if (player.position.x > goal.position.x + App::GetWindowSize().x - 128.0f)
			{
				player.position.x += 0.0f;
				counter++;
			}
			else
			{
				player.position.x += 9.0f;
			}

			if (counter >= 60)
			{
				clearSound.Play();
				mode = Clear3;
			}

			finish.position = camera.position;
			finish.Draw();
			DrawCharacter(playerSprite3, player);
			
			break;

		case Goal4:

			camera.color = Float4(0.0f, 0.75f, 1.0f, 1.0f);

			camera.Update();

			if (player.position.x > goal.position.x + App::GetWindowSize().x - 128.0f)
			{
				player.position.x += 0.0f;
				counter++;
			}
			else
			{
				player.position.x += 4.0f;
			}

			if (counter >= 60)
			{
				clearSound.Play();
				mode = Result;
			}

			finish.position = camera.position;
			finish.Draw();

			if (playerLife == 5)
			{
				DrawCharacter(playerSprite4, player);
			}
			else if (playerLife == 4)
			{
				DrawCharacter(playerSprite5, player);
			}
			else if (playerLife == 3)
			{
				DrawCharacter(playerSprite6, player);
			}
			else if (playerLife == 2)
			{
				DrawCharacter(playerSprite7, player);
			}
			else if (playerLife == 1)
			{
				DrawCharacter(playerSprite8, player);
			}

			break;

		case Clear1:

			camera.color = Float4(0.0f, 1.0f, 1.0f, 1.0f);

			camera.Update();

			counter++;

			if (counter >= 90)
			{
				clear1.position = Float3(camera.position.x, 310.0f, 0.0f);
				clear1.Draw();
			}

			if (counter >= 210)
			{
				clear4.position = Float3(camera.position.x, 200.0f, 0.0f);
				clear4.Draw();
			}

			if (counter == 210)
			{
				pointSound.Play();
				stage1score += 200;
			}

			if (counter >= 270)
			{
				if (App::GetKeyDown(VK_SPACE))
				{
					map.clear();
					width = 0;
					height = 0;
					counter = 0;
					mode = Interval2;
				}

				clear2.position = Float3(camera.position.x, 140.0f, 0.0f);
				clear2.Draw();
			}

			break;

		case Clear2:

			camera.color = Float4(0.0f, 0.0f, 1.0f, 1.0f);

			camera.Update();

			counter++;

			if (counter >= 90)
			{
				clear1.position = Float3(camera.position.x, 310.0f, 0.0f);
				clear1.Draw();
			}

			if (counter >= 210)
			{
				clear4.position = Float3(camera.position.x, 200.0f, 0.0f);
				clear4.Draw();
			}

			if (counter == 210)
			{
				pointSound.Play();
				stage2score += 200;
			}

			if (counter >= 270)
			{
				if (App::GetKeyDown(VK_SPACE))
				{
					enemys.clear();
					map.clear();
					width = 0;
					height = 0;
					counter = 0;
					mode = Interval3;
				}

				clear2.position = Float3(camera.position.x, 140.0f, 0.0f);
				clear2.Draw();
			}

			break;

		case Clear3:

			camera.color = Float4(1.0f, 0.75f, 0.0f, 1.0f);

			camera.Update();

			counter++;

			if (counter >= 90)
			{
				clear1.position = Float3(camera.position.x, 310.0f, 0.0f);
				clear1.Draw();
			}

			if (counter >= 210)
			{
				clear4.position = Float3(camera.position.x, 200.0f, 0.0f);
				clear4.Draw();
			}

			if (counter == 210)
			{
				pointSound.Play();
				stage3score += 200;
			}

			if (counter >= 270)
			{
				if (continueCount < 1)
				{
					if (counter == 270)
					{
						pointSound.Play();
						stage3score += 500;
					}

					if (counter >= 340)
					{
						if (App::GetKeyDown(VK_SPACE))
						{
							map.clear();
							width = 0;
							height = 0;
							counter = 0;
							mode = Interval4;
						}
						clear3.position = Float3(camera.position.x, 100.0f, 0.0f);
						clear3.Draw();
					}
					clear5.position = Float3(camera.position.x, 150.0f, 0.0f);
					clear5.Draw();
				}
				else
				{
					if (App::GetKeyDown(VK_SPACE))
					{
						map.clear();
						width = 0;
						height = 0;
						counter = 0;
						mode = Interval4;
					}
					clear3.position = Float3(camera.position.x, 140.0f, 0.0f);
					clear3.Draw();
				}
			}

			break;

		case Result:

			camera.color = Float4(0.0f, 0.75f, 1.0f, 1.0f);

			camera.Update();

			counter++;

			if (counter >= 90)
			{
				Bonus1.position = Float3(camera.position.x, 360.0f, 0.0f);
				Bonus1.Draw();
			}

			if (counter >= 210)
			{
				Bonus2.position = Float3(camera.position.x, 270.0f, 0.0f);
				Bonus2.Draw();
			}

			if (counter == 210)
			{
				pointSound.Play();
				bonusscore += 500;
			}

			if (counter >= 270)
			{
				if (playerLife == 5)
				{
					Bonus3_1.position = Float3(camera.position.x, 210.0f, 0.0f);
					Bonus3_1.Draw();
				}
				else if (playerLife == 4)
				{
					Bonus3_2.position = Float3(camera.position.x, 210.0f, 0.0f);
					Bonus3_2.Draw();
				}
				else if (playerLife == 3)
				{
					Bonus3_3.position = Float3(camera.position.x, 210.0f, 0.0f);
					Bonus3_3.Draw();
				}
				else if (playerLife == 2)
				{
					Bonus3_4.position = Float3(camera.position.x, 210.0f, 0.0f);
					Bonus3_4.Draw();
				}
				else if (playerLife == 1)
				{
					Bonus3_5.position = Float3(camera.position.x, 210.0f, 0.0f);
					Bonus3_5.Draw();
				}
			}

			if (counter == 270)
			{
				if (playerLife == 5)
				{
					pointSound.Play();
					bonusscore += 1000;
				}
				else if (playerLife == 4)
				{
					pointSound.Play();
					bonusscore += 800;
				}
				else if (playerLife == 3)
				{
					pointSound.Play();
					bonusscore += 600;
				}
				else if (playerLife == 2)
				{
					pointSound.Play();
					bonusscore += 400;
				}
				else if (playerLife == 1)
				{
					pointSound.Play();
					bonusscore += 200;
				}
			}

			if (counter >= 340)
			{
				if (App::GetKeyDown(VK_SPACE))
				{
					map.clear();
					width = 0;
					height = 0;
					counter = 0;
					mode = Staff;
				}
				clear3.position = Float3(camera.position.x, 100.0f, 0.0f);
				clear3.Draw();
			}

			break;

		case Staff:

			camera.color = Float4(0.0f, 0.0f ,0.5f, 1.0f);
			camera.position = Float3(1000.0f, 260.0f, 0.0f);
			camera.Update();
			counter++;

			staff1.position.y += 1.0f;
			staff2.position.y += 1.0f;
			staff3.position.y += 1.0f;
			staff4.position.y += 1.0f;
			staff5.position.y += 1.0f;
			staff6.position.y += 1.0f;
			staff7.position.y += 1.0f;
			staff8.position.y += 1.0f;
			staff9.position.y += 1.0f;

			if (counter >= 1200)
			{
				thankyou.Draw();
				forplaying.Draw();
			}
			if (counter >= 1350)
			{
				Return.Draw();

				if (App::GetKeyDown(VK_SPACE))
				{
					stage1score = 0;
					stage2score = 0;
					stage3score = 0;
					bonusscore = 0;
					totalscore = 0;
					counter = 0;
					mode = Title;
				}
			}

			staff1.Draw();
			staff2.Draw();
			staff3.Draw();
			staff4.Draw();
			staff5.Draw();
			staff6.Draw();
			staff7.Draw();
			staff8.Draw();
			staff9.Draw();

			break;

		}

		int center = (int)roundf(player.position.x / 32.0f);

		for (int x = center - 28; x <= center + 38; x++)
		{
			for (int y = 0; y < height; y++)
			{
				if (x < 0 || x >= width)
				{
					continue;
				}

				Float3 position = Float3(x * 32.0f, (height - y) * 16.0f, 0.0f);
				
				if (map[x + y * width] == '1')
				{
					object1.position = position;
					object1.Draw();
				}
				if (map[x + y * width] == '2')
				{					
					object2.position = position;					
					object2.Draw();
				}
				if (map[x + y * width] == '3')
				{
					object3.position = position;
					object3.Draw();
				}
				if (map[x + y * width] == '4')
				{
					object4.position = position;
					object4.Draw();
				}
				if (map[x + y * width] == '5')
				{
					object5.position = position;
					object5.Draw();
				}
				if (map[x + y * width] == '<')
				{
					seaobject.position = position;
					seaobject.Draw();
				}
				if (map[x + y * width] == '0')
				{
					ring.position = position;
					ring.Draw();
				}
				if (map[x + y * width] == 'O')
				{
					ring2.position = position;
					ring2.Draw();
				}
				if (map[x + y * width] == 'a')
				{
					goal.position = position;
					goal.angles.y = App::GetTime() * 180.0;
					goal.Draw();
				}
				if (map[x + y * width] == 'b')
				{
					goal.position = position;
					goal.angles.y = App::GetTime() * 180.0;
					goal.Draw();
				}
				if (map[x + y * width] == 'c')
				{
					goal.position = position;
					goal.angles.y = App::GetTime() * 180.0;
					goal.Draw();
				}
				if (map[x + y * width] == '@')
				{
					goal.position = position;
					goal.angles.y = App::GetTime() * 180.0;
					goal.Draw();
				}
				if (map[x + y * width] == 'S')
				{
					ground.position = position;
					ground.Draw();
				}
				if (map[x + y * width] == 'r')
				{
					ground2.position = position;
					ground2.Draw();
				}
				if (map[x + y * width] == 's')
				{
					ground3.position = position;
					ground3.Draw();
				}
				if (map[x + y * width] == 'T')
				{
					ceiling.position = position;
					ceiling.Draw();
				}
				if (map[x + y * width] == 't')
				{
					seastaging.position = position;
					seastaging.Draw();
				}
				if (map[x + y * width] == '#')
				{
					staging.position = position;
					staging.Draw();
				}
			}
		}

		totalscore++;
		totalscore = stage1score + stage2score + stage3score + bonusscore;

		ScoreText.Create(to_wstring(totalscore), 30.0);

		if (totalscore > highscore)
		{
			highscore = totalscore;
		}

		HighScoreText.Create(to_wstring(highscore), 30.0);

		Score.position = Float3(camera.position.x - 300.0f, camera.position.y + 220.0f, 0.0f);
		ScoreText.position = Float3(camera.position.x - 300.0f, camera.position.y + 180.0f, 0.0f);
		Score.Draw();
		ScoreText.Draw();

		HighScore.position = Float3(camera.position.x + 300.0f, camera.position.y + 220.0f, 0.0f);
		HighScoreText.position = Float3(camera.position.x + 300.0f, camera.position.y + 180.0f, 0.0f);
		HighScore.Draw();
		HighScoreText.Draw();
	}

	return 0;
}