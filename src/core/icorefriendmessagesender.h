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

#include "receiptnum.h"

#include <QString>
#include <cstdint>

class ICoreFriendMessageSender
{
public:
    ICoreFriendMessageSender() = default;
    virtual ~ICoreFriendMessageSender();
    ICoreFriendMessageSender(const ICoreFriendMessageSender&) = default;
    ICoreFriendMessageSender& operator=(const ICoreFriendMessageSender&) = default;
    ICoreFriendMessageSender(ICoreFriendMessageSender&&) = default;
    ICoreFriendMessageSender& operator=(ICoreFriendMessageSender&&) = default;
    virtual bool sendAction(uint32_t friendId, const QString& action, ReceiptNum& receipt) = 0;
    virtual bool sendMessage(uint32_t friendId, const QString& message, ReceiptNum& receipt) = 0;
};
