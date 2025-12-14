#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QGridLayout>
#include <QDebug>
#include <QRandomGenerator>
#include "gamesettingdialog.h"
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    ui->replaybutton->setEnabled(true);

    ui->saveButton->hide();

    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::loadGameFromFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newbutton_clicked()
{
    gamesettingdialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        size = dialog.boardSize();
        board.resize(size);
        for (int i = 0; i < size; ++i) {
            board[i].resize(size);
        }
        int obstacle = dialog.obstacleCount();
        QString first = dialog.firstPlayer();

        blackFirst = true;
        if (first == "백") {
            blackFirst = false;
        } else if (first == "무작위") {
            blackFirst = QRandomGenerator::global()->bounded(2) == 0;
        }

        if (currentBoard)
        {
            currentBoard->setParent(nullptr);
            delete currentBoard;
            currentBoard = nullptr;
        }

        GameSettings settings = dialog.getSettings();
        currentBoard = new gameboardwidget(size, obstacle, blackFirst, settings, this);

        connect(currentBoard, &gameboardwidget::replayAccepted, this, [=]() {
            replayAllowed = true;
            ui->replaybutton->setEnabled(true);
        });

        connect(currentBoard, &gameboardwidget::replayDenied, this, [=]() {
            replayAllowed = false;
            ui->replaybutton->setEnabled(false);
        });

        QLayout* oldLayout = ui->boardContainer->layout();
        if (oldLayout)
        {
            QLayoutItem* item;
            while ((item = oldLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
            delete oldLayout;
        }

        QVBoxLayout* layout = new QVBoxLayout(ui->boardContainer);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(currentBoard);

        connect(currentBoard, &gameboardwidget::backToMainRequested, this, [=]() {
            ui->stackedWidget->setCurrentIndex(0);
        });

        ui->stackedWidget->setCurrentIndex(1);

        qDebug() << "보드 크기:" << size;
        qDebug() << "장애물 개수:" << obstacle;
        qDebug() << "선공:" << (blackFirst ? "흑" : "백");

        connect(currentBoard, &gameboardwidget::replayAccepted, this, [=]() {
            replayAllowed = true;
        });

        connect(currentBoard, &gameboardwidget::replayDenied, this, [=]() {
            replayAllowed = false;
            qDebug() << "리플레이가 비활성화되었습니다.";
        });
    }
}

void MainWindow::on_continuebutton_clicked()
{
    if (currentBoard)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        QMessageBox::information(this, "안내", "진행 중인 게임이 없습니다.");
    }
}

void MainWindow::on_replaybutton_clicked()
{
    if (!currentBoard || !replayAllowed)
    {
        QMessageBox::information(this, "안내", "진행 중인 게임이 없습니다.");
        return;
    }

    if (!replayAllowed)
    {
        qDebug() << "no replay";
        return;
    }
    gamesettingdialog dialog(this);
    GameSettings settings = dialog.getSettings();
    if (!replayWidget) {
        replayWidget = new replaywidget(size, blackFirst, settings, this);

        connect(replayWidget->getTurnSlider(), &QSlider::valueChanged, replayWidget, [=](int value)
                {
            replayWidget->loadTurn(value);
        });
        if (currentBoard) {
            replayWidget->setGameHistory(currentBoard->getGameHistory());
            auto history = currentBoard->getGameHistory();
            qDebug() << "게임 기록 개수:" << history.size();
            replayWidget->setGameHistory(history);
        }
        if (ui->stackedWidget->indexOf(replayWidget) == -1) {
            ui->stackedWidget->addWidget(replayWidget);
        }
        connect(replayWidget, &replaywidget::backToMainRequested, this, [=]() {
            ui->stackedWidget->setCurrentIndex(0);
        });
    } else {
        if (currentBoard) {
            auto history = currentBoard->getGameHistory();
            qDebug() << "게임 기록 개수:" << history.size();
            for (const auto& move : history) {
                qDebug() << "Move position:" << move.movePosition
                         << "Player:" << move.currentPlayer
                         << "Black count:" << move.blackCount
                         << "White count:" << move.whiteCount;

                if (!move.boardState.isEmpty()) {
                    QString rowStr;
                    for (int val : move.boardState[0]) {
                        rowStr += QString::number(val) + " ";
                    }
                    qDebug() << "Board first row:" << rowStr;
                }
            }
            replayWidget->setGameHistory(history);
        }
    }
    ui->stackedWidget->setCurrentWidget(replayWidget);
}


void MainWindow::on_endbutton_clicked()
{
    QApplication::quit();
}

void MainWindow::askToSaveReplay()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "리플레이 저장", "리플레이를 저장하시겠습니까?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        replayAllowed = false;
    } else {
        replayAllowed = true;
    }
}

void MainWindow::loadGameFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "게임 불러오기", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject gameState = doc.object();

    QJsonArray boardArray = gameState["board"].toArray();
    int boardSize = boardArray.size();

    size = boardSize;
    board.resize(size);
    for (int i = 0; i < size; ++i) {
        board[i].resize(size);
    }

    for (int i = 0; i < boardSize; ++i) {
        QJsonArray row = boardArray[i].toArray();
        int rowSize = row.size();

        for (int j = 0; j < rowSize; ++j) {
            board[i][j] = row[j].toInt();
        }
    }

    currentTurn = gameState["turn"].toInt();
    blackCount = gameState["blackCount"].toInt();
    whiteCount = gameState["whiteCount"].toInt();

    obstacleList.clear();
    QJsonArray obstacleArray = gameState["obstacles"].toArray();
    for (auto val : obstacleArray) {
        QJsonArray pos = val.toArray();
        obstacleList.push_back(QPoint(pos[0].toInt(), pos[1].toInt()));
    }

    gameHistorym.clear();
    QJsonArray historyArray = gameState["history"].toArray();
    for (auto val : historyArray) {
        QJsonObject step = val.toObject();
        GameState s;
        s.turn = step["turn"].toInt();
        QJsonArray pos = step["position"].toArray();
        s.x = pos[0].toInt();
        s.y = pos[1].toInt();
        gameHistorym.push_back(s);
    }

    if (!currentBoard) {
        GameSettings settings;
        currentBoard = new gameboardwidget(size, obstacleList.size(), true, settings, this);
        QVBoxLayout* layout = new QVBoxLayout(ui->boardContainer);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(currentBoard);

        connect(currentBoard, &gameboardwidget::backToMainRequested, this, [=]() {
            ui->stackedWidget->setCurrentIndex(0);
        });

        ui->stackedWidget->setCurrentIndex(1);
    }


    if (currentBoard) {
        currentBoard->setParent(nullptr);
        currentBoard->deleteLater();
        currentBoard = nullptr;
    }


    GameSettings settings;
    currentBoard = new gameboardwidget(size, obstacleList.size(), true, settings, this);

    QVBoxLayout* layout = new QVBoxLayout(ui->boardContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(currentBoard);

    connect(currentBoard, &gameboardwidget::backToMainRequested, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    ui->stackedWidget->setCurrentIndex(1);

    currentBoard->setBoardState(board);
    currentBoard->setObstacles(obstacleList);
    currentBoard->setTurn(currentTurn);

    update();
}
