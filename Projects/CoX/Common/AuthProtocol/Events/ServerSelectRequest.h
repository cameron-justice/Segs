#pragma once
#include "AuthProtocol/AuthEvents.h"

class ServerSelectRequest : public AuthLinkEvent
{
public:
    ServerSelectRequest() : AuthLinkEvent(evServerSelectRequest),m_server_id(0)
    {}
    void init(EventProcessor *ev_src,uint8_t server_id)
    {
        m_server_id    = server_id;
        m_event_source = ev_src;
    }
    void serializefrom(GrowingBuffer &buf)
    {
        uint8_t op;
        buf.uGet(op);
        buf.uGetBytes(unkLoginArray, sizeof(unkLoginArray));
        buf.uGet(m_server_id);
    }
    void serializeto(GrowingBuffer &buf) const
    {
        buf.uPut((uint8_t)2);
        buf.uPutBytes(unkLoginArray, sizeof(unkLoginArray));
        buf.uPut(m_server_id);
    }
    uint8_t unkLoginArray[8];
    uint8_t m_server_id;
};
