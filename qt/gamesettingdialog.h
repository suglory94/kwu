#ifndef GAMESETTINGDIALOG_H
#define GAMESETTINGDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QSpinBox>

struct GameSettings {
    bool timeLimitEnabled;
    int baseTime;
    int increment;
};

namespace Ui {
class gamesettingdialog;
}

class gamesettingdialog : public QDialog
{
    Q_OBJECT

public:
    explicit gamesettingdialog(QWidget *parent = nullptr);
    ~gamesettingdialog();

    int boardSize() const;
    int obstacleCount() const;
    QString firstPlayer() const;

    bool isTimeLimitEnabled() const;
    int getInitialTime() const;
    int getIncrementTime() const;

    GameSettings getSettings() const;

private:
    Ui::gamesettingdialog *ui;

    QCheckBox *enableTimeLimitCheckBox;
    QSpinBox *initialTimeSpinBox;
    QSpinBox *incrementTimeSpinBox;
};

#endif
