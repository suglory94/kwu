#ifndef GAMEBOARDWIDGET_H
#define GAMEBOARDWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include "Move.h"
#include "gamesettingdialog.h"

enum class Player { Black, White };

namespace Ui {
class gameboardwidget;
}

class gameboardwidget : public QWidget
{
    Q_OBJECT

public:
    explicit gameboardwidget(int size, int obstacleCount, bool blackFirst, const GameSettings& settings, QWidget *parent = nullptr);
    ~gameboardwidget();

    void setReplayMode(bool enabled);

    QVector<GameTurnState> getGameHistory() const;

    void setBoardState(const QVector<QVector<int>>& newBoard);
    void setObstacles(const QVector<QPoint>& newObstacles);
    void setTurn(int turn);

signals:
    void backToMainRequested();
    void replayDenied();
    void replayAccepted();

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::gameboardwidget *ui;

    int boardSize;
    int cellSize;
    bool currentPlayerBlack;

    QVector<QVector<int>> boardData;
    QVector<QPoint> validMoves;

    QLabel* turnLabel;
    QLabel* blackCountLabel;
    QLabel* whiteCountLabel;
    QPushButton* backToMenuButton;
    QPushButton* undoButton = nullptr;
public:
    QSlider* turnSlider;

private:
    bool replayMode = false;
    QVector<GameTurnState> gameHistory;
    int currentTurnIndex;
    QPoint lastMovePosition;
    bool replaySaved = true;

    void updateValidMoves();
    bool canPlaceAt(int row, int col, int player);
    void flipStones(int row, int col, int player);
    void placeStone(int row, int col);
    void switchPlayer();
    bool hasValidMoves(int player);
    void gameOver();
    void placeObstacles(int count);
    void updateStatus();
    void saveCurrentTurnState(const QPoint& movePos, int player);
public:
    void loadTurnState(const GameTurnState& state);
    void loadTurn(int turnIndex);
    int getBoardSize() const;
    int getCurrentPlayer() const;
    void hideBackToMenuButton();
    void clearBoard();

public:
    void initializeTimers();
    void setGameSettings(const GameSettings& s);
    void resetTimers();

private:
    QTimer* player1Timer;
    QTimer* player2Timer;
    int remainingTime1;
    int remainingTime2;


    GameSettings settings;
    QLabel* timeLabel1;
    QLabel* timeLabel2;

private:
    int totalTime = 0;

    Player currentPlayer = Player::Black;

    QVector<QVector<int>> boardState;
    QVector<QPoint> obstacles;
    int currentTurn;

    QPushButton* saveButton;
    void saveGameToJson(const QString& filePath);

private slots:
    void undoLastMove();
};

#endif
