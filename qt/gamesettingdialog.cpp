#include "gamesettingdialog.h"
#include "ui_Gamesettingdialog.h"

gamesettingdialog::gamesettingdialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::gamesettingdialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->checkBoxTimeLimit, &QCheckBox::toggled, [=](bool checked) {
        ui->spinBoxBaseTime->setEnabled(checked);
        ui->spinBoxIncrement->setEnabled(checked);
    });
}

gamesettingdialog::~gamesettingdialog()
{
    delete ui;
}

int gamesettingdialog::boardSize() const {
    return ui->spinBox_2->value();
}

int gamesettingdialog::obstacleCount() const {
    return ui->spinBox->value();
}

QString gamesettingdialog::firstPlayer() const {
    return ui->comboBox->currentText();
}

GameSettings gamesettingdialog::getSettings() const {
    GameSettings settings;
    settings.timeLimitEnabled = ui->checkBoxTimeLimit->isChecked();
    settings.baseTime = ui->spinBoxBaseTime->value();
    settings.increment = ui->spinBoxIncrement->value();
    return settings;
}
