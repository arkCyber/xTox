/*
    Copyright © 2019 by The xTox Project Contributors

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

#pragma once

#include <QWidget>

class Style;

class GenericForm : public QWidget
{
    Q_OBJECT
public:
    GenericForm(const QPixmap& icon, Style& style);
    virtual ~GenericForm()
    {
    }

    virtual QString getFormName() = 0;
    QPixmap getFormIcon();

public slots:
    virtual void reloadTheme() {}

protected:
    bool eventFilter(QObject* o, QEvent* e) final;
    void eventsInit();

protected:
    QPixmap formIcon;
};
