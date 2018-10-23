#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(void)
{
    struct cpu cpu; // more flexible

    cpu_init(&cpu);
    cpu_load(&cpu);
    cpu_run(&cpu);

    return 0;
}

// int main(void)
// {
//     struct cpu *cpu = cpu_create();

//     cpu_load(&cpu);
//     cpu_run(&cpu);

//     cpu_destroy(cpu);

//     return 0;
// }