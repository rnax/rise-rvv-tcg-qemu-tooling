#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <riscv_vector.h>

#define FAILED_INDEX 4

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

char *buf1;
char *buf2;

static void handler(int sig, siginfo_t *si, void *unused)
{
    uint32_t vstart;
    asm volatile("csrr %0, vstart":"=r"(vstart));
    printf("vstart[0x%x] expected vstart[0x%x]\n", vstart, FAILED_INDEX);

    if (vstart != FAILED_INDEX) {
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_SUCCESS);
    }
}

void *memcpy_vec(void *restrict destination, const void *restrict source,
        size_t n)
{
    unsigned char *dst = destination;
    const unsigned char *src = source;

    for (size_t vl; n > 0; n -= vl, src += vl, dst += vl) {
        vl = __riscv_vsetvl_e8m1(n);
        vuint8m1_t vec_src = __riscv_vle8_v_u8m1(src, vl);
        __riscv_vse8_v_u8m1(dst, vec_src, vl);
    }
    return destination;
}

int main(int argc, char *argv[])
{
    char *p;
    int pagesize;
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        handle_error("sigaction");
    }

    pagesize = sysconf(_SC_PAGE_SIZE);
    if (pagesize == -1) {
        handle_error("sysconf");
    }

    /* Allocate a buf1 aligned on a page boundary;
     *        initial protection is PROT_READ | PROT_WRITE */
    buf1 = memalign(pagesize, 4 * pagesize);
    if (buf1 == NULL) {
        handle_error("memalign");
    }

    buf2 = memalign(pagesize, pagesize);
    if (buf2 == NULL) {
        handle_error("memalign");
    }

    if (mprotect(buf1 + pagesize, pagesize, PROT_READ) == -1) {
        handle_error("mprotect");
    }

    memcpy_vec(buf1 + pagesize - FAILED_INDEX, buf2, 8);

    printf("Loop completed\n");     /* Should never happen */
    exit(EXIT_SUCCESS);
}
