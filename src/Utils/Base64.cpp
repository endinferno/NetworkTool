#include "Utils/Base64.hpp"

std::string Base64::Encode(const std::string_view& data, bool removePadding)
{
    const size_t dataSize = data.size();
    const size_t resultSize =
        (dataSize / 3 + static_cast<size_t>(dataSize % 3 > 0)) << 2;
    std::string result(resultSize, PADDING_CHAR);

    size_t dataIdx = 0;
    size_t resultIdx = 0;

    for (size_t i = dataSize / 3; i != 0; i--) {
        const uint8_t byte1 = data[dataIdx++];
        const uint8_t byte2 = data[dataIdx++];
        const uint8_t byte3 = data[dataIdx++];
        result[resultIdx++] = ENCODE_TABLE_0[byte1];
        result[resultIdx++] =
            ENCODE_TABLE_1[((byte1 & 0x03) << 4) | ((byte2 >> 4) & 0x0F)];
        result[resultIdx++] =
            ENCODE_TABLE_1[((byte2 & 0x0F) << 2) | ((byte3 >> 6) & 0x03)];
        result[resultIdx++] = ENCODE_TABLE_1[byte3];
    }

    switch (dataSize % 3) {
    case 0:
    {
        break;
    }
    case 1:
    {
        const uint8_t byte1 = data[dataIdx];
        result[resultIdx++] = ENCODE_TABLE_0[byte1];
        result[resultIdx++] = ENCODE_TABLE_1[(byte1 & 0x03) << 4];
        break;
    }
    case 2:
    {
        const uint8_t byte1 = data[dataIdx];
        const uint8_t byte2 = data[dataIdx + 1];
        result[resultIdx++] = ENCODE_TABLE_0[byte1];
        result[resultIdx++] =
            ENCODE_TABLE_1[((byte1 & 0x03) << 4) | ((byte2 >> 4) & 0x0F)];
        result[resultIdx++] = ENCODE_TABLE_1[(byte2 & 0x0F) << 2];
        break;
    }
    default:
    {
        throw std::runtime_error("Invalid base64 encoded data");
    }
    }
    if (removePadding) {
        result.resize(resultIdx);
    }

    return result;
}
