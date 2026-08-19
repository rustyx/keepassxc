/*
 *  Copyright (C) 2026 KeePassXC Team <team@keepassxc.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KEEPASSXC_BROWSERENTRYSEARCHDIALOG_H
#define KEEPASSXC_BROWSERENTRYSEARCHDIALOG_H

#include <QDialog>
#include <QTimer>

class Entry;

struct SearchDialogEntry
{
    Entry* entry;
    QString warning;
};

namespace Ui
{
    class BrowserEntrySearchDialog;
}

class BrowserEntrySearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrowserEntrySearchDialog(QWidget* parent = nullptr);
    ~BrowserEntrySearchDialog() override;

    void setSiteUrl(const QString& siteUrl);
    void setEntries(const QList<SearchDialogEntry>& entries);
    QList<Entry*> selectedEntries() const;
    bool remember() const;

signals:
    void searchRequested(const QString& searchText);

private slots:
    void performSearch();
    void selectionChanged();

private:
    void addEntryToList(const SearchDialogEntry& searchEntry, int row);

private:
    QScopedPointer<Ui::BrowserEntrySearchDialog> m_ui;
    QList<SearchDialogEntry> m_entries;
    QTimer m_searchTimer;
};

#endif // KEEPASSXC_BROWSERENTRYSEARCHDIALOG_H
