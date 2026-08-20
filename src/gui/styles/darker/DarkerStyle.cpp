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

#include "DarkerStyle.h"

#include <QDialog>
#include <QFile>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

#ifdef Q_OS_MACOS
#include "gui/osutils/OSUtils.h"
#endif

/*
 * Darker theme.
 *
 * Sits between stock Dark (#3B3B3D chrome) and Dark (OLED) (#000000 chrome):
 * chrome around #1E1E20 with #141416 input surfaces, so contrast stays low
 * without collapsing surface hierarchy into pure black.
 */
DarkerStyle::DarkerStyle()
    : BaseStyle()
{
#ifdef Q_OS_MACOS
    m_drawNativeMacOsToolBar = osUtils->isDarkMode();
#endif
}

QPalette DarkerStyle::standardPalette() const
{
    auto palette = BaseStyle::standardPalette();

    palette.setColor(QPalette::Active, QPalette::Window, QRgb(0x1E1E20));
    palette.setColor(QPalette::Inactive, QPalette::Window, QRgb(0x232325));
    palette.setColor(QPalette::Disabled, QPalette::Window, QRgb(0x262628));

    palette.setColor(QPalette::Active, QPalette::WindowText, QRgb(0xC6C7CA));
    palette.setColor(QPalette::Inactive, QPalette::WindowText, QRgb(0xB4B5B8));
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QRgb(0x6A6A6E));

    palette.setColor(QPalette::Active, QPalette::Text, QRgb(0xC6C7CA));
    palette.setColor(QPalette::Inactive, QPalette::Text, QRgb(0xB4B5B8));
    palette.setColor(QPalette::Disabled, QPalette::Text, QRgb(0x6A6A6E));

    palette.setColor(QPalette::Active, QPalette::PlaceholderText, QRgb(0x7A7A80));
    palette.setColor(QPalette::Inactive, QPalette::PlaceholderText, QRgb(0x74747A));
    palette.setColor(QPalette::Disabled, QPalette::PlaceholderText, QRgb(0x5E5E62));

    palette.setColor(QPalette::Active, QPalette::BrightText, QRgb(0x1A1A1B));
    palette.setColor(QPalette::Inactive, QPalette::BrightText, QRgb(0x202022));
    palette.setColor(QPalette::Disabled, QPalette::BrightText, QRgb(0x2A2A2C));

    palette.setColor(QPalette::Active, QPalette::Base, QRgb(0x141416));
    palette.setColor(QPalette::Inactive, QPalette::Base, QRgb(0x171719));
    palette.setColor(QPalette::Disabled, QPalette::Base, QRgb(0x1D1D20));

    palette.setColor(QPalette::Active, QPalette::AlternateBase, QRgb(0x191A1D));
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase, QRgb(0x18181B));
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase, QRgb(0x212124));

    palette.setColor(QPalette::All, QPalette::ToolTipBase, QRgb(0x244324));
    palette.setColor(QPalette::All, QPalette::ToolTipText, QRgb(0xC4C4C4));

    palette.setColor(QPalette::Active, QPalette::Button, QRgb(0x17171A));
    palette.setColor(QPalette::Inactive, QPalette::Button, QRgb(0x17171A));
    palette.setColor(QPalette::Disabled, QPalette::Button, QRgb(0x1A1A1B));

    palette.setColor(QPalette::Active, QPalette::ButtonText, QRgb(0xB4B4B9));
    palette.setColor(QPalette::Inactive, QPalette::ButtonText, QRgb(0x98999E));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QRgb(0x6A6B74));

    palette.setColor(QPalette::Active, QPalette::Highlight, QRgb(0x2A4E2A));
    palette.setColor(QPalette::Inactive, QPalette::Highlight, QRgb(0x2C3A2D));
    palette.setColor(QPalette::Disabled, QPalette::Highlight, QRgb(0x223022));

    palette.setColor(QPalette::Active, QPalette::HighlightedText, QRgb(0xCCCCCC));
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QRgb(0xCECECE));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QRgb(0x6A6A6A));

    palette.setColor(QPalette::All, QPalette::Light, QRgb(0x2A2A2E));
    palette.setColor(QPalette::All, QPalette::Midlight, QRgb(0x232326));
    palette.setColor(QPalette::All, QPalette::Mid, QRgb(0x1B1B1E));
    palette.setColor(QPalette::All, QPalette::Dark, QRgb(0x121214));
    palette.setColor(QPalette::All, QPalette::Shadow, QRgb(0x0C0C0D));

    palette.setColor(QPalette::All, QPalette::Link, QRgb(0x6BB86B));
    palette.setColor(QPalette::Disabled, QPalette::Link, QRgb(0x6A9A6A));
    palette.setColor(QPalette::All, QPalette::LinkVisited, QRgb(0x78BA78));
    palette.setColor(QPalette::Disabled, QPalette::LinkVisited, QRgb(0x6E9C6E));

    return palette;
}

QString DarkerStyle::getAppStyleSheet() const
{
    QFile extStylesheetFile(QStringLiteral(":/styles/darker/darkerstyle.qss"));
    if (extStylesheetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return extStylesheetFile.readAll();
    }
    qWarning("Failed to load darker theme stylesheet.");
    return {};
}

void DarkerStyle::polish(QWidget* widget)
{
    if (qobject_cast<QMainWindow*>(widget) || qobject_cast<QDialog*>(widget) || qobject_cast<QMenuBar*>(widget)
        || qobject_cast<QToolBar*>(widget)) {
        auto palette = widget->palette();
        palette.setColor(QPalette::Active, QPalette::Window, QRgb(0x191919));
        palette.setColor(QPalette::Inactive, QPalette::Window, QRgb(0x1C1C1E));
        palette.setColor(QPalette::Disabled, QPalette::Window, QRgb(0x212123));
        widget->setPalette(palette);
    }
}
