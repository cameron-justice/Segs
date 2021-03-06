#pragma once
#include "EventProcessor.h"

#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>

#include <unordered_map>
#include <vector>

class MessageBusEndpoint;
class SEGSTimer;
/**
 * @brief The MessageBus class is responsible for forwarding incoming events to correct subscriber's endpoints
 *
 */
class MessageBus final : private EventProcessor
{
    std::unordered_map<uint32_t,std::vector<MessageBusEndpoint *> > m_specific_subscriber_map;
    std::vector<MessageBusEndpoint *> m_catch_all_subscribers;
    SEGSTimer *m_statistics_timer = nullptr;
    friend void postGlobalEvent(SEGSEvent *ev);
    friend void shutDownMessageBus();
    friend class MessageBusEndpoint; // allow endpoints to register/unregister
public:

static constexpr uint32_t ALL_EVENTS = ~0U; // special event type allowing subscriptions to all incoming events

public:
                MessageBus();
                bool ReadConfigAndRestart();
private:
                ///
                /// \brief subscribe given endpoint \a ep to all events of \a type
                /// \param type specific event type, or ALL_EVENTS
                /// \param ep which endpoint to notify
                ///
        void    subscribe(uint32_t type, MessageBusEndpoint *ep);
                ///
                /// \brief unsubscribe from all events ( type == ALL_EVENTS )
                ///
        void    unsubscribe(uint32_t type,MessageBusEndpoint *);
        void    on_timer_event(TimerEvent *tmr);
        void    dispatch(SEGSEvent *ev) override;
                ///
                /// \brief do_publish will locate all handlers ( catch-all, and event specific ), and send message copies to them.
                /// \param ev this event will be shallow_copy'ied and putq'd to all subscribers
                ///
        void    do_publish(SEGSEvent *ev);
        void    recalculateStatisitcs();
};

void postGlobalEvent(SEGSEvent *ev);
void shutDownMessageBus();
