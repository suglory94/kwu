#include "gameboardwidget.h"
#include "ui_gameboardwidget.h"

#include <QStyle>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QFileDialog>

const QVector<QPoint> directions = {
    QPoint(-1, -1), QPoint(-1, 0), QPoint(-1, 1), QPoint(0, -1), QPoint(0, 1), QPoint(1, -1), QPoint(1, 0), QPoint(1, 1)
};

gameboardwidget::gameboardwidget(int size, int obstacleCount, bool blackFirst, const GameSettings& settings, QWidget *parent)
    : QWidget(parent), boardSize(size), cellSize(40), currentPlayerBlack(blackFirst), settings(settings)
    , ui(new Ui::gameboardwidget)
{
    ui->setupUi(this);

    boardData.resize(boardSize, QVector<int>(boardSize, 0));
    int mid = boardSize / 2;
    boardData[mid - 1][mid - 1] = 2;
    boardData[mid][mid] = 2;
    boardData[mid - 1][mid] = 1;
    boardData[mid][mid - 1] = 1;

    if (obstacleCount > 0) {
        placeObstacles(obstacleCount);
    }

    updateValidMoves();

    QWidget *mainContainer = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(mainContainer);

    QWidget *boardArea = new QWidget(this);
    boardArea->setFixedSize(boardSize * cellSize, boardSize * cellSize);
    mainLayout->addWidget(boardArea);

    QWidget *infoWidget = new QWidget(this);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);

    turnLabel = new QLabel(infoWidget);
    blackCountLabel = new QLabel(infoWidget);
    whiteCountLabel = new QLabel(infoWidget);
    backToMenuButton = new QPushButton("메인화면", infoWidget);
    undoButton = new QPushButton("되돌리기", infoWidget);
    infoLayout->addWidget(undoButton);

    infoLayout->addWidget(turnLabel);
    infoLayout->addWidget(blackCountLabel);
    infoLayout->addWidget(whiteCountLabel);
    infoLayout->addSpacing(20);
    infoLayout->addWidget(backToMenuButton);
    infoLayout->addStretch();

    mainLayout->addWidget(infoWidget);

    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(10);
    mainContainer->setLayout(mainLayout);

    int totalWidth = boardSize * cellSize + 160;
    int totalHeight = boardSize * cellSize + 10;
    setFixedSize(totalWidth, totalHeight);

    connect(backToMenuButton, &QPushButton::clicked, this, [=]() {
        emit backToMainRequested();
    });

    connect(undoButton, &QPushButton::clicked, this, &gameboardwidget::undoLastMove);

    timeLabel1 = new QLabel("Black: --", this);
    timeLabel2 = new QLabel("White: --", this);
    infoLayout->addWidget(timeLabel1);
    infoLayout->addWidget(timeLabel2);

    initializeTimers();

    if (settings.timeLimitEnabled) {
        timeLabel1->show();
        timeLabel2->show();
    } else {
        timeLabel1->hide();
        timeLabel2->hide();
    }

    updateStatus();

    saveCurrentTurnState(QPoint(-1, -1), currentPlayerBlack ? 1:2);
    saveButton = new QPushButton("저장하기", infoWidget);

    connect(saveButton, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(this, "저장할 파일 선택", "", "JSON 파일 (*.json)");
        if (!fileName.isEmpty()) {
            if (!fileName.endsWith(".json"))
                fileName += ".json";
            saveGameToJson(fileName);
        }
    });

    infoLayout->addWidget(saveButton);
}


gameboardwidget::~gameboardwidget()
{
    delete ui;
}

void gameboardwidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            QRect rect(col * cellSize, row * cellSize, cellSize, cellSize);
            painter.setPen(QPen(Qt::black, 1));
            painter.drawRect(rect);
        }
    }

    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            if (boardData[row][col] != 0) {
                int centerX = col * cellSize + cellSize / 2;
                int centerY = row * cellSize + cellSize / 2;
                int radius = cellSize / 2 - 6;

                if (boardData[row][col] == 1)
                    painter.setBrush(Qt::black);
                else
                    painter.setBrush(Qt::white);

                painter.setPen(Qt::NoPen);
                painter.drawEllipse(QPoint(centerX, centerY), radius, radius);
            }
        }
    }

    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            if (boardData[row][col] == 3) {
                int x = col * cellSize + 6;
                int y = row * cellSize + 6;
                painter.setBrush(Qt::gray);
                painter.drawRect(x, y, cellSize - 12, cellSize - 12);
            }
        }
    }

    if (!replayMode) {
        painter.setBrush(QColor(255, 105, 180, 100));
        painter.setPen(Qt::NoPen);
        for (const QPoint& p : validMoves) {
            int centerX = p.y() * cellSize + cellSize / 2;
            int centerY = p.x() * cellSize + cellSize / 2;
            int radius = 8;
            painter.drawEllipse(QPoint(centerX, centerY), radius, radius);
        }
    }

    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            if (boardData[row][col] == 3) {
                int x = col * cellSize + 6;
                int y = row * cellSize + 6;
                painter.setBrush(Qt::gray);
                painter.drawRect(x, y, cellSize - 12, cellSize - 12);
            }
        }
    }

    if (replayMode && lastMovePosition != QPoint(-1, -1)) {
        int x = lastMovePosition.y() * cellSize + 4;
        int y = lastMovePosition.x() * cellSize + 4;
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(QColor(255, 0, 0), 2));
        painter.drawRect(x, y, cellSize - 8, cellSize - 8);
    }

    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(14);
    painter.setFont(font);
}

