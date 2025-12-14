#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "replaywidget.h"
#include "ui_replaywidget.h"


replaywidget::replaywidget(int boardSize, int currentPlayer, GameSettings settings, QWidget *parent)
    : QWidget(parent), ui(new Ui::replaywidget), currentTurnIndex(0)
{
    ui->setupUi(this);

    replayBoard = new gameboardwidget(boardSize, currentPlayer, true, settings, this);
    replayBoard->setReplayMode(true);

    replayBoard->hideBackToMenuButton();

    nextTurnButton = new QPushButton("다음 턴", this);
    prevTurnButton = new QPushButton("이전 턴", this);

    turnSlider = new QSlider(Qt::Horizontal, this);
    turnSlider->setMinimum(0);
    turnSlider->setValue(0);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 20, 10, 10);
    mainLayout->setSpacing(10);

    mainLayout->addSpacing(10);
    mainLayout->addWidget(replayBoard);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(prevTurnButton);
    buttonLayout->addWidget(nextTurnButton);
    mainLayout->addLayout(buttonLayout);

    mainLayout->addWidget(turnSlider);

    blackCountLabel = new QLabel("흑돌: 0", this);
    whiteCountLabel = new QLabel("백돌: 0", this);

    QHBoxLayout* countLayout = new QHBoxLayout();
    countLayout->addWidget(blackCountLabel);
    countLayout->addWidget(whiteCountLabel);

    mainLayout->addLayout(countLayout);

    QPushButton* backButton = new QPushButton("메인화면으로", this);
    mainLayout->addWidget(backButton);

    setLayout(mainLayout);

    connect(nextTurnButton, &QPushButton::clicked, this, &replaywidget::onNextTurn);
    connect(prevTurnButton, &QPushButton::clicked, this, &replaywidget::onPrevTurn);

    connect(turnSlider, &QSlider::valueChanged, this, [=](int value){
        qDebug() << "슬라이더 변경됨: " << value;
        loadTurn(value);
    });

    connect(backButton, &QPushButton::clicked, this, [=]() {
        qDebug() << "메인화면 버튼 클릭됨 - backToMainRequested emit";
        emit backToMainRequested();
    });

    turnLabel = new QLabel(this);
    turnLabel->setText("흑돌 차례");

    countLayout->addWidget(turnLabel);

}

replaywidget::~replaywidget()
{
    delete ui;
}

void replaywidget::setGameHistory(const QVector<GameTurnState>& history)
{
    gameHistory = history;
    currentTurnIndex = 0;
    qDebug() << "리플레이 위젯에 전달된 게임 기록 개수:" << gameHistory.size();
    if (!gameHistory.isEmpty()) {
        turnSlider->setMaximum(gameHistory.size() - 1);
        turnSlider->setValue(0);
    } else {
        turnSlider->setMaximum(0);
        turnSlider->setValue(0);
    }

    updateReplayView();
}

void replaywidget::updateReplayView()
{
    qDebug() << "history size :" << gameHistory.size();
    if (gameHistory.isEmpty())
    {
        qDebug() << "replay is empty!";
        return;
    }

    qDebug() << "current turn" << currentTurnIndex;
    const auto& turnState = gameHistory[currentTurnIndex];
    replayBoard->loadTurnState(turnState);
    if (turnSlider->value() != currentTurnIndex)
        turnSlider->setValue(currentTurnIndex);

    int blackCount = 0;
    int whiteCount = 0;
    prevTurnButton->setEnabled(currentTurnIndex > 0);
    nextTurnButton->setEnabled(currentTurnIndex < gameHistory.size() - 1);

    for (int i = 0; i < turnState.boardState.size(); ++i) {
        for (int j = 0; j < turnState.boardState[i].size(); ++j) {
            if (turnState.boardState[i][j] == 1)
                ++blackCount;
            else if (turnState.boardState[i][j] == 2)
                ++whiteCount;
        }
    }

    blackCountLabel->setText(QString("흑돌: %1").arg(blackCount));
    whiteCountLabel->setText(QString("백돌: %1").arg(whiteCount));

    if (turnState.currentPlayer == 1) {
        turnLabel->setText("흑돌 차례");
    } else if (turnState.currentPlayer == 2) {
        turnLabel->setText("백돌 차례");
    } else {
        turnLabel->setText("알 수 없는 차례");
    }
}

void replaywidget::onNextTurn()
{
    if (currentTurnIndex < gameHistory.size() - 1) {
        ++currentTurnIndex;
        updateReplayView();
    }
}

void replaywidget::onPrevTurn()
{
    if (currentTurnIndex > 0) {
        --currentTurnIndex;
        updateReplayView();
    }
}

QSlider* replaywidget::getTurnSlider() { return turnSlider; }

void replaywidget::loadTurn(int turnIndex)
{
    if (turnIndex < 0 || turnIndex >= gameHistory.size()) {
        qDebug() << "잘못된 턴 인덱스:" << turnIndex;
        return;
    }

    currentTurnIndex = turnIndex;
    updateReplayView();
}
