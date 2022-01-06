#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>
#include <iostream>
#include <cstdio>
#include <QPainter>


static void print_mem(const void *base, int size){
    for(int i=0; i<size; i++){
        printf("%02X", ((unsigned char*)base)[i]);
    }
    printf("\n");
}


typedef void (QPainter::*drawPixmap_t)(const QPointF &point, const QPixmap &pixmap);

static drawPixmap_t origin_drawpixmap = 0;


void QPainter::drawPixmap(const QPointF &point, const QPixmap &pixmap)
{
    if(origin_drawpixmap == 0){
        void *tmpPtr = dlsym(RTLD_NEXT, "_ZN8QPainter10drawPixmapERK7QPointFRK7QPixmap");
        memcpy(&origin_drawpixmap, &tmpPtr, sizeof(tmpPtr));
    }

    std::cerr << __func__ << ": in hook\n";
    return (this->*origin_drawpixmap)(point, pixmap);
}

