#pragma once

#include <cstdint>
#include <cstring>

namespace LightweightWaveReader {

    struct WaveFormat {
        uint16_t FormatTag;
        uint16_t Channels;
        uint32_t SampleRate;
        uint32_t BytesRate;
        uint16_t BlockAlign;
        uint16_t BitsPerSample;
        uint16_t Size;  // Size of extra format information (if any)
    };

    struct WaveFormatExtensible : WaveFormat {
        uint16_t ValidBitsPerSample;
        uint32_t ChannelMask;
        uint8_t SubFormat[16];  // GUID
    };

    class WaveReader {
       private:
        const uint8_t* data;
        size_t dataSize;
        size_t dataPos;
        WaveFormat format;
        WaveFormatExtensible formatExtensible;

       public:
        WaveReader(const uint8_t* dataPtr, size_t dataSize) : data(dataPtr), dataSize(dataSize), dataPos(0) {}

        bool readHeader() {
            if (dataPos + 44 > dataSize) return false;  // Insufficient data for header

            // Check RIFF header
            if (memcmp(data + dataPos, "RIFF", 4) != 0 || memcmp(data + dataPos + 8, "WAVE", 4) != 0) return false;  // Not a valid WAV file

            // Read wave format
            format = *reinterpret_cast<const WaveFormat*>(data + dataPos + 20);
            if (format.Size == 22) {
                formatExtensible = *reinterpret_cast<const WaveFormatExtensible*>(data + dataPos + 20);
                dataPos += 52;  // Skip the header for WaveFormatExtensible
            } else {
                dataPos += 44;  // Skip the header for WaveFormat
            }
            return true;
        }

        const WaveFormat& getFormat() const { return (format.Size == 22) ? static_cast<const WaveFormat&>(formatExtensible) : format; }

        size_t readData(void* buffer, size_t bufferSize) {
            size_t bytesToRead = (dataPos + bufferSize <= dataSize) ? bufferSize : (dataSize - dataPos);
            memcpy(buffer, data + dataPos, bytesToRead);
            dataPos += bytesToRead;
            return bytesToRead;
        }

        void reset() { dataPos = 0; }
    };

}  // namespace LightweightWaveReader
