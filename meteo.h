#ifndef METEO_H
#define METEO_H

#include <QObject>
#include <QImage>
#include <QVector>
#include <QColor>
#include <QPainter>
#include <QPixmap>
#include "filedownloader.h"

class meteo : public QObject
{
    Q_OBJECT
public:
    meteo();
    QImage nowy;
private slots:
    void refresh();
private:
    FileDownloader *fd;
    bool isBlack(QColor color);
    int recognizeDigit(QImage& img, int x, int y);
    double recognizeNumber(QImage& img, int line, int x0);
signals:
    void gotowe();
};

#endif // METEO_H
