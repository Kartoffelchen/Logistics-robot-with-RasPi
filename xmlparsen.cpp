#include "xmlparsen.h"

XMLParsen::XMLParsen()
{
    m_XMLPath = " ";
    m_nOrderNum = 0;
}
QList<XMLInfo> XMLParsen::getXMLInfo()
{
    QFile xmlFile(m_XMLPath);
    if(!xmlFile.exists())
    {
        qDebug()<<"cannot open this file,maybe wrong filename!";
    }
//    QFile xmlFile("/home/pi/Desktop/MyCar/Challenge/WallE/example_deliveryPlan.xml");
    xmlFile.open(QIODevice::ReadOnly);
    QDomDocument doc;
    doc.setContent(xmlFile.readAll());
    xmlFile.close();

    m_orderList = doc.firstChildElement("orderList");
    m_orderListID = m_orderList.firstChildElement("id").text();
    m_orderListDescription = m_orderList.firstChildElement("description").text();

    m_order = doc.elementsByTagName("order");
    m_nOrderNum = m_order.size();

    XMLInfo dXMLInfo[m_nOrderNum];
    for(int idx = 0; idx < m_nOrderNum; idx++)
    {
        QDomNode subOrder =  m_order.item(idx);
        dXMLInfo[idx].subOrderID = subOrder.firstChildElement("id").text();
        dXMLInfo[idx].subOrderDescription = subOrder.firstChildElement("description").text();
        dXMLInfo[idx].subOrderStorageRack = subOrder.firstChildElement("storageRack").text();
        dXMLInfo[idx].subOrderWarehouseClerk = subOrder.firstChildElement("warehouseClerk").text();
        m_XMLInfo.append(dXMLInfo[idx]);
    }
    return m_XMLInfo;
}
QString XMLParsen::sendOrderListID()
{
    return m_orderListID;
}
QString XMLParsen::sendOrderListDescription()
{
    return m_orderListDescription;
}
void XMLParsen::slot_setXMLPath(QString qXMLPath)
{
    m_XMLPath = qXMLPath;
}
XMLParsen::~XMLParsen()
{
}
