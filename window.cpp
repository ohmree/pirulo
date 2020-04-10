#include "window.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QShowEvent>
#include <QTimer>

//! [0]
Window::Window()
{
    createActions();
    createTrayIcon();
    trayIcon->setIcon(QIcon(":/images/pirulo.png"));
    trayIcon->show();
    connect(QApplication::clipboard(), SIGNAL(dataChanged()),
            this, SLOT(clipboardChanged()));
}
//! [0]

void Window::createActions()
{
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void Window::setVisible([[maybe_unused]] bool visible)
{
    QDialog::setVisible(false);
}

void Window::clipboardChanged()
{
    // we need to ignore every other dataChanged() signal because
    // those signals are triggered by us calling QClipboard::setText()
    static bool ignoreSignal = false;

    if (ignoreSignal == false) {
        clipboardText = QApplication::clipboard()->text();

        // will trigger another dataChanged() signal
        // (after our method exits)
        QTimer::singleShot(50, this, SLOT(setClipboard()));

        // ignore the next dataChanged() signal
        ignoreSignal = true;
    }
    else {
        // We're ignoring this signal. Don't ignore the next signal.
        ignoreSignal = false;
    }
}

void Window::setClipboard()
{
    QApplication::clipboard()->setText(copypasta);
}

#endif
