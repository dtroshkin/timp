#ifndef TESTER_H
#define TESTER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include "networkmanager.h"

class Tester : public QObject {
    Q_OBJECT
public:
    explicit Tester(QObject *parent = nullptr);
private:
    NetworkManager* manager;
};

#endif
