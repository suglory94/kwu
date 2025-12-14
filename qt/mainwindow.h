#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>

#include "gamesettingdialog.h"
#include "gameboardwidget.h"
#include "replaywidget.h"
#include "Move.h"

struct GameState {
    int turn;
    int x, y;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    gameboardwidget* currentBoard = nullptr;
    replaywidget* replayWidget = nullptr;

    int size;
    bool blackFirst;

    bool replayAllowed = true;

    int currentTurn = 0;
    int blackCount = 0;
    int whiteCount = 0;

    QVector<QVector<int>> board;
    QVector<QPoint> obstacleList;
    QVector<GameState> gameHistorym;

    void updateStatus();

private slots:
    void on_replaybutton_clicked();
    void on_endbutton_clicked();
    void on_newbutton_clicked();
    void on_continuebutton_clicked();
    void askToSaveReplay();
    void loadGameFromFile();
};
#endif
