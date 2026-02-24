#include "MicroUdpFraming.h"

namespace MicroUdp
{
    static void WriteU16BE(TArray<uint8>& Out, uint16 V)
    {
        Out.Add(uint8((V >> 8) & 0xFF));
        Out.Add(uint8(V & 0xFF));
    }

    static bool ReadU16BE(const TArray<uint8>& In, int32& Offset, uint16& OutV)
    {
        if (Offset + 2 > In.Num()) return false;
        OutV = (uint16(In[Offset]) << 8) | uint16(In[Offset + 1]);
        Offset += 2;
        return true;
    }

    bool Encode(const FFrame& Frame, TArray<uint8>& OutBytes)
    {
        OutBytes.Reset();

        WriteU16BE(OutBytes, Frame.Magic);
        OutBytes.Add(Frame.Version);
        OutBytes.Add(Frame.MsgType);
        WriteU16BE(OutBytes, Frame.Seq);
        OutBytes.Add(Frame.Flags);

        const uint16 Len = uint16(Frame.Payload.Num());
        WriteU16BE(OutBytes, Len);

        OutBytes.Append(Frame.Payload);

        // Phase 1: CRC unused -> write 0
        WriteU16BE(OutBytes, 0);
        return true;
    }

    bool Decode(const TArray<uint8>& InBytes, FFrame& OutFrame)
    {
        int32 Offset = 0;
        uint16 Magic = 0;
        if (!ReadU16BE(InBytes, Offset, Magic)) return false;
        if (Magic != MAGIC) return false;

        if (Offset + 1 > InBytes.Num()) return false;
        const uint8 Version = InBytes[Offset++];

        if (Version != VERSION) return false;

        if (Offset + 1 > InBytes.Num()) return false;
        const uint8 MsgType = InBytes[Offset++];

        uint16 Seq = 0;
        if (!ReadU16BE(InBytes, Offset, Seq)) return false;

        if (Offset + 1 > InBytes.Num()) return false;
        const uint8 Flags = InBytes[Offset++];

        uint16 Len = 0;
        if (!ReadU16BE(InBytes, Offset, Len)) return false;

        if (Offset + int32(Len) + 2 > InBytes.Num()) return false; // + CRC
        TArray<uint8> Payload;
        Payload.Append(&InBytes[Offset], Len);
        Offset += Len;

        uint16 Crc = 0;
        if (!ReadU16BE(InBytes, Offset, Crc)) return false;

        OutFrame.Magic = Magic;
        OutFrame.Version = Version;
        OutFrame.MsgType = MsgType;
        OutFrame.Seq = Seq;
        OutFrame.Flags = Flags;
        OutFrame.Payload = MoveTemp(Payload);
        OutFrame.Crc16 = Crc;
        return true;
    }
}