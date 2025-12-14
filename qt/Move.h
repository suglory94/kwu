#ifndef MOVE_H
#define MOVE_H

#include <QVector>
#include <QPoint>

struct GameTurnState {
    QVector<QVector<int>> boardState;
    QPoint movePosition;
    int currentPlayer;
    int blackCount;
    int whiteCount;
};

#endif
