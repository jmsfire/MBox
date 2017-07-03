#pragma once

#include <KTL\KTL.Memory.SharedPtr.h>
#include <KTL\KTL.Containers.List.h>

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {
        struct OperationCallbackPacket;
        struct InstanceCallbackFunction;
        struct StreamCallbackFunction;
        struct StreamHandleCallbackFunction;
        struct FileCallbackFunction;
        struct ContextCleanupCallbackFunction;

        struct CallbackPacket
        {
            OperationCallbackPacket*    m_OperationCallbackPacket{};
            InstanceCallbackFunction*   m_InstanceCallbackFunction{};
            StreamCallbackFunction*     m_StreamCallbackFunction{};
            StreamHandleCallbackFunction*   m_StreamHandleCallbackFunction{};
            FileCallbackFunction*           m_FileCallbackFunction{};
            ContextCleanupCallbackFunction* m_ContextCleanupCallbackFunction{};
        };

        using CallbackPacketList$Type = ktl::list<ktl::shared_ptr<CallbackPacket>>;

        NTSTATUS CreateCallbackPacketList();
        void     DestoryCallbackPacketList();
        CallbackPacketList$Type* GetCallbackPacketList();

        template <typename F>
        void TraverseCallbacketList(F aFunctor)
        {
            auto vCallbackPacketList = GetCallbackPacketList();

            ktl::u32 vIndex = 0;
            for (auto &vItem : *vCallbackPacketList)
            {
                if (aFunctor(vItem.get(), vIndex++))
                {
                    break;
                }
            }
        }

        template<typename F>
        NTSTATUS RegisterCallbackPacket(
            CallbackPacket* aCallbackPacket,
            F aDeletor)
        {
            auto vItem = GetCallbackPacketList()->emplace_back();
            if ((GetCallbackPacketList()->end() != vItem)
                && (vItem->attach(aCallbackPacket, aDeletor)))
            {
                return STATUS_SUCCESS;
            }

            return STATUS_INSUFFICIENT_RESOURCES;
        }
    }
}