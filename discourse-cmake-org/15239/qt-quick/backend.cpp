#include <QApplication>

#include "backend.h"

Backend::Backend(QObject *parent) : QObject(parent) { }

void Backend::showAboutQt()
{
    QApplication::aboutQt();
}
