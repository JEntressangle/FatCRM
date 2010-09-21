#ifndef OPPORTUNITIESPAGE_H
#define OPPORTUNITIESPAGE_H

#include "page.h"

#include <akonadi/item.h>


class OpportunitiesPage : public Page
{
    Q_OBJECT
public:
    explicit OpportunitiesPage( QWidget *parent = 0 );

    ~OpportunitiesPage();

protected:
    /*reimp*/ void addItem( const QMap<QString, QString> &data );
    /*reimp*/ void modifyItem( Akonadi::Item &item, const QMap<QString, QString> &data );

};

#endif /* OPPORTUNITIESPAGE_H */

