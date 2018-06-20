#include "MessageDispatcher.h"

namespace utils
{

MessageDispatcher::MessageDispatcher(const network::Address& address, const std::shared_ptr<Log>& logger)
    : network(address)
    , logger(logger)
    , callbacks()
{
}

void MessageDispatcher::sendMessage(std::unique_ptr<Message> message, const network::Address& destAddress)
{
    auto msg = message->serialize();
    logger->log("Sending message", message->getMsgTypeStr(), "from", message->getSourceAddress(), " to ", destAddress.toString(), message->getDestinationAddress(), message->getId());

    network.send(destAddress, msg);
}

void MessageDispatcher::run()
{
    logger->log("<MessageDispatcher::run> -- start");

    while (isRunning)
    {
        auto message = network.receive();
        if (message.empty())
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(MessageDispatcher::SLEEP_DELAY);
            continue;
        }

        // try
        // {
        std::unique_ptr<Message> parsedMessage = Message::parseMessage(message);

        logger->log("<MessageDispatcher> -- Received msg ", parsedMessage->getMsgTypeStr(), " from ", parsedMessage->getSourceAddress().toString(), " to ", parsedMessage->getDestinationAddress(), " id ", parsedMessage->getId());

        Callback callback;
        {
            std::lock_guard<std::mutex> lock(mutex);

            auto it = callbacks.find(parsedMessage->getMsgTypeStr());
            if (it == callbacks.end())
            {
                logger->log("<MessageDispatcher> -- listeners of ", parsedMessage->getMsgTypeStr(), " not found. Discarding this message");
                continue;
            }

            callback = it->second;
        }

        callback(std::move(parsedMessage));
        // }
        // catch (...)
        // {
        //     logger->log(network.address.toString(), this);
        //     logger->log(network.address.toString(), "stack trace:");

        //     // storage array for stack trace address data
        //     int max_frames = 1024;
        //     void* addrlist[max_frames + 1];

        //     // retrieve current stack addresses
        //     int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

        //     if (addrlen == 0)
        //     {
        //         logger->log(network.address.toString(), "  <empty, possibly corrupt>");
        //         return;
        //     }

        //     // resolve addresses into strings containing "filename(function+address)",
        //     // this array must be free()-ed
        //     char** symbollist = backtrace_symbols(addrlist, addrlen);

        //     // allocate string which will be filled with the demangled function name
        //     size_t funcnamesize = 256;
        //     char* funcname = (char*)malloc(funcnamesize);

        //     // iterate over the returned symbol lines. skip the first, it is the
        //     // address of this function.
        //     for (int i = 1; i < addrlen; i++)
        //     {
        //         char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

        //         // find parentheses and +address offset surrounding the mangled name:
        //         // ./module(function+0x15c) [0x8048a6d]
        //         for (char* p = symbollist[i]; *p; ++p)
        //         {
        //             if (*p == '(')
        //                 begin_name = p;
        //             else if (*p == '+')
        //                 begin_offset = p;
        //             else if (*p == ')' && begin_offset)
        //             {
        //                 end_offset = p;
        //                 break;
        //             }
        //         }

        //         if (begin_name && begin_offset && end_offset
        //             && begin_name < begin_offset)
        //         {
        //             *begin_name++ = '\0';
        //             *begin_offset++ = '\0';
        //             *end_offset = '\0';

        //             // mangled name is now in [begin_name, begin_offset) and caller
        //             // offset in [begin_offset, end_offset). now apply
        //             // __cxa_demangle():

        //             int status;
        //             char* ret = abi::__cxa_demangle(begin_name,
        //                 funcname, &funcnamesize, &status);
        //             if (status == 0)
        //             {
        //                 funcname = ret; // use possibly realloc()-ed string
        //                 logger->log(network.address.toString(), symbollist[i], ":", funcname, "+", begin_offset);
        //             }
        //             else
        //             {
        //                 // demangling failed. Output function name as a C function with
        //                 // no arguments.
        //                 logger->log(network.address.toString(), symbollist[i], ":", begin_name, "()+", begin_offset);
        //             }
        //         }
        //         else
        //         {
        //             // couldn't parse the line? print the whole line.
        //             logger->log(network.address.toString(), symbollist[i]);
        //         }
        //     }

        //     free(funcname);
        //     free(symbollist);
        //     exit(1);
        // }
    }

    logger->log("<MessageDispatcher::run> -- stop");
}

void MessageDispatcher::stopListening(const std::string& token)
{
    std::lock_guard<std::mutex> lock(mutex);

    auto it = tokenToType.find(token);
    if (it == tokenToType.end())
    {
        std::stringstream ss;
        ss << "Listener to token " << token << " not found";
        throw std::logic_error(ss.str());
    }

    auto msgType = it->second;
    if (typeToToken[msgType] != token)
    {
        std::stringstream ss;
        ss << "Given listener " << token << " of message " << msgType << " doesn't match registered listener " << typeToToken[msgType];
        throw std::logic_error(ss.str());
    }

    tokenToType.erase(it);
    typeToToken.erase(msgType);
    callbacks.erase(msgType);
}

std::string MessageDispatcher::listen(const std::string& msgType, const Callback& callback)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (callbacks.find(msgType) != callbacks.end())
    {
        std::stringstream ss;
        ss << "Listener to message " << msgType << " already exists";
        throw std::logic_error(ss.str());
    }

    std::string token = Utils::getRandomString(TOKEN_LENGTH);
    callbacks[msgType] = callback;
    typeToToken[msgType] = token;
    tokenToType[token] = msgType;

    return token;
}

constexpr std::chrono::duration<long long, std::milli> MessageDispatcher::SLEEP_DELAY;
}
