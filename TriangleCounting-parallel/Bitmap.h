//
// Created by Kevan on 2019-10-08.
//

#ifndef TRIANGLECOUNTING_PRALLEL_BITMAP_H
#define TRIANGLECOUNTING_PRALLEL_BITMAP_H

#include<stdint.h>
#include<stdbool.h>

/* Bitmap 数据结构定义 */
typedef struct {
    uint64_t bitsize;
    uint64_t maxpos;

    /* 不定长，必须是结构的最后一个成员 */
    uint8_t data[];
} Bitmap;

Bitmap* init_bitmap(void);
Bitmap* init_bitmap_size(uint64_t size);
bool set_bit(Bitmap* bitmap, uint64_t offset, uint8_t value);
uint8_t get_bit(Bitmap* bitmap, uint64_t offset);
void free_bitmap(Bitmap* bitmap);

#endif //TRIANGLECOUNTING_PRALLEL_BITMAP_H
