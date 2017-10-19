#include <cstdlib>
#include <iostream>


#include <stdint.h>

#define AL      (sizeof(uintptr_t))
#define U(p)    ((uintptr_t) (p))
#define PA(p)   ((uintptr_t *) (U(p) / AL * AL))

uintptr_t *_begin;
uintptr_t *_end;

void mysetup(void *buf, std::size_t size) {
    _begin = PA(U(buf) + AL - 1);
    _end = PA(U(buf) + size);
    *_begin = *(_end - 1) = _end - _begin - 2;
}

void *myalloc(std::size_t size) {
    // TODO
}

void myfree(void *p) {
    // TODO
}

void show(const char *caption, void *from, void *to, std::size_t unit_size, std::size_t count) {
    std::cout << caption << " \t"
              << from << " -> " << to << "\t"
              << " #" << U(to) - U(from) << "\t"
              << " [" << count << " * " << unit_size << "]" << "\t"
              << std::endl;
}

int main() {
    long buf[15] = {0};

    std::size_t size = sizeof(buf);
    size_t bs = sizeof(buf[0]);
    size_t cnt = size / bs;


    mysetup(buf, size);


    show("in ", buf, buf + cnt, bs, cnt);
    show("out", _begin, _end, AL, _end - _begin);

    return 0;
}
