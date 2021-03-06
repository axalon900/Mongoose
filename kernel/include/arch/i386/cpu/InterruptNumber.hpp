//
// Created by Martin Miralles-Cordal on 4/21/2018.
//

#pragma once

enum class InterruptNumber : uint8_t
{
    kDivideByZero = 0,
    kDebugger,
    kNMI,
    kBreakpoint,
    kOverflow,
    kBounds,
    kInvalidOpcode,
    kCoprocessorNotAvailable,
    kDoubleFault,
    kCoprocessorSegmentOverrun,
    kInvalidTSS,
    kSegmentNotPresent,
    kStackFault,
    kGeneralProtectionFault,
    kPageFault,
    k0E,
    kMathFault,
    kAlignmentCheck,
    kMachineCheck,
    kSIMDFloatingPointException,
    kIRQ0 = 32,
    kPITIRQ = kIRQ0,
    kKeyboardIRQ,
    kIRQ2,
    kIRQ3,
    kIRQ4,
    kIRQ5,
    kIRQ6,
    kIRQ7,
    kIRQ8,
    kIRQ9,
    kIRQ10,
    kIRQ11,
    kIRQ12,
    kIRQ13,
    kIRQ14,
    kIRQ15,
    kSystemCall = 0x80
};
