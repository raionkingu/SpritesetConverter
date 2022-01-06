#pragma once

#include "SpritesetWidget.hpp"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOpen();
    void onClose();
    void onSave();
    void onSaveAs();
    void onQuit();

    void onSizeFactorChanged(int index);

    void onChangeColorKey();
    void onResetColorKey();
    void onColorKeyChanged(QColor ck);

    void enableItems(bool enabled);
//    bool askForSave();

private:
    void redrawSprite();

    Ui::MainWindow *ui;
    SpritesetWidget *spriteset_widget;

    QString current_filename;
    QColor color_key;
};
