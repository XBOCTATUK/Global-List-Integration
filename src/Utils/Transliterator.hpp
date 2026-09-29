#pragma once

namespace Utils {
    static const std::unordered_map<uint32_t, std::string> translit = {
        {0x0410, "A"},  {0x0430, "a"},
        {0x0411, "B"},  {0x0431, "b"},
        {0x0412, "V"},  {0x0432, "v"},
        {0x0413, "G"},  {0x0433, "g"},
        {0x0414, "D"},  {0x0434, "d"},
        {0x0415, "E"},  {0x0435, "e"},
        {0x0401, "YO"}, {0x0451, "yo"},
        {0x0416, "ZH"}, {0x0436, "zh"},
        {0x0417, "Z"},  {0x0437, "z"},
        {0x0418, "I"},  {0x0438, "i"},
        {0x0419, "Y"},  {0x0439, "y"},
        {0x041A, "K"},  {0x043A, "k"},
        {0x041B, "L"},  {0x043B, "l"},
        {0x041C, "M"},  {0x043C, "m"},
        {0x041D, "N"},  {0x043D, "n"},
        {0x041E, "O"},  {0x043E, "o"},
        {0x041F, "P"},  {0x043F, "p"},
        {0x0420, "R"},  {0x0440, "r"},
        {0x0421, "S"},  {0x0441, "s"},
        {0x0422, "T"},  {0x0442, "t"},
        {0x0423, "U"},  {0x0443, "u"},
        {0x0424, "F"},  {0x0444, "f"},
        {0x0425, "KH"}, {0x0445, "kh"},
        {0x0426, "TS"}, {0x0446, "ts"},
        {0x0427, "CH"}, {0x0447, "ch"},
        {0x0428, "SH"}, {0x0448, "sh"},
        {0x0429, "SHCH"}, {0x0449, "shch"},
        {0x042A, ""},   {0x044A, ""},  
        {0x042B, "Y"},  {0x044B, "y"},
        {0x042C, ""},   {0x044C, ""},  
        {0x042D, "E"},  {0x044D, "e"},
        {0x042E, "YU"}, {0x044E, "yu"},
        {0x042F, "YA"}, {0x044F, "ya"},
    };

    inline std::string transliterator(const std::string& ruStr) {
        std::string result;
        result.reserve(ruStr.size());

        for (size_t i = 0; i < ruStr.size();) {
            unsigned char c = ruStr[i];

            if (c < 0b10000000) {
                result += static_cast<char>(c);
                ++i;
                continue;
            }

            uint32_t codepoint = 0;
            size_t sequenceLength = 0;

            if ((c & 0b11100000) == 0b11000000 && i + 1 < ruStr.size()) {
                codepoint =
                    ((c & 0b00011111) << 6) |
                    (static_cast<unsigned char>(ruStr[i + 1]) & 0b00111111);

                sequenceLength = 2;
            }

            else if ((c & 0b11110000) == 0b11100000 && i + 2 < ruStr.size()) {
                codepoint =
                    ((c & 0b00001111) << 12) |
                    ((static_cast<unsigned char>(ruStr[i + 1]) & 0b00111111) << 6) |
                    (static_cast<unsigned char>(ruStr[i + 2]) & 0b00111111);

                sequenceLength = 3;
            }

            else if ((c & 0b11111000) == 0b11110000 && i + 3 < ruStr.size()) {
                codepoint =
                    ((c & 0b00000111) << 18) |
                    ((static_cast<unsigned char>(ruStr[i + 1]) & 0b00111111) << 12) |
                    ((static_cast<unsigned char>(ruStr[i + 2]) & 0b00111111) << 6) |
                    (static_cast<unsigned char>(ruStr[i + 3]) & 0b00111111);

                sequenceLength = 4;
            }

            if (sequenceLength == 0) {
                result += static_cast<char>(c);
                ++i;
                continue;
            }

            if (auto it = translit.find(codepoint); it != translit.end()) {
                result += it->second;
            }
            else {
                result.append(ruStr, i, sequenceLength);
            }

            i += sequenceLength;
        }

        return result;
    }
}