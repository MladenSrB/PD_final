
#include <dino_game.h>

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

void DinoGame::displayGame(char map[height][width], int score) {
    system("clear");
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cout << map[i][j];
        }
        cout << endl;
    }
    cout << "Score: " << score << endl;
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
        obstacleSpawnTimer = rand() % 15 + 5;
    } else {
        obstacleSpawnTimer--;
    }

    // 更新障礙物位置和計分
    for (int &x : obstacleX) {
        if (x == 1 && dinoY == height - 2) {
            gameOver = true;
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

void DinoGame::runGame() {
    char map[height][width];
    vector<int> obstacleX;
    int obstacleSpawnTimer = 0;
    int score = 0;

    srand(time(0));

    setNonBlockingInput();

    while (!gameOver) {
        char ch;
        if (read(STDIN_FILENO, &ch, 1) > 0 && ch == ' ') {
            thread t([this] { jump(); });
            t.detach();
        }

        updateMap(map, obstacleX, score, obstacleSpawnTimer);
        displayGame(map, score);
        this_thread::sleep_for(milliseconds(100));
    }

    resetInput();
    cout << "Game Over! Your score: " << score << endl;
}

const char DinoGame::ground = '_';
const char DinoGame::dinosaur = 'T';
const char DinoGame::obstacle = 'O';

int main() {
    DinoGame game;
    game.runGame();
    return 0;
}