void gameboardwidget::mousePressEvent(QMouseEvent *event)
{
    if (replayMode) {
        return;
    }

    QPoint pos = event->pos();
    int col = pos.x() / cellSize;
    int row = pos.y() / cellSize;

    if (row >= 0 && row < boardSize && col >= 0 && col < boardSize) {
        QPoint clickedPoint(row, col);

        if (validMoves.contains(clickedPoint)) {
            placeStone(row, col);
        }
    }
}

bool gameboardwidget::canPlaceAt(int row, int col, int player)
{
    if (boardData[row][col] != 0) return false;

    int opponent = (player == 1) ? 2 : 1;

    for (const QPoint& dir : directions) {
        int x = row + dir.x();
        int y = col + dir.y();
        bool foundOpponent = false;

        while (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
            if (boardData[x][y] == opponent) {
                foundOpponent = true;
            } else if (boardData[x][y] == player) {
                if (foundOpponent) return true;
                else break;
            } else {
                break;
            }
            x += dir.x();
            y += dir.y();
        }
    }
    return false;
}

void gameboardwidget::updateValidMoves()
{
    validMoves.clear();
    if (replayMode) {
        update();
        return;
    }
    int player = currentPlayerBlack ? 1 : 2;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (canPlaceAt(i, j, player)) {
                validMoves.append(QPoint(i, j));
            }
        }
    }
    update();
}

void gameboardwidget::flipStones(int row, int col, int player)
{
    int opponent = (player == 1) ? 2 : 1;

    for (const QPoint& dir : directions) {
        QVector<QPoint> stonesToFlip;
        int x = row + dir.x();
        int y = col + dir.y();

        while (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
            if (boardData[x][y] == opponent) {
                stonesToFlip.append(QPoint(x, y));
            } else if (boardData[x][y] == player) {
                for (const QPoint& p : stonesToFlip)
                    boardData[p.x()][p.y()] = player;
                break;
            } else {
                break;
            }
            x += dir.x();
            y += dir.y();
        }
    }
}

void gameboardwidget::placeStone(int row, int col)
{
    int player = currentPlayerBlack ? 1 : 2;
    boardData[row][col] = player;

    flipStones(row, col, player);

    saveCurrentTurnState(QPoint(row, col), player);

    if (settings.timeLimitEnabled) {
        if (currentPlayerBlack) {
            player1Timer->stop();
            remainingTime1 += settings.increment;
            timeLabel1->setText(QString("Black: %1s").arg(remainingTime1));
            player2Timer->start(1000);
        } else {
            player2Timer->stop();
            remainingTime2 += settings.increment;
            timeLabel2->setText(QString("White: %1s").arg(remainingTime2));
            player1Timer->start(1000);
        }
    }

    switchPlayer();

    updateValidMoves();

    if (validMoves.isEmpty()) {
        switchPlayer();
        updateValidMoves();

        if (validMoves.isEmpty()) {
            gameOver();
        } else {
            QMessageBox::information(this, "턴 패스", "현재 플레이어가 둘 곳이 없습니다.\n턴이 상대방으로 넘어갑니다.");
        }
    }

    updateStatus();
    update();
}


void gameboardwidget::saveCurrentTurnState(const QPoint& movePos, int player)
{
    GameTurnState turnState;
    QVector<QVector<int>> deepCopiedBoard;
    for (const auto& row : std::as_const(boardData)) {
        deepCopiedBoard.append(row);
    }
    turnState.boardState = deepCopiedBoard;
    turnState.currentPlayer = player;

    int blackCount = 0, whiteCount = 0;
    for (const QVector<int>& row : std::as_const(boardData)) {
        for (int cell : row) {
            if (cell == 1) blackCount++;
            else if (cell == 2) whiteCount++;
        }
    }
    turnState.blackCount = blackCount;
    turnState.whiteCount = whiteCount;

    turnState.movePosition = movePos;

    gameHistory.append(turnState);
    currentTurnIndex = gameHistory.size() - 1;

    qDebug() << "turn number : " << gameHistory.size();
}

