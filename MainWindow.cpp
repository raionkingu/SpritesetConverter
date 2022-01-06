#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    spriteset_widget(nullptr),
    current_filename(),
    color_key()
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(ui->horizontalLayout);

    ui->sizeFactorComboBox->addItem(tr("0.25x"));
    ui->sizeFactorComboBox->addItem(tr("0.5x"));
    ui->sizeFactorComboBox->addItem(tr("1x"));
    ui->sizeFactorComboBox->addItem(tr("2x"));
    ui->sizeFactorComboBox->addItem(tr("4x"));

    QPixmap ck(64, 24);
    ck.fill(Qt::transparent);
    ui->colorKeyButton->setIcon(QIcon(ck));

    ui->scrollArea->setBackgroundRole(QPalette::Dark);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpen);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::onClose);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSave);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAs);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::onQuit);

    connect(ui->action025x, &QAction::triggered, [=] { onSizeFactorChanged(0); });
    connect(ui->action05x, &QAction::triggered, [=] { onSizeFactorChanged(1); });
    connect(ui->action1x, &QAction::triggered, [=] { onSizeFactorChanged(2); });
    connect(ui->action2x, &QAction::triggered, [=] { onSizeFactorChanged(3); });
    connect(ui->action4x, &QAction::triggered, [=] { onSizeFactorChanged(4); });
    connect(ui->sizeFactorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int i) { onSizeFactorChanged(i); });

    connect(ui->actionChangeColorKey, &QAction::triggered, this, &MainWindow::onChangeColorKey);
    connect(ui->actionResetColorKey, &QAction::triggered, this, &MainWindow::onResetColorKey);
    connect(ui->colorKeyButton, &QPushButton::clicked, this, &MainWindow::onChangeColorKey);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::redrawSprite()
{
    double factor;
    switch (ui->sizeFactorComboBox->currentIndex())
    {
    case 0:
        factor = 0.25;
        break;
    case 1:
        factor = 0.5;
        break;
    case 2:
        factor = 1;
        break;
    case 3:
        factor = 2;
        break;
    case 4:
        factor = 4;
        break;
    default:
        throw std::logic_error("invalid size factor index");
    }

    spriteset_widget->redrawSprite(factor, color_key);
}

void MainWindow::onOpen()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Choose a sprite set"),
        QString(),
        tr("Image files (*.png *.bmp *.jpg)")
    );

    if (!filename.isEmpty())
    {
        onClose();
        enableItems(true);

        spriteset_widget = new SpritesetWidget(filename, ui->scrollArea);
        connect(spriteset_widget, &SpritesetWidget::colorKeySelected, this, &MainWindow::onColorKeyChanged);
        ui->scrollArea->setWidget(spriteset_widget);

        onSizeFactorChanged(2);
    }
}

void MainWindow::onClose()
{
    enableItems(false);

    current_filename.clear();
    color_key = QColor();

    if (spriteset_widget)
    {
        disconnect(spriteset_widget, &SpritesetWidget::colorKeySelected, this, &MainWindow::onColorKeyChanged);
        delete spriteset_widget;
        spriteset_widget = nullptr;
    }
}

void MainWindow::onSave()
{
    if (current_filename.isEmpty())
        onSaveAs();

    if (!current_filename.isEmpty())
        spriteset_widget->save(current_filename);
}

void MainWindow::onSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Choose a file to save into")
    );

    if (!filename.isEmpty())
    {
        current_filename = filename;
        onSave();
    }
}

void MainWindow::onQuit()
{
    qApp->exit();
}

void MainWindow::onSizeFactorChanged(int index)
{
//  ugly
    switch (index)
    {
    case 0:
        ui->action025x->setChecked(true);
        ui->action05x->setChecked(false);
        ui->action1x->setChecked(false);
        ui->action2x->setChecked(false);
        ui->action4x->setChecked(false);
        break;
    case 1:
        ui->action025x->setChecked(false);
        ui->action05x->setChecked(true);
        ui->action1x->setChecked(false);
        ui->action2x->setChecked(false);
        ui->action4x->setChecked(false);
        break;
    case 2:
        ui->action025x->setChecked(false);
        ui->action05x->setChecked(false);
        ui->action1x->setChecked(true);
        ui->action2x->setChecked(false);
        ui->action4x->setChecked(false);
        break;
    case 3:
        ui->action025x->setChecked(false);
        ui->action05x->setChecked(false);
        ui->action1x->setChecked(false);
        ui->action2x->setChecked(true);
        ui->action4x->setChecked(false);
        break;
    case 4:
        ui->action025x->setChecked(false);
        ui->action05x->setChecked(false);
        ui->action1x->setChecked(false);
        ui->action2x->setChecked(false);
        ui->action4x->setChecked(true);
        break;
    default:
        throw std::logic_error("invalid size factor index");
    }

    ui->sizeFactorComboBox->setCurrentIndex(index);
    redrawSprite();
}

void MainWindow::onChangeColorKey()
{
    QColor ck = QColorDialog::getColor(Qt::white, this, tr("Pick a color key"));

    if (ck.isValid())
    {
        color_key = ck;

        QPixmap img(64, 24);
        img.fill(ck);
        ui->colorKeyButton->setIcon(QIcon(img));

        redrawSprite();
    }
}

void MainWindow::onResetColorKey()
{
    color_key = QColor();

    QPixmap img(64, 24);
    img.fill(Qt::transparent);
    ui->colorKeyButton->setIcon(QIcon(img));

    redrawSprite();
}

void MainWindow::onColorKeyChanged(QColor ck)
{
    color_key = ck;

    QPixmap img(64, 24);
    img.fill(ck);
    ui->colorKeyButton->setIcon(QIcon(img));

    redrawSprite();
}

void MainWindow::enableItems(bool enabled)
{
    ui->actionClose->setEnabled(enabled);
    ui->actionSave->setEnabled(enabled);
    ui->actionSaveAs->setEnabled(enabled);

    ui->menuEdit->setEnabled(enabled);

    ui->action025x->setEnabled(enabled);
    ui->action05x->setEnabled(enabled);
    ui->action1x->setEnabled(enabled);
    ui->action2x->setEnabled(enabled);
    ui->action4x->setEnabled(enabled);

    ui->actionChangeColorKey->setEnabled(enabled);
    ui->actionResetColorKey->setEnabled(enabled);

    ui->sizeFactorComboBox->setEnabled(enabled);
    ui->colorKeyButton->setEnabled(enabled);
}
