#ifndef SERVICE_H
#define SERVICE_H

#include <wSocket/socket.h>
#include <wType/string.h>
#include <QtCore/QString>
#include <QtCore/QMap>

class Service
{
private:
    Service(uint64_t p_id, const QString& p_name, const QString& p_address, const QString& p_port);
    
public:
    ~Service();
    
    static Service* create(const QMap<QString, QString>& params);
    
private:
    W::Socket* socket;
    static uint64_t lastId;
    
public:
    QString name;
    QString address;
    QString port;
    const uint64_t id;
    
private:
    
    
};

#endif // SERVICE_H
