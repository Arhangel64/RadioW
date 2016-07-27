#include "detailedview.h"

DetailedView::DetailedView(QWidget* parent):
    QWidget(parent),
    layout(new QGridLayout(this)),
    logArea(new QPlainTextEdit(this))
{
    setLayout(layout);
    logArea->setReadOnly(true);
    layout->addWidget(logArea, 0, 0, 1, 1);
}

void DetailedView::appendMessage(const QString& msg)
{
    logArea->appendPlainText(msg);
}

void DetailedView::clear()
{
    logArea->clear();
}

