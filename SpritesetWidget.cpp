#include "SpritesetWidget.hpp"

#include <QMouseEvent>

SpritesetWidget::SpritesetWidget(const QString &filename, QWidget *parent):
    QWidget(parent),
    background_label(new QLabel(this)),
    spriteset_label(new QLabel(this)),
    raw_spriteset(filename),
    resized_spriteset()
{
    redrawSprite(1, QColor());
}

static QPixmap create_background(const QSize &size)
{
    QImage bg(size, QImage::Format_RGB32);

    for (int i = 0; i < size.width(); ++i)
        for (int j = 0; j < size.height(); ++j)
            bg.setPixelColor(i, j, (((i/32)%2) ^ ((j/32)%2))? Qt::darkGray : Qt::gray);

    return QPixmap::fromImage(bg);
}

static QPixmap create_resized(const QImage &raw, QImage &resized, double factor, const QColor &ck)
{
    const int w = raw.width() * factor;
    const int h = raw.height() * factor;

    resized = QImage(w, h, QImage::Format_RGBA8888);
    resized.fill(Qt::transparent);

    for (int x = 0; x < w; ++x)
        for (int y = 0; y < h; ++y)
            if (!(ck.isValid() && raw.pixelColor(x/factor, y/factor) == ck))
                resized.setPixelColor(x, y, raw.pixelColor(x/factor, y/factor));

    return QPixmap::fromImage(resized);
}

void SpritesetWidget::redrawSprite(double factor, const QColor &ck)
{
    resize(raw_spriteset.size() * factor);

    QPixmap bg = create_background(raw_spriteset.size() * factor);
    background_label->setPixmap(bg);
    background_label->resize(bg.size());

    QPixmap img = create_resized(raw_spriteset, resized_spriteset, factor, ck);
    spriteset_label->setPixmap(img);
    spriteset_label->resize(img.size());
}

void SpritesetWidget::save(const QString &filename) const
{
    resized_spriteset.save(filename);
}

void SpritesetWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit colorKeySelected(resized_spriteset.pixelColor(event->x(), event->y()));
}
