/*
  This file is part of FatCRM, a desktop application for SugarCRM written by KDAB.

  Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Authors: Michel Boyer de la Giroday <michel.giroday@kdab.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SELECTITEMDIALOG_H
#define SELECTITEMDIALOG_H

#include <QDialog>

#include "enums.h"
#include "itemstreemodel.h"

namespace Akonadi
{
class Item;
}

namespace Ui {
class SelectItemDialog;
}

class ItemDataExtractor;

class SelectItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectItemDialog(DetailsType type, QWidget *parent);
    ~SelectItemDialog();

    void setModel(QAbstractItemModel *model);

Q_SIGNALS:
    void selectedItem(const QString &itemId);

protected:
    virtual void accept() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void slotItemSelected(const Akonadi::Item &item);

private:
    ItemDataExtractor *mDataExtractor;
    Akonadi::Item mSelectedItem;
    Ui::SelectItemDialog *mUi;
};

#endif // SELECTITEMDIALOG_H