#include <stdint.h>
#include <types/boottypes.h>
#include <acpi/acpi.h>
#include <util/util.h>
#include <init/init.h>
#include <interrupt/interrupt.h>
#include <interrupt/int_handler.h>
#include <mm/segmentation.h>
#include <mm/paging.h>
#include <mm/memory.h>
#include <graphics/graphics.h>
#include <debug/debug.h>
#include <device/device.h>
#include <app/app.h>
#include <task/thread.h>

// bootinfoからとれる情報
struct video_info *vinfo_global;
struct RSDP *rsdp;
struct memory_descriptor *start_mmap;
uint64_t mmapsize;
uint64_t memdescsize;
// GDTは0x80に固定配置
uint64_t *GDT = (uint64_t *)0x80;
// 初期ページテーブルは固定配置
uint64_t *PML4 = (uint64_t *)0x1000;
uint64_t *PDP = (uint64_t *)0x2000;
uint64_t *PD = (uint64_t *)0x3000;
// IDTは固定配置
struct gate_descriptor *IDT = (struct gate_descriptor *)0x13000;

void main_routine(void);
void hlt(int _argc, char **_argv);
void task_a(void);
void task_b(void);

extern uint64_t stack0[0x1000];
extern uint64_t stack1[0x1000];
extern uint64_t stack2[0x1000];
extern uint64_t stack3[0x1000];
extern uint64_t stack4[0x1000];
extern uint64_t stack5[0x1000];

void start_kernel(struct bootinfo *binfo)
{
    init_bss();
    init_serial();
    init_global_variables(binfo);
    init_gdt(); // init_gdtでmain_routineへジャンプする
}

/* GDTの設定が終わった後のルーチン */
void main_routine(void)
{
    init_paging();
    init_interrupt();
    init_graphics();
    //init_local_APIC();
    init_pic();

    /* いろんなレジスタとかメモリとかの表示 */
    // EFER
    putstr(600, 10, black, white, vinfo_global, "CR3: ");
    putnum(650, 10, black, white, vinfo_global, get_cr3());
    // EFER
    putstr(600, 30, black, white, vinfo_global, "CR4: ");
    putnum(650, 30, black, white, vinfo_global, get_cr4());
    // EFER
    putstr(600, 50, black, white, vinfo_global, "EFER: ");
    putnum(650, 50, black, white, vinfo_global, get_efer());
    /* 名前 */
    putstr(515, 560, black, white, vinfo_global,
           "minOS - A Minimal Operating System.");
    putstr(500, 580, black, white, vinfo_global,
           "Developer : Totsugekitai(@totsugeki8)");

    // putstr(0, 16, black, white, vinfo_global, "mmapsize: ");
    // putnum(100, 16, black, white, vinfo_global, mmapsize);

    // putstr(0, 32, black, white, vinfo_global, "memdescsize: ");
    // putnum(100, 32 ,black, white, vinfo_global, memdescsize);

    // タスクスイッチ間隔を設定
    puts_serial("period init 10\n");
    schedule_period_init(100);
    // threadsを初期化
    threads_init();

    // スレッドを生成
    // コンソールとhltを設定
    struct thread thread0 = thread_gen(stack0, (uint64_t*)task_a, 0, 0);
    struct thread thread1 = thread_gen(stack1, (uint64_t*)hlt, 0, 0);
    struct thread thread2 = thread_gen(stack2, (uint64_t*)task_b, 0, 0);

    // スレッドを走らせる
    puts_serial("thread0 run\n");
    thread_run(thread0);
    puts_serial("thread1 run\n");
    thread_run(thread1);
    thread_run(thread2);

    puts_serial("\n");
    puts_serial("next start rsp: ");
    putnum_serial(thread0.rsp);
    puts_serial("\n");
    puts_serial("next start func address: ");
    putnum_serial((uint64_t)(thread0.func_info.func));
    puts_serial("\n");
    puts_serial("next thread rip: ");
    putnum_serial(thread0.rip);
    puts_serial("\n\n");
    puts_serial("first dispatch start\n\n");

    first_dispatch3(thread0.rsp);

    // // コンソール
    // puts_serial("console start\n");
    // console(0, 0);

    puts_serial("kernel end.\n");
    while (1) {
        asm("hlt");
    }
}

void hlt(int _argc, char **_argv)
{
    puts_serial("hlt function\n");
    while (1) {
        asm volatile("hlt");
    }
}

void task_a(void)
{
    puts_serial("taskA\n");
    while (1) {
        asm volatile("hlt");
    }
}

void task_b(void)
{
    puts_serial("taskB\n");
    while (1) {
        asm volatile("hlt");
    }
}
