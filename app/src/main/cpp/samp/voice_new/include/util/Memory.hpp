/*
    This is a SampVoice project file
    Developer: CyberMor <cyber.mor.2020@gmail.ru>

    See more here https://github.com/CyberMor/sampvoice

    Copyright (c) Daniel (CyberMor) 2020 All rights reserved
*/

#pragma once

#include <memory>
#include <type_traits>
#include <functional>
#include <cassert>
#include <vector>
#include <cstring>

#define RequestArithmeticType(type) static_assert(std::is_arithmetic<type>::value, #type " is not arithmetic type")
#define RequestAddressType(type) static_assert(std::is_arithmetic<type>::value || std::is_pointer<type>::value, #type " is not address type")
#define SizeOfArray(arr) ((sizeof(arr) / sizeof(0[arr])) / ((size_t)(!(sizeof(arr) % sizeof(0[arr])))))

namespace Memory
{
    class ScopeExit {
    public:

    private:
        using CallbackType = std::function<void()>;

    public:
        explicit ScopeExit(CallbackType callback) noexcept
            : callback(std::move(callback)) {}

        ~ScopeExit() noexcept
        {
            if(this->callback) this->callback();
        }

    public:
        void Release() noexcept
        {
            this->callback = nullptr;
        }

    private:
        CallbackType callback { nullptr };
    };

    template<class ObjectType> class ObjectContainer {
    public:

        ObjectContainer() = default;
        ObjectContainer(const ObjectContainer&) = default;
        ObjectContainer(ObjectContainer&&) noexcept = default;
        ObjectContainer& operator=(const ObjectContainer&) = default;
        ObjectContainer& operator=(ObjectContainer&&) noexcept = default;

    public:

        explicit ObjectContainer(const uint32_t addMemSize)
            : bytes(sizeof(ObjectType) + addMemSize) {}

        template<class MemAddrType = const void*, class MemSizeType = uint32_t>
        explicit ObjectContainer(const MemAddrType memAddr, const MemSizeType memSize)
            : bytes((uint32_t)(memSize))
        {
            RequestAddressType(MemAddrType);
            RequestArithmeticType(MemSizeType);

            assert((const void*)(memAddr));
            assert((uint32_t)(memSize));

            assert((uint32_t)(memSize) >= sizeof(ObjectType));

            std::memcpy(this->bytes.data(), (const void*)(memAddr), this->bytes.size());
        }

        ~ObjectContainer() noexcept = default;

    public:

        const ObjectType* operator->() const noexcept
        {
            return reinterpret_cast<const ObjectType*>(this->bytes.data());
        }

        ObjectType* operator->() noexcept
        {
            return reinterpret_cast<ObjectType*>(this->bytes.data());
        }

        const ObjectType* operator&() const noexcept
        {
            return reinterpret_cast<const ObjectType*>(this->bytes.data());
        }

        ObjectType* operator&() noexcept
        {
            return reinterpret_cast<ObjectType*>(this->bytes.data());
        }

        const void* GetData() const noexcept
        {
            return static_cast<const void*>(this->bytes.data());
        }

        void *GetData() noexcept
        {
            return static_cast<void *>(this->bytes.data());
        }

        uint32_t GetSize() const noexcept
        {
            return static_cast<uint32_t>(this->bytes.size());
        }

    private:

        std::vector<uint8_t> bytes { sizeof(ObjectType) };

    };

    template<class ObjectType> using ObjectContainerPtr = std::unique_ptr<ObjectContainer<ObjectType>>;
};

#define MakeObjectContainer(ObjectType) std::make_unique<Memory::ObjectContainer<ObjectType>>