void gameboardwidget::switchPlayer()
{
    currentPlayerBlack = !currentPlayerBlack;
    currentPlayer = currentPlayerBlack ? Player::Black : Player::White;
}

bool gameboardwidget::hasValidMoves(int player)
{
    for (int i = 0; i < boardSize; ++i)
        for (int j = 0; j < boardSize; ++j)
            if (canPlaceAt(i, j, player))
                return true;
    return false;
}

void gameboardwidget::gameOver()
{
    int blackCount = 0;
    int whiteCount = 0;

    for (int i = 0; i < boardSize; ++i)
        for (int j = 0; j < boardSize; ++j) {
            if (boardData[i][j] == 1) blackCount++;
            else if (boardData[i][j] == 2) whiteCount++;
        }

    QString result;
    if (blackCount > whiteCount)
        result = "흑돌 승리!";
    else if (whiteCount > blackCount)
        result = "백돌 승리!";
    else
        result = "무승부!";

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "게임 종료",
        QString("흑돌: %1, 백돌: %2\n%3\n\n리플레이를 저장하시겠습니까?").arg(blackCount).arg(whiteCount).arg(result),
        QMessageBox::Yes | QMessageBox::No
        );

    qDebug() << "reply button pressed:" << reply;

    if (reply == QMessageBox::Yes) {
        emit replayAccepted();
    } else {
        emit replayDenied();
        return;
    }

    updateStatus();
}

void gameboardwidget::placeObstacles(int count)
{
    int placed = 0;
    int maxAttempts = 10000;
    int mid = boardSize / 2;

    auto isCenter = [&](int r, int c) {
        return (r == mid || r == mid - 1) && (c == mid || c == mid - 1);
    };

    while (placed < count / 2 && maxAttempts-- > 0) {
        int row = QRandomGenerator::global()->bounded(boardSize);
        int col = QRandomGenerator::global()->bounded(boardSize);
        int mirrorCol = boardSize - col - 1;

        if (isCenter(row, col) || isCenter(row, mirrorCol))
            continue;

        if (boardData[row][col] == 0 && boardData[row][mirrorCol] == 0) {
            boardData[row][col] = 3;
            boardData[row][mirrorCol] = 3;
            placed++;
        }
    }
}

void gameboardwidget::updateStatus()
{
    int blackCount = 0, whiteCount = 0;

    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (boardData[i][j] == 1) ++blackCount;
            else if (boardData[i][j] == 2) ++whiteCount;
        }
    }

    blackCountLabel->setText(QString("흑: %1개").arg(blackCount));
    whiteCountLabel->setText(QString("백: %1개").arg(whiteCount));

    if (currentPlayerBlack) {
        turnLabel->setText("● 흑 턴");
        turnLabel->setStyleSheet("background-color: black; color: white; font-weight: bold;");
    } else {
        turnLabel->setText("○ 백 턴");
        turnLabel->setStyleSheet("background-color: white; color: black; font-weight: bold;");
    }

    if (undoButton)
        undoButton->setEnabled(gameHistory.size() > 1 && !replayMode);
}

void gameboardwidget::setReplayMode(bool enabled)
{
    if (enabled && !replaySaved) {
        QMessageBox::warning(this, "리플레이 불가", "저장된 리플레이가 없습니다.");
        return;
    }

    replayMode = enabled;

    if (undoButton)
        undoButton->setDisabled(replayMode);

    if (replayMode) {
        validMoves.clear();
        turnLabel->hide();
        blackCountLabel->hide();
        whiteCountLabel->hide();

        loadTurn(0);
    } else {
        turnLabel->show();
        blackCountLabel->show();
        whiteCountLabel->show();

        updateValidMoves();
    }

    update();
}


void gameboardwidget::loadTurn(int turnIndex)
{
    if (turnIndex < 0 || turnIndex >= gameHistory.size())
        return;

    boardData = gameHistory[turnIndex].boardState;

    lastMovePosition = gameHistory[turnIndex].movePosition;

    currentPlayerBlack = (gameHistory[turnIndex].currentPlayer == 1);

    validMoves.clear();

    updateStatus();
    update();
}


QVector<GameTurnState> gameboardwidget::getGameHistory() const
{
    return gameHistory;
}

int gameboardwidget::getBoardSize() const { return boardSize; }
int gameboardwidget::getCurrentPlayer() const { return currentPlayerBlack; }

void gameboardwidget::hideBackToMenuButton()
{
    if (backToMenuButton)
    {
        backToMenuButton->hide();
    }
}

void gameboardwidget::clearBoard()
{
    for (int y = 0; y < boardSize; ++y) {
        for (int x = 0; x < boardSize; ++x) {
            boardData[y][x] = 0;
        }
    }

    lastMovePosition = QPoint(-1, -1);
}

