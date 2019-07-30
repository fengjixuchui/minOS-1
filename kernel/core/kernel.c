#include <stdint.h>
#include <types/boottypes.h>
#include <init/initfunc.h>
#include <mm/segmentation.h>
#include <interrupt/interrupt.h>
#include <interrupt/int_handler.h>
#include <mm/paging.h>
#include <graphics/graphics.h>
#include <debug/debug.h>

void main_routine(struct video_info *vinfo);

struct pix_format black = {0x00, 0x00, 0x00, 0x00};
struct pix_format white = {0xFF, 0xFF, 0xFF, 0x00};
struct pix_format red = {0x00, 0x00, 0xFF, 0x00};
struct pix_format green = {0x00, 0xFF, 0x00, 0x00};
struct pix_format blue = {0xFF, 0x00, 0x00, 0x00};

struct video_info *vinfo_global = (struct video_info *)0;

void start_kernel(struct bootinfo *binfo)
{
    /* 画面描画 */
    struct video_info vinfo = binfo->vinfo;
    uint32_t i, j;

    /* フレームバッファの初期化作業 */
    uint32_t x_axis = vinfo.x_axis;
    uint32_t y_axis = vinfo.y_axis;
    uint32_t ppsl = vinfo.ppsl;
    struct pix_format *fb = vinfo.fb;
    for (i = 0; i < y_axis; i++) {
        for (j = 0; j < x_axis; j++) {
            fb[i * ppsl + j] = white;
        }
    }
    /* 初期の画面描画ここまで */

    /* GDTなどの初期化 */
    // BSSセクションの初期化
    init_bss();

    // GDTの先頭アドレスは0x80
    uint64_t *GDT = (uint64_t *)0x80;
    // 空, KERNEL_CS, USER_CSの3つを用意
    GDT[0] = make_segment_descriptor(0, 0, 1);
    GDT[1] = make_segment_descriptor(5, 0, 0);
    GDT[2] = make_segment_descriptor(5, 3, 0);
    // lgdtでGDTをセットする
    load_gdt(GDT, 0x17);
    // セグメントレジスタの設定をやってmain_routineへとぶ
    set_segment_register(0x8, &vinfo);
}

/* GDTの設定が終わった後のルーチン */
void main_routine(struct video_info *vinfo)
{
    vinfo_global = vinfo; // 割り込みハンドラ用のグローバル変数
    // uint32_t i;
    /* ページングの初期化 */
    uint64_t *PML4 = (uint64_t *)0x1000;
    uint64_t *PDP = (uint64_t *)0x2000;
    uint64_t *PD = (uint64_t *)0x10000;
    create_pgtable(PML4, PDP, PD);
    load_pgtable(PML4);

    /* IDTの初期化 */
    // IDTの先頭アドレスは0x20000
    struct gate_descriptor *IDT = (struct gate_descriptor *)0x20000;
    struct gate_descriptor zero_gate = make_gate_descriptor((uint64_t)general_protection, 0, 0, 0);
    for (int i = 0; i < 13; i++) {
        IDT[i] = zero_gate;
    }
    IDT[13] = make_gate_descriptor((uint64_t)general_protection, 0, 0, 0);
    for (int i = 14; i < 256; i++) {
        IDT[i] = zero_gate;
    }
    load_idt((uint64_t)IDT, 256);

    /* いろんなレジスタとかメモリとかの表示 */
    // EFER
    putstr(600, 10, black, white, vinfo, "CR3: ");
    putnum(650, 10, black, white, vinfo, get_cr3());
    // EFER
    putstr(600, 30, black, white, vinfo, "CR4: ");
    putnum(650, 30, black, white, vinfo, get_cr4());
    // EFER
    putstr(600, 50, black, white, vinfo, "EFER: ");
    putnum(650, 50, black, white, vinfo, get_efer());
    // 自由欄
    putstr(10, 10, black, white, vinfo, "func: ");
    putnum(60, 10, black, white, vinfo, (uint64_t)make_segment_descriptor);

    /* 名前 */
    putstr(515, 560, black, white, vinfo,
           "minOS - A Minimal Operating System.");
    putstr(500, 580, black, white, vinfo,
           "Developer : Totsugekitai(@totsugeki8)");

    // デバッグ用
    general_protection();
    // generate_gp();

    while (1) {}
}
