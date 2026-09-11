/*
    Copyright © 2018-2019 by The xTox Project Contributors

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

class ToxFilePause
{
public:
    void localPause()
    {
        localPauseState = true;
    }

    void localResume()
    {
        localPauseState = false;
    }

    void localPauseToggle()
    {
        localPauseState = !localPauseState;
    }

    void remotePause()
    {
        remotePauseState = true;
    }

    void remoteResume()
    {
        remotePauseState = false;
    }

    void remotePauseToggle()
    {
        remotePauseState = !remotePauseState;
    }

    bool localPaused() const
    {
        return localPauseState;
    }

    bool remotePaused() const
    {
        return remotePauseState;
    }

    bool paused() const
    {
        return localPauseState || remotePauseState;
    }

private:
    bool localPauseState = false;
    bool remotePauseState = false;
};
