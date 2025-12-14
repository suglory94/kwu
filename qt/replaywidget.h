#ifndef REPLAYWIDGET_H
#define REPLAYWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QSlider>
#include "gameboardwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui{
class replaywidget;
}
QT_END_NAMESPACE

class replaywidget : public QWidget
{
    Q_OBJECT

public:
    explicit replaywidget(int boardSize, int currentPlayer, GameSettings settings, QWidget *parent = nullptr);
    ~replaywidget();

    void setGameHistory(const QVector<GameTurnState>& history);
    void loadTurn(int turn);

    QSlider* getTurnSlider();

private slots:
    void onNextTurn();
    void onPrevTurn();

private:
    Ui::replaywidget *ui;

    gameboardwidget* replayBoard;
    QVector<GameTurnState> gameHistory;
    int currentTurnIndex;

    QPushButton* nextTurnButton;
    QPushButton* prevTurnButton;

    void updateReplayView();

    QLabel* blackCountLabel;
    QLabel* whiteCountLabel;

    QLabel* turnLabel;
public:
    QSlider* turnSlider;

signals:
    void backToMainRequested();
};
#endif
