#include <types/boottypes.h>
#include <graphics/graphics.h>
#include <device/device.h>
#include <interrupt/int_handler.h>

// グローバル変数の読み込み
extern struct pix_format blue;
extern struct pix_format red;
extern struct pix_format white;
extern struct video_info *vinfo_global;

void gp_handler(void) {
    putstr(600, 70, white, blue, vinfo_global, "#GP");
    __asm__ volatile(
        "loop1:     hlt           \n"
        "          jmp loop1        "
    );
}

__attribute__((interrupt))
void gp_handler_deluxe(struct InterruptFrame *frame, uint64_t error_code)
{
    puts_serial("#GP ERROR CODE = ");
    putnum_serial(error_code);
    puts_serial("\n");

    puts_serial("RIP: ");
    putnum_serial(frame->rip);
    puts_serial("\n");
    puts_serial("CS: ");
    putnum_serial(frame->cs);
    puts_serial("\n");
    puts_serial("RFLAGS: ");
    putnum_serial(frame->rflags);
    puts_serial("\n");
    puts_serial("RSP: ");
    putnum_serial(frame->rsp);
    puts_serial("\n");
    puts_serial("SS: ");
    putnum_serial(frame->ss);
    puts_serial("\n");

    putstr(600, 70, white, blue, vinfo_global, "#GP");
    while (1) asm volatile("hlt");
}

void pf_handler(void)
{
    putstr(600, 70, white, red, vinfo_global, "#PF");
    __asm__ volatile(
        "loop2:     hlt           \n"
        "          jmp loop2        "
    );
}

