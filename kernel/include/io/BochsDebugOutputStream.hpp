#pragma once

#include <arch/i386/sys/asm.h>
#include <io/OutputStream.hpp>

class BochsDebugOutputStream : public OutputStream
{
public:
    virtual void write(std::byte byte) { outb(0xe9, (uint8_t)byte); }
};
