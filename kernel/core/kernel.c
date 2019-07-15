#ifndef MY_HEADER_H_
#define MY_HEADER_H_

#include <types/boottypes.h>
#include <types/mmtypes.h>
#include <init/initfunc.h>
#include <mm/segmentation.h>
#include <graphics/graphics.h>
extern void load_gdt(long *);

#endif

void start_kernel(struct bootinfo *binfo)
{
    /* GDT初期化 */
    {
        init_bss();
        init_gdt();
        // GDTは0x00000000~0x0009FFFF
        struct segm_descriptor *gdt = (struct segm_descriptor *)0x00000000;
        // 0こ目のdescriptorはNULL descriptor
        set_segm_descriptor(gdt + 0, 0x00000000, 0x00000000, 0, 0, 0, 0);
        // 1つ目のdescriptorはメモリ全体
        set_segm_descriptor(gdt + 1, 0x00000000, 0x40000000, 3, 0, 0, 1);
        // lgdtでセットする(アセンブラで書く)
        load_gdt(0x00000000);
    }

    /* 画面描画ここから */
    struct video_info vinfo = binfo->vinfo;
    unsigned int i, j;

    // フレームバッファの初期化作業
    {
        unsigned int x_axis = vinfo.x_axis;
        unsigned int y_axis = vinfo.y_axis;
        unsigned int ppsl = vinfo.ppsl;
        struct pix_format *fb = vinfo.fb;
        struct pix_format white = { 0xFF, 0xFF, 0xFF, 0x00 };
        for (i = 0; i < y_axis; i++) {
            for (j = 0; j < x_axis; j++) {
                fb[i * ppsl + j] = white;
            }
        }
    }

    struct pix_format black = { 0x00, 0x00, 0x00, 0x00 };
    struct pix_format white = { 0xFF, 0xFF, 0xFF, 0x00 };
    struct pix_format red   = { 0x00, 0x00, 0xFF, 0x00 };
    struct pix_format green = { 0x00, 0xFF, 0x00, 0x00 };
    struct pix_format blue  = { 0xFF, 0x00, 0x00, 0x00 };

    draw_square(30, 30, red, 300, 200, &vinfo);
    draw_square(60, 60, green, 300, 200, &vinfo);
    draw_square(90, 90, blue, 300, 200, &vinfo);

    putchar(100, 100, white, black, &vinfo, 'A');
    putchar(108, 100, white, black, &vinfo, 'B');
    putchar(116, 100, white, black, &vinfo, 'C');

    putstr(400, 400, black, white, &vinfo,
            "minOS - A Minimal Operating System.");
    putstr(400, 420, black, white, &vinfo,
            "Developer : Totsugekitai(@totsugeki8)");

    while (1) {}

    return;
}
