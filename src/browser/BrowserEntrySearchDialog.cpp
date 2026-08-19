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

#include "BrowserEntrySearchDialog.h"
#include "ui_BrowserEntrySearchDialog.h"

#include <QUrl>

#include "core/Entry.h"
#include "core/Group.h"
#include "gui/Icons.h"
#include "gui/UrlTools.h"
#include "gui/styles/StateColorPalette.h"

static constexpr int SEARCH_INTERVAL = 150;

BrowserEntrySearchDialog::BrowserEntrySearchDialog(QWidget* parent)
    : QDialog(parent)
    , m_ui(new Ui::BrowserEntrySearchDialog())
{
    // The dialog is intentionally parentless: it must not be tied to the main window, so that the window manager
    // returns the focus to the browser instead of KeePassXC after the dialog is closed.
    setAttribute(Qt::WA_X11BypassTransientForHint);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowIcon(icons()->applicationIcon());

    m_ui->setupUi(this);

    m_searchTimer.setInterval(SEARCH_INTERVAL);
    m_searchTimer.setSingleShot(true);

    connect(m_ui->searchEdit, SIGNAL(textChanged(QString)), &m_searchTimer, SLOT(start()));
    // Refresh the results before the default button accepts the dialog
    connect(m_ui->searchEdit, SIGNAL(returnPressed()), SLOT(performSearch()));
    connect(&m_searchTimer, SIGNAL(timeout()), SLOT(performSearch()));
    connect(m_ui->selectButton, SIGNAL(clicked()), SLOT(accept()));
    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
    connect(m_ui->itemsTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(accept()));
    connect(m_ui->itemsTable->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this,
            SLOT(selectionChanged()));
    connect(m_ui->itemsTable, SIGNAL(acceptSelections()), SLOT(accept()));
    connect(m_ui->itemsTable, SIGNAL(focusInWithoutSelections()), this, SLOT(selectionChanged()));

    m_ui->itemsTable->setColumnCount(4);
    m_ui->itemsTable->setHorizontalHeaderLabels({tr("Title"), tr("Username"), tr("URL"), tr("Warning")});
    m_ui->itemsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

BrowserEntrySearchDialog::~BrowserEntrySearchDialog()
{
}

void BrowserEntrySearchDialog::setSiteUrl(const QString& siteUrl)
{
    QUrl url(siteUrl);
    m_ui->siteLabel->setText(m_ui->siteLabel->text().arg(
        url.toDisplayString(QUrl::RemoveUserInfo | QUrl::RemovePath | QUrl::RemoveQuery | QUrl::RemoveFragment)));
    m_ui->siteLabel->setToolTip(siteUrl);

    m_ui->searchEdit->setText(UrlTools::getBaseDomainFromUrl(siteUrl));
    m_ui->searchEdit->selectAll();
    m_ui->searchEdit->setFocus();

    performSearch();
}

void BrowserEntrySearchDialog::setEntries(const QList<SearchDialogEntry>& entries)
{
    m_entries = entries;

    m_ui->itemsTable->clearContents();
    m_ui->itemsTable->setRowCount(entries.count());

    int row = 0;
    for (const auto& searchEntry : entries) {
        addEntryToList(searchEntry, row);
        ++row;
    }

    if (!entries.isEmpty()) {
        m_ui->itemsTable->selectRow(0);
    }

    selectionChanged();
}

QList<Entry*> BrowserEntrySearchDialog::selectedEntries() const
{
    QList<Entry*> selected;
    for (const auto& index : m_ui->itemsTable->selectionModel()->selectedRows()) {
        selected.append(m_entries.at(index.row()).entry);
    }
    return selected;
}

bool BrowserEntrySearchDialog::remember() const
{
    return m_ui->rememberCheckBox->isChecked();
}

void BrowserEntrySearchDialog::performSearch()
{
    m_searchTimer.stop();
    emit searchRequested(m_ui->searchEdit->text());
}

void BrowserEntrySearchDialog::selectionChanged()
{
    const auto selectedRows = m_ui->itemsTable->selectionModel()->selectedRows();

    m_ui->selectButton->setEnabled(!selectedRows.isEmpty());
    m_ui->selectButton->setDefault(!selectedRows.isEmpty());
    m_ui->selectButton->setAutoDefault(!selectedRows.isEmpty());

    if (selectedRows.isEmpty()) {
        m_ui->selectButton->clearFocus();
    }
}

void BrowserEntrySearchDialog::addEntryToList(const SearchDialogEntry& searchEntry, int row)
{
    const auto* entry = searchEntry.entry;
    const auto entryUrl = entry->resolveMultiplePlaceholders(entry->url());

    const auto titleItem = new QTableWidgetItem();
    titleItem->setText(entry->resolveMultiplePlaceholders(entry->title()));
    if (entry->group()) {
        titleItem->setToolTip(entry->group()->hierarchy().join("/"));
    }
    m_ui->itemsTable->setItem(row, 0, titleItem);

    const auto usernameItem = new QTableWidgetItem();
    usernameItem->setText(entry->resolveMultiplePlaceholders(entry->username()));
    m_ui->itemsTable->setItem(row, 1, usernameItem);

    const auto urlItem = new QTableWidgetItem();
    urlItem->setText(entryUrl);
    urlItem->setToolTip(entryUrl);
    m_ui->itemsTable->setItem(row, 2, urlItem);

    // Show why the entry was not offered automatically, e.g. access to it has been denied for this site
    const auto warningItem = new QTableWidgetItem();
    if (!searchEntry.warning.isEmpty()) {
        warningItem->setText(searchEntry.warning);
        warningItem->setToolTip(searchEntry.warning);
        warningItem->setIcon(
            icons()->icon("dialog-warning", true, StateColorPalette().color(StateColorPalette::Warning)));
    }
    m_ui->itemsTable->setItem(row, 3, warningItem);
}
