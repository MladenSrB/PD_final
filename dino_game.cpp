#include "redistributable.h"

using namespace std;

class DinoGame
{
  protected:
    const int width = 50;
    const int height = 10;
    const char ground = '_';
    const char dinosaur = 'T';
    const char obstacle = 'O';

    int dinoY = height - 2;
    bool isJumping = false;
    bool gameOver = false;

  public:
    // constructor
    //DinoGame();
    // 設置非阻塞輸入
    void setNonBlockingInput();
    // 重置輸入模式
    void resetInput();
    // 顯示遊戲地圖
    void displayGame(char map[height][width], int score);
    // 更新遊戲地圖
    void updateMap(char map[height][width], vector<int> &obstacleX, int &score, int &obstacleSpawnTimer);
    // 跳躍邏輯
    void jump();
    // 遊戲執行
    void runGame();
}



// 設置非阻塞輸入
void DinoGame:: setNonBlockingInput()
{
  termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= ~ICANON; // 即時輸入
  term.c_lflag &= ~ECHO;   // 關閉回顯
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
  fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
}

// 重置輸入模式
void DinoGame::resetInput()
{
  termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag |= ICANON;
  term.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// 顯示遊戲地圖
void DinoGame::displayGame(char map[height][width], int score)
{
  system("clear"); // 清屏
  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      cout << map[i][j];
    }
    cout << endl;
  }
  cout << "Score: " << score << endl;
}

// 更新遊戲地圖
void DinoGame::updateMap(char map[height][width], vector<int> &obstacleX, int &score, int &obstacleSpawnTimer)
{
  // 清除上一幀
  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      if (map[i][j] != ground)
      {
        map[i][j] = ' ';
      }
    }
  }

  // 畫出地面
  for (int j = 0; j < width; ++j)
  {
    map[height - 1][j] = ground;
  }

  // 畫出恐龍
  map[dinoY][1] = dinosaur;

  // 隨機生成新障礙物
  if (obstacleSpawnTimer <= 0)
  {
    obstacleX.push_back(width - 1);       // 新增障礙物到最右邊
    obstacleSpawnTimer = rand() % 15 + 5; // 隨機設定下一次生成時間（5~19個更新周期）
  }
  else
  {
    obstacleSpawnTimer--;
  }

  // 更新障礙物位置並計分
  for (int i = 0; i < obstacleX.size(); ++i)
  {
    if (obstacleX[i] == 1)
    { // 當障礙物剛好離開恐龍位置
      score++;
    }
    obstacleX[i]--; // 障礙物向左移動
  }

  // 移除移出畫面的障礙物
  obstacleX.erase(remove_if(obstacleX.begin(), obstacleX.end(), [](int x)
                            { return x < 0; }),
                  obstacleX.end());

  // 畫出障礙物
  for (int x : obstacleX)
  {
    if (x > 0 && x < width)
    {
      map[height - 2][x] = obstacle;
    }
  }

  // 檢查碰撞
  for (int x : obstacleX)
  {
    if (dinoY == height - 2 && x == 1)
    {
      gameOver = true;
    }
  }
}

// 跳躍邏輯
void DinoGame::jump()
{
  if (!isJumping)
  {
    isJumping = true;

    for (int i = 0; i < 3; ++i)
    { // 恐龍上升
      dinoY--;
      this_thread::sleep_for(chrono::milliseconds(100));
    }
    // 保持在空中
    this_thread::sleep_for(chrono::milliseconds(50)); // 新增空中停留時間

    for (int i = 0; i < 3; ++i)
    { // 恐龍下降
      dinoY++;
      this_thread::sleep_for(chrono::milliseconds(100));
    }

    isJumping = false;
  }
}

// 遊戲執行
void DinoGame::runGame()
{
  char map[height][width];
  vector<int> obstacleX;      // 障礙物的位置集合
  int obstacleSpawnTimer = 0; // 初始化為 0，立即生成第一個障礙物
  int score = 0;

  srand(time(0)); // 初始化隨機數生成器

  // 初始化地圖
  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      map[i][j] = ' ';
    }
  }

  setNonBlockingInput(); // 設置非阻塞輸入

  while (!gameOver)
  {
    char ch;
    // 使用 read() 函數來讀取單個字符，避免等待回車
    if (read(STDIN_FILENO, &ch, 1) > 0)
    {
      if (ch == ' ')
      { // 空白鍵觸發跳躍
        thread t(jump);
        t.detach();
      }
    }

    // 更新地圖和顯示
    updateMap(map, obstacleX, score, obstacleSpawnTimer);
    displayGame(map, score);
    this_thread::sleep_for(chrono::milliseconds(100));
  }

  resetInput(); // 恢復輸入模式

  cout << "Game Over! Your score: " << score << endl;
  return 0;
}