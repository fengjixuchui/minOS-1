#include <stdint.h>
#include <util/util.h>

#define PORT    0x3f8

// シリアル通信
void init_serial(void)
{
    io_outb(PORT + 1, 0x00);
    io_outb(PORT + 3, 0x80);
    io_outb(PORT + 0, 0x03);
    io_outb(PORT + 1, 0x00);
    io_outb(PORT + 3, 0x03);
    io_outb(PORT + 2, 0xC7);
    io_outb(PORT + 4, 0x0B);
}

uint32_t serial_received(void)
{
    return io_inb(PORT + 5) & 1;
}

uint8_t read_serial(void)
{
    while (serial_received() == 0) {}
    return io_inb(PORT);
}

// JISキーボード対応
char map_scan_to_ascii(uint64_t scan, uint8_t *shift)
{
    if (scan == 0x2a || scan == 0x36) {
        *shift = 1;
        return 0x00;
    }
    if (scan == 0xaa || scan == 0xb6) {
        *shift = 0;
        return 0x00;
    }

    if (*shift == 0) {
        switch (scan)
        {
        case 0x1e:
            return 'a';
        case 0x30:
            return 'b';
        case 0x2e:
            return 'c';
        case 0x20:
            return 'd';
        case 0x12:
            return 'e';
        case 0x21:
            return 'f';
        case 0x22:
            return 'g';
        case 0x23:
            return 'h';
        case 0x17:
            return 'i';
        case 0x24:
            return 'j';
        case 0x25:
            return 'k';
        case 0x26:
            return 'l';
        case 0x32:
            return 'm';
        case 0x31:
            return 'n';
        case 0x18:
            return 'o';
        case 0x19:
            return 'p';
        case 0x10:
            return 'q';
        case 0x13:
            return 'r';
        case 0x1f:
            return 's';
        case 0x14:
            return 't';
        case 0x16:
            return 'u';
        case 0x2f:
            return 'v';
        case 0x11:
            return 'w';
        case 0x2d:
            return 'x';
        case 0x15:
            return 'y';
        case 0x2c:
            return 'z';
        case 0x0b:
            return '0';
        case 0x02:
            return '1';
        case 0x03:
            return '2';
        case 0x04:
            return '3';
        case 0x05:
            return '4';
        case 0x06:
            return '5';
        case 0x07:
            return '6';
        case 0x08:
            return '7';
        case 0x09:
            return '8';
        case 0x0a:
            return '9';
        case 0x0c:
            return '-';
        case 0x0d:
            return '^';
        case 0x7d:
            return '\\';
        case 0x2b:
            return ']';
        case 0x0e:
            return 0x08; // バックスペース
        case 0x39:
            return ' ';
        case 0x1c:
            return 0x0a; // LF(改行)
        case 0x1a:
            return '@';
        case 0x1b:
            return '[';
        case 0x27:
            return ';';
        case 0x28:
            return ':';
        case 0x33:
            return ',';
        case 0x34:
            return '.';
        case 0x35:
            return '/';
        case 0x73:
            return '\\';
        default:
            break;
        }
    } else {
        switch (scan)
        {
        case 0x1e:
            return 'A';
        case 0x30:
            return 'B';
        case 0x2e:
            return 'C';
        case 0x20:
            return 'D';
        case 0x12:
            return 'E';
        case 0x21:
            return 'F';
        case 0x22:
            return 'G';
        case 0x23:
            return 'H';
        case 0x17:
            return 'I';
        case 0x24:
            return 'J';
        case 0x25:
            return 'K';
        case 0x26:
            return 'L';
        case 0x32:
            return 'M';
        case 0x31:
            return 'N';
        case 0x18:
            return 'O';
        case 0x19:
            return 'P';
        case 0x10:
            return 'Q';
        case 0x13:
            return 'R';
        case 0x1f:
            return 'S';
        case 0x14:
            return 'T';
        case 0x16:
            return 'U';
        case 0x2f:
            return 'V';
        case 0x11:
            return 'W';
        case 0x2d:
            return 'X';
        case 0x15:
            return 'Y';
        case 0x2c:
            return 'Z';
        case 0x02:
            return '!';
        case 0x03:
            return '"';
        case 0x04:
            return '#';
        case 0x05:
            return '$';
        case 0x06:
            return '%';
        case 0x07:
            return '&';
        case 0x08:
            return '\'';
        case 0x09:
            return '(';
        case 0x0a:
            return ')';
        case 0x0c:
            return '=';
        case 0x0d:
            return '~';
        case 0x7d:
            return '|';
        case 0x2b:
            return '}';
        case 0x0e:
            return 0x08; // バックスペース
        case 0x39:
            return ' '; // スペース
        case 0x1c:
            return 0x0a; // LF(改行)
        case 0x1a:
            return '`';
        case 0x1b:
            return '{';
        case 0x27:
            return '+';
        case 0x28:
            return '*';
        case 0x33:
            return '<';
        case 0x34:
            return '>';
        case 0x35:
            return '?';
        case 0x73:
            return '_';
        default:
            break;
        }
    }
    return 0x00;
}