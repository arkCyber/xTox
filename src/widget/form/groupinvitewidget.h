/*
    Copyright © 2017-2019 by The xTox Project Contributors

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

#include "src/model/groupinvite.h"

#include <QWidget>

class CroppingLabel;

class QHBoxLayout;
class QPushButton;
class Settings;
class Core;

class GroupInviteWidget : public QWidget
{
    Q_OBJECT
public:
    GroupInviteWidget(QWidget* parent, const GroupInvite& invite, Settings& settings,
        Core& core);
    void retranslateUi();
    const GroupInvite getInviteInfo() const;

signals:
    void accepted(const GroupInvite& invite);
    void rejected(const GroupInvite& invite);

private:
    QPushButton* acceptButton;
    QPushButton* rejectButton;
    CroppingLabel* inviteMessageLabel;
    QHBoxLayout* widgetLayout;
    GroupInvite inviteInfo;
    Settings& settings;
    Core& core;
};
