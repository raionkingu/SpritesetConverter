#pragma once

#include <QWidget>
#include <QLabel>
#include <QImage>

class SpritesetWidget: public QWidget
{
    Q_OBJECT
public:
    SpritesetWidget(const QString &filename, QWidget *parent = nullptr);

    void redrawSprite(double factor, const QColor &ck);

    void save(const QString &filename) const;

signals:
    void colorKeySelected(QColor ck);

private:
    QLabel *background_label, *spriteset_label;
    QImage raw_spriteset, resized_spriteset;

    void mousePressEvent(QMouseEvent *event) override;
};
