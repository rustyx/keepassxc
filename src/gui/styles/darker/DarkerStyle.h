/*
 *  Copyright (C) 2020 KeePassXC Team <team@keepassxc.org>
 *  Copyright (C) 2026 KeePassXC contributors (Darker theme)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KEEPASSXC_DARKERSTYLE_H
#define KEEPASSXC_DARKERSTYLE_H

#include "gui/styles/base/BaseStyle.h"

/**
 * Low-luminance dark theme, darker than stock Dark but not pure black.
 * Selectable via View → Theme → Darker; does not replace stock Dark.
 */
class DarkerStyle : public BaseStyle
{
    Q_OBJECT

public:
    DarkerStyle();
    QPalette standardPalette() const override;

    using BaseStyle::polish;
    void polish(QWidget* widget) override;

protected:
    QString getAppStyleSheet() const override;
};

#endif // KEEPASSXC_DARKERSTYLE_H
