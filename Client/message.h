#ifndef MESSAGE_H
#define MESSAGE_H

#include <stddef.h>
#include <stdint.h>
#include <QByteArray>


struct Message {

    Message();
    Message(const uint32_t& message_size, const QByteArray& body);
    Message(const Message& message);

    uint32_t message_size;
    QByteArray body;

};

#endif // MESSAGE_H
