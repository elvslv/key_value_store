#include "MembershipUpdate.h"

std::map<membership_protocol::MembershipUpdateType, std::string> membership_protocol::MembershipUpdate::UPDATE_TO_STR = {
    {membership_protocol::JOINED, "JOINED"},
    {membership_protocol::FAILED, "FAILED"},
};

std::map<membership_protocol::MembershipUpdateSource, std::string> membership_protocol::MembershipUpdate::SOURCE_TO_STR = {
    {membership_protocol::FAILURE_DETECTOR, "FAILURE_DETECTOR"},
    {membership_protocol::GOSSIP_PROTOCOL, "GOSSIP_PROTOCOL"},
    {membership_protocol::INITIAL_SYNC, "INITIAL_SYNC"},
    {membership_protocol::MEMBERSHIP_PROTOCOL, "MEMBERSHIP_PROTOCOL"}
};
