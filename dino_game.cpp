#include "redistributable.h"
#include "dino_game.h"
#include "character.h"
using namespace std;
using namespace chrono;

void DinoGame::setNonBlockingInput() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ICANON;
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
}

void DinoGame::resetInput() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ICANON;
    term.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void DinoGame::displayGame(char map[height][width], int score, int countdown) {
    system("clear");
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cout << map[i][j];
        }
        cout << endl;
    }
    cout << "Score: " << score << endl;
    cout << "Time remaining: " << countdown << " seconds" << endl;
}

void DinoGame::updateMap(char map[height][width], vector<int> &obstacleX, int &score, int &obstacleSpawnTimer) {
    // 清除上一幀
    for (int i = 0; i < height; ++i) {
        fill(map[i], map[i] + width, ' ');
    }

    // 畫地面
    fill(map[height - 1], map[height - 1] + width, ground);

    // 畫恐龍
    map[dinoY][1] = dinosaur;

    // 生成新障礙物
    if (obstacleSpawnTimer <= 0) {
        obstacleX.push_back(width - 1);
        obstacleSpawnTimer = rand() % 10 + 3;  // 調整這裡，讓間隔時間更短
    } else {
        obstacleSpawnTimer--;
    }

    // 更新障礙物位置和計分
    for (int &x : obstacleX) {
        if (x == 1 && dinoY == height - 2) { // 碰撞檢測
            gameOver = true;
        }

        if (x == 1 && dinoY != height - 2) { // 成功避開障礙物
            score++; 
        }
        
        x--;
    }

    obstacleX.erase(remove_if(obstacleX.begin(), obstacleX.end(), [](int x) { return x < 0; }), obstacleX.end());

    // 畫障礙物
    for (int x : obstacleX) {
        if (x > 0 && x < width) {
            map[height - 2][x] = obstacle;
        }
    }
}

void DinoGame::jump() {
    if (isJumping) return;

    isJumping = true;
    for (int i = 0; i < 3; ++i) {
        dinoY--;
        this_thread::sleep_for(milliseconds(100));
    }
    this_thread::sleep_for(milliseconds(50));
    for (int i = 0; i < 3; ++i) {
        dinoY++;
        this_thread::sleep_for(milliseconds(100));
    }
    isJumping = false;
}

void DinoGame::runGame(Player &player) {
    char map[height][width];
    vector<int> obstacleX;
    int obstacleSpawnTimer = 0;
    int score = 0;
    int coin = 0;
    int exp = 0;
    int blood = 10;

    srand(time(0));

    setNonBlockingInput();

    auto startTime = chrono::steady_clock::now(); // 開始計時

    while (!gameOver) {
        // 檢查時間是否超過30秒
        auto currentTime = chrono::steady_clock::now();
        auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
        int countdown = 15 - elapsedTime;

        if (countdown <= 0) {
            coin = score / 3;
            exp = score / 2;
            player.addCoin(coin);
            player.addExp(exp);
            cout << "Congratulations! You won the game!\n" << endl;
            cout << "coin +" << coin << ", total coin: " << player.getCoin() << endl;
            cout << "exp +" << exp << ", total exp: " << player.getExp() << endl;
            break;
        }

        char ch;
        if (read(STDIN_FILENO, &ch, 1) > 0 && ch == ' ') {
            thread t([this] { jump(); });
            t.detach();
        }

        updateMap(map, obstacleX, score, obstacleSpawnTimer);
        displayGame(map, score, countdown);
        this_thread::sleep_for(milliseconds(100));
    }

    if (gameOver) {
        coin = score / 3;
        exp = score / 2;
        player.addCoin(coin);
        player.addExp(exp);
        player.decreaseHp(blood);
        cout << "Game Over!" << endl;
        cout << "coin +" << coin << ", total coin: " << player.getCoin() << endl;
        cout << "exp +" << exp << ", total exp: " << player.getExp() << endl;
        cout << "hp -" << blood << ", total hp: " << player.gethp() << endl;
    }

    resetInput();
}

const char DinoGame::ground = '_';
const char DinoGame::dinosaur = 'T';
const char DinoGame::obstacle = 'O';

 //單獨實行可以用
int main() {
    string name = "ppp";
    Player player(name);
    DinoGame game;
    game.runGame(player);
    return 0;
 }
