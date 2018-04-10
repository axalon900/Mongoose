#include <string.h>
#include <Kernel.hpp>

//======================================================
// Kernel
//======================================================
void Kernel::panic(const char *string)
{
    size_t length = strlen(string);
    console()->setBackgroundColor(COLOR_RED);
    console()->setForegroundColor(COLOR_WHITE);
    console()->clear();
    const char *banner[3] = {"* * * * * * * * * * * * * * * * * * * *",
                             "* LAMBOS FOUND OUT ABOUT WHAT YOU DID *",
                             "* * * * * * * * * * * * * * * * * * * *"};
    for (int i = 0; i < 3; ++i) {
        size_t bannerlength = strlen(banner[i]);
        size_t columnToCenter = console()->width() > bannerlength ? (console()->width() - bannerlength) / 2 : 0;
        console()->moveTo(i + 1, columnToCenter);
        console()->writeString(banner[i]);
    }

    size_t rowToCenter = console()->height() / 2;
    size_t columnToCenter = (console()->width() - length) / 2;
    console()->moveTo(rowToCenter, columnToCenter);
    console()->writeString(string);

    const char *msg = "0xL4MBOS";
    console()->moveTo(console()->height() - 2, (console()->width() - strlen(msg)) / 2);
    console()->writeString(msg);

    asm("cli\nhlt");
}
