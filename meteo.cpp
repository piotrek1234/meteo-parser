#include "meteo.h"

meteo::meteo()
{
    QUrl url("http://www.meteo.pl/um/metco/mgram_pict.php?ntype=0u&row=406&col=250&lang=pl");
    //QUrl url("http://www.meteo.pl/um/metco/mgram_pict.php?ntype=0u&fdate=2016062512&row=406&col=250&lang=pl");
    fd = new FileDownloader(url);
    connect(fd, SIGNAL(downloaded()), this, SLOT(refresh()));
}

void meteo::refresh()
{
    QImage meteogram;
    QImage cyfry(":/img/cyfry.bmp");

    meteogram.loadFromData(fd->downloadedData());
    double tempx0 = 71, tempxk = 560, tempy0=58, tempyk=134;
    double t0, tk;
    if(meteogram.width() == 630)
    {
        tempx0 = 71;
        tempxk = 560;
    }
    else    //540px
    {
        tempx0 = 63;
        tempxk = 475;
    }

    //podziałki temperatury
    int tempspan_px;
    QColor tempcol(255, 0, 0);
    QVector<double> temps;
    QVector<int> dividers;

    for(int i=tempy0; i<=tempyk; ++i)
    {
        int blacks = 0;
        for(int x=tempx0; x<=tempxk; ++x)
        {
            if(isBlack(meteogram.pixelColor(x, i)))
                blacks++;
        }
        if(blacks > 50)
            dividers.push_back(i);
    }
    if(dividers.length()>1)
        tempspan_px = dividers.at(1)-dividers.at(0);

    //progowanie dla odczytu liczb
    QImage aa = meteogram;
    for(int x=0; x<aa.width(); ++x)
    {
        for(int y=0; y<aa.height(); ++y)
        {
            if(aa.pixelColor(x, y).value() > 100)
                aa.setPixelColor(x, y, Qt::white);
            else
                aa.setPixelColor(x, y, Qt::black);
        }
    }

    //minimalna i maksymalna temperatura
    int x0;
    if(aa.width() == 630)
        x0=59;
    else
        x0=52;
    int g = recognizeNumber(aa, dividers.at(0), x0);
    int h = recognizeNumber(aa, dividers.at(1), x0);

    tk = (dividers.at(0)-tempy0)*(g-h)/tempspan_px+g;
    t0 = tk - (tempyk-tempy0)*(g-h)/tempspan_px;

    for(int i=tempx0; i<=tempxk; ++i)
    {
        bool next = false;
        for(int y=tempy0; y<=tempyk; ++y)
        {
            if(meteogram.pixelColor(i, y) == tempcol)
            {
                temps.push_back((tempyk-y)*(tk-t0)/(tempyk-tempy0)-t0);
                next = true;
                break;
            }
        }
        if(!next)
            temps.push_back(-100);
    }

    /*if(temps.at(0) == -100)
        temps[0] = temps.at(1);
    if(temps.at(temps.count()-1) == -100)
        temps[temps.count()-1] = temps.at(temps.count(-2));
    for(int i=1; i<temps.count()-1; ++i)
    {
        if(temps.at(i) == -100)
            temps[i] = (temps.at(i-1)+temps.at(i+1))/2;
    }*/

    nowy = QImage(temps.count(), 120, QImage::Format_RGB32);
    nowy.fill(QColor(255, 255, 255));

    //for(int i=0; i<temps.count(); ++i)
    //    nowy.setPixelColor(i, 4*(9-temps.at(i)), QColor(255, 0, 0));
    QPixmap px = QPixmap::fromImage(nowy);
    QPainter p(&px);
    p.setPen(Qt::red);

    int x=0, y=4*(20-temps.at(0));
    for(int i=1; i<temps.count()-1; ++i)
    {
        if(temps.at(i) != -100)
        {
            p.drawLine(x, y, i, 4*(20-temps.at(i)));
            x=i, y=4*(20-temps.at(i));
        }
    }
    p.end();
    nowy = px.toImage();

    emit gotowe();
}

bool meteo::isBlack(QColor color)
{
    if(color.lightness() < 50)
                return true;
    return false;
}

int meteo::recognizeDigit(QImage &img, int x, int y)
{
    QImage pattern(":/img/cyfry.bmp");
    for(int i=0; i<10; ++i)
    {
        bool next;
        for(int a=0; a<5; ++a)
        {
            next=false;
            for(int b=0; b<8; ++b)
            {
                if(img.pixelColor(x+a, y+b) != pattern.pixel(i*5+a, b))
                {
                    next = true;
                    break;
                }
            }
            if(next)
                break;
        }
        if(!next)
            return i;
    }
    return -1;
}

double meteo::recognizeNumber(QImage &img, int line, int x0)
{
    //todo: ujemne
    int offsety=4, offsetx=0, multiplier=1, number=0;

    if(recognizeDigit(img, x0, line-offsety) == -1)
        offsety = 3;

    for(int i=0; i<4; ++i)
    {
        int digit = recognizeDigit(img, x0-offsetx, line-offsety);
        if(digit != -1)
            number += multiplier*digit;
        else
            break;
        offsetx += 6;
        multiplier *= 10;
    }

    return number;
}
