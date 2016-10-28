#ifndef TEXTRECOGNIZER_H
#define TEXTRECOGNIZER_H

#include <QObject>

class TextRecognizer : public QObject
{
    Q_OBJECT
public:
    explicit TextRecognizer(QObject *parent = 0);
signals:

public slots:
};

#endif // TEXTRECOGNIZER_H
