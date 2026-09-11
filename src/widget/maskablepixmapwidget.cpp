/*
    Copyright © 2014-2019 by The xTox Project Contributors

    This file is part of xTox, a Qt-based graphical interface for Tox.

    xTox is libre software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    xTox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with xTox.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "maskablepixmapwidget.h"
#include <QPainter>
#include <QStyle>

/**
 * @var QPixmap* MaskablePixmapWidget::renderTarget
 * @brief pointer to dynamically call the constructor.
 */

MaskablePixmapWidget::MaskablePixmapWidget(QWidget* parent, QSize size, QString maskName_)
    : QLabel("", parent)
    , renderTarget(nullptr)
    , maskName(maskName_)
    , clickable(false)
{
    setSize(size);
}

MaskablePixmapWidget::~MaskablePixmapWidget()
{
    delete renderTarget;
}

void MaskablePixmapWidget::setClickable(bool clickable_)
{
    clickable = clickable_;

    if (clickable) {
        setCursor(Qt::PointingHandCursor);
    } else {
        unsetCursor();
    }
}

void MaskablePixmapWidget::setPixmap(const QPixmap& pmap)
{
    if (pmap.isNull()) {
        return;
    }

    unscaled = pmap;
    pixmap = pmap.scaled(width(), height(),
                         Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
    updatePixmap();
    update();
}

QPixmap MaskablePixmapWidget::getPixmap() const
{
    return *renderTarget;
}

void MaskablePixmapWidget::setSize(QSize size)
{
    setFixedSize(size);
    delete renderTarget;
    renderTarget = new QPixmap(size);

    QPixmap pmapMask = QPixmap(maskName);
    if (!pmapMask.isNull()) {
        mask = pmapMask.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    if (!unscaled.isNull()) {
        pixmap = unscaled.scaled(width(), height(),
                                 Qt::KeepAspectRatio,
                                 Qt::SmoothTransformation);
        updatePixmap();
        update();
    }
}

void MaskablePixmapWidget::mousePressEvent(QMouseEvent* event)
{
    std::ignore = event;
    if (clickable) {
        emit clicked();
    }
}

void MaskablePixmapWidget::updatePixmap()
{
    renderTarget->fill(Qt::transparent);

    QPoint offset((width() - pixmap.size().width()) / 2,
                  (height() - pixmap.size().height()) / 2); // centering the pixmap

    QPainter painter(renderTarget);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(offset, pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawPixmap(0, 0, mask);
    painter.end();
    QLabel::setPixmap(*renderTarget);
}
