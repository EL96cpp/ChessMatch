#include "message.h"

Message::Message() : message_size(0) {}

Message::Message(const uint32_t& message_size,
                 const QByteArray &body) : message_size(message_size),
                                           body(body) {}
