#pragma once
#include <Messanger_common.h>
namespace Messanger
{
    enum class msg_type : uint32_t {
        JoinServer,
        ServerAccept,
        ServerDeny,
        ServerPing,
        MessageAll,
        ServerMessage,
        PassString
    };
}