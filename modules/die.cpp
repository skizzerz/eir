#include "eir.h"

#include "handler.h"

using namespace std::tr1::placeholders;
using namespace eir;

struct Die : public CommandHandlerBase<Die>
{
    void die(const Message *m)
    {
        if (m->source.client && m->source.client->privs().has_privilege("admin"))
        {
            m->source.reply("Bye bye...");
            m->bot->disconnect("Shutting down (" + m->source.name + ")");
            throw DieException(m->source.client->nuh());
        }
    }
    void restart(const Message *m)
    {
        if (m->source.client && m->source.client->privs().has_privilege("admin"))
        {
            m->source.reply("Restarting...");
            m->bot->disconnect("Restarting (" + m->source.name + ")");
            throw RestartException();
        }
    }

    CommandHolder die_id, restart_id;

    Die()
    {
        die_id = add_handler("die", sourceinfo::IrcCommand, &Die::die);
        restart_id = add_handler("restart", sourceinfo::IrcCommand, &Die::restart);
    }
} dieer;

