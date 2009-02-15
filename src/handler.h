#ifndef handler_h
#define handler_h

#include "command.h"
#include "event.h"
#include <tr1/functional>


namespace eir
{
    template <class T_>
    struct CommandHandlerBase
    {
        template <class F_>
        CommandRegistry::id add_handler(std::string s, F_ f)
        {
            return eir::CommandRegistry::get_instance()->add_handler(s,
                    std::tr1::bind(f, static_cast<T_*>(this), std::tr1::placeholders::_1));
        }

        template <class F_>
        CommandRegistry::id add_handler(std::string s, unsigned int t, F_ f)
        {
            return eir::CommandRegistry::get_instance()->add_handler(s, t,
                    std::tr1::bind(f, static_cast<T_*>(this), std::tr1::placeholders::_1));
        }

        template <class F_>
        EventManager::id add_event(time_t t, F_ f)
        {
            return EventManager::get_instance()->add_event(t,
                    std::tr1::bind(f, static_cast<T_*>(this)));
        }

        template <class F_>
        EventManager::id add_recurring_event(time_t t, F_ f)
        {
            return EventManager::get_instance()->add_recurring_event(t,
                    std::tr1::bind(f, static_cast<T_*>(this)));
        }
    };

    class CommandHolder :
        public paludis::InstantiationPolicy<CommandHolder, paludis::instantiation_method::NonCopyableTag>
    {
        private:
            CommandRegistry::id _id;

            void _release() { if (_id) CommandRegistry::get_instance()->remove_handler(_id); _id = 0; }

        public:
            CommandHolder() : _id(0)
            { }
            CommandHolder(CommandRegistry::id id) : _id(id)
            { }
            const CommandHolder & operator= (CommandRegistry::id id)
            { _release(); _id = id; return *this; }

            ~CommandHolder() { _release(); }
    };

    class EventHolder :
        public paludis::InstantiationPolicy<EventHolder, paludis::instantiation_method::NonCopyableTag>
    {
        private:
            EventManager::id _id;

            void _release() { if (_id) EventManager::get_instance()->remove_event(_id); _id = 0; }

        public:
            EventHolder() : _id(0)
            { }
            EventHolder(EventManager::id id) : _id(id)
            { }
            const EventHolder & operator= (EventManager::id id)
            { _release(); _id = id; return *this; }

            ~EventHolder() { _release(); }
    };
}

#endif
