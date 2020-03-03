#ifndef XMLPARSEN_H
#define XMLPARSEN_H

#include <QtXml>
#include <QList>
#include <QDebug>
#include <QObject>
#include "namesanddefines.h"

struct XMLInfo{
    QString subOrderID;
    QString subOrderDescription;
    QString subOrderStorageRack;
    QString subOrderWarehouseClerk;
};
class XMLParsen: public QObject
{
    Q_OBJECT
public:
    XMLParsen();
    ~XMLParsen();
    QList<XMLInfo> getXMLInfo();
    QString sendOrderListID();
    QString sendOrderListDescription();
public slots:
    void slot_setXMLPath(QString);
private:
    QDomElement m_orderList;
    QString m_orderListID;
    QString m_orderListDescription;
    QDomNodeList m_order;
    QList<XMLInfo> m_XMLInfo;
    int m_nOrderNum;
    QString m_XMLPath;
};

#endif // XMLPARSEN_H