void gameboardwidget::loadTurnState(const GameTurnState& state)
{
    boardData = state.boardState;
    currentPlayerBlack =(state.currentPlayer == 1);
    lastMovePosition = state.movePosition;


    update();
}

void gameboardwidget::initializeTimers() {
    if (!settings.timeLimitEnabled)
        return;

    remainingTime1 = settings.baseTime;
    remainingTime2 = settings.baseTime;

    player1Timer = new QTimer(this);
    player2Timer = new QTimer(this);

    connect(player1Timer, &QTimer::timeout, this, [=]() {
        remainingTime1--;
        timeLabel1->setText(QString("Black: %1s").arg(remainingTime1));
        qDebug() << "initializing timers : " << settings.timeLimitEnabled;
        qDebug() << "Initial time: Black =" << remainingTime1 << "White =" << remainingTime2;
        if (remainingTime1 <= 0) {
            player1Timer->stop();
            QMessageBox::information(this, "시간 초과", "흑 플레이어 시간 초과! 패배 처리됩니다.");
        }
    });

    connect(player2Timer, &QTimer::timeout, this, [=]() {
        remainingTime2--;
        timeLabel2->setText(QString("White: %1s").arg(remainingTime2));
        if (remainingTime2 <= 0) {
            player2Timer->stop();
            QMessageBox::information(this, "시간 초과", "백 플레이어 시간 초과! 패배 처리됩니다.");
        }
    });

    if (currentPlayerBlack)
        player1Timer->start(1000);
    else
        player2Timer->start(1000);
}

void gameboardwidget::setGameSettings(const GameSettings& s) { settings = s; }

void gameboardwidget::resetTimers()
{
    if (!settings.timeLimitEnabled)
        return;

    remainingTime1 = settings.baseTime;
    remainingTime2 = settings.baseTime;

    timeLabel1->setText(QString("Black: %1s").arg(remainingTime1));
    timeLabel2->setText(QString("White: %1s").arg(remainingTime2));

    player1Timer->stop();
    player2Timer->stop();

    if (currentPlayerBlack)
        player1Timer->start(1000);
    else
        player2Timer->start(1000);
}

void gameboardwidget::undoLastMove()
{
    if (replayMode || gameHistory.size() < 2)
        return;

    gameHistory.removeLast();
    currentTurnIndex = gameHistory.size() - 1;

    const GameTurnState& previousState = gameHistory[currentTurnIndex];
    loadTurnState(previousState);

    currentPlayerBlack = (previousState.currentPlayer == 1);
    currentPlayer = currentPlayerBlack ? Player::Black : Player::White;

    updateStatus();
    updateValidMoves();

    if (settings.timeLimitEnabled) {
        player1Timer->stop();
        player2Timer->stop();

        if (currentPlayerBlack) {
            player1Timer->start(1000);
        } else {
            player2Timer->start(1000);
        }
    }
}

void gameboardwidget::setBoardState(const QVector<QVector<int>>& newBoard)
{
    boardState = newBoard;
    update();
}

void gameboardwidget::setObstacles(const QVector<QPoint>& newObstacles)
{
    obstacles = newObstacles;
    update();
}

void gameboardwidget::setTurn(int turn)
{
    currentTurn = turn;
    update();
}

void gameboardwidget::saveGameToJson(const QString& filePath)
{
    QJsonObject root;

    root["boardSize"] = boardSize;
    root["currentPlayer"] = currentPlayerBlack ? 1 : 2;

    QJsonArray boardArray;
    for (const QVector<int>& row : boardData) {
        QJsonArray jsonRow;
        for (int cell : row)
            jsonRow.append(cell);
        boardArray.append(jsonRow);
    }
    root["board"] = boardArray;

    QJsonArray historyArray;
    for (const GameTurnState& state : gameHistory) {
        QJsonObject turnObj;

        QJsonArray boardRows;
        for (const QVector<int>& row : state.boardState) {
            QJsonArray jsonRow;
            for (int val : row)
                jsonRow.append(val);
            boardRows.append(jsonRow);
        }
        turnObj["boardState"] = boardRows;
        turnObj["currentPlayer"] = state.currentPlayer;
        turnObj["blackCount"] = state.blackCount;
        turnObj["whiteCount"] = state.whiteCount;

        QJsonObject move;
        move["x"] = state.movePosition.x();
        move["y"] = state.movePosition.y();
        turnObj["movePosition"] = move;

        historyArray.append(turnObj);
    }
    root["history"] = historyArray;

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
        file.close();
        QMessageBox::information(this, "저장 완료", "게임이 성공적으로 저장되었습니다.");
    } else {
        QMessageBox::warning(this, "저장 실패", "파일을 열 수 없습니다.");
    }
}
