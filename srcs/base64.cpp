#include "base64.hpp"
#include <iostream>
#include <stdio.h>

using std::cout;
using std::endl;
using std::string;

static char to_base64(unsigned char c)
{
    if (c <= 25) {
        return ('A' + c);
    } else if (c <= 51) {
        return ('a' + c - 26);
    } else if (c <= 61) {
        return ('0' + c - 52);
    } else if (c == 61) {
        return ('+');
    }
    return ('/');
}

std::string Base64::encode(std::string str)
{
    size_t size = ((str.size() / 3) + 1) * 4;
    char bits[size + 1];
    const char* char_str = str.c_str();
    unsigned char c;
    size_t i = 0;
    size_t k = 0;
    unsigned char bit = 0;
    unsigned char rest = 0;
    int pos = 0;
    unsigned char rest_array[] = {0b00000000, 0b00000001, 0b00000011, 0b00000111, 0b00001111,
                                  0b00011111, 0b00111111, 0b01111111, 0b11111111};
    while (char_str[i]) {
        c = (unsigned char)char_str[i];
        pos = pos - 6;
        if (pos < 0)
            pos += 8;
        bit = rest | (c >> pos);
        rest = (c & rest_array[pos]) << (6 - pos);
        bits[k++] = to_base64(bit);
        if (pos == 6) {
            bits[k++] = to_base64(rest);
            rest = 0;
            pos = 0;
        }
        i++;
    }
    if (pos != 6) {
        bits[k++] = to_base64(rest);
    }
    while (size > k) {
        bits[k++] = '=';
    }
    bits[size] = 0;
    return string(bits);
}

static unsigned char to_bit(char c)
{
    if ('A' <= c && c <= 'Z') {
        return (c - 'A');
    } else if ('a' <= c && c <= 'z') {
        return (c - 'a' + 26);
    } else if ('0' <= c && c <= '9') {
        return (c - '0' + 52);
    } else if (c == '+') {
        return (62);
    }
    return (63);
}

std::string Base64::decode(std::string str)
{
    unsigned char rest_array[] = {0b00000000, 0b00100000, 0b00110000, 0b00111000, 0b00111100, 0b00111110, 0b00111111};
    size_t size = (str.size() * 3) / 4;
    const char* char_str = str.c_str();
    char decode_char[size + 1];
    size_t i = 0;
    size_t k = 0;
    unsigned char c;
    unsigned char rest;
    int pos = 0;
    while (char_str[i] && char_str[i] != '=') {
        c = to_bit(char_str[i]);
        pos = pos + 6;
        if (pos >= 8) {
            pos -= 8;
            decode_char[k++] = rest | ((c & rest_array[6 - pos]) >> pos);
        }
        rest = c << (8 - pos);
        i++;
    }
    decode_char[k++] = '\0';

    return (string(decode_char));
}
