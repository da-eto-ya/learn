#include <cstdio>
#include <cinttypes>
#include <map>

std::map<uint64_t, uint64_t> mem;

inline uint64_t bits(uint64_t value, int length, int start = 0) {
    return ((value & (((1UL << length) - 1) << start)) >> start);
}

void out_hex(uint64_t value, const char *header = nullptr) {
    if (header != nullptr) {
        printf(header);
    }

    printf("%018" PRIu64 "\t0x%016" PRIx64 "\n", value, value);
}

uint64_t mem_value(uint64_t pa) {
    auto it = mem.find(pa);

    if (it != mem.end()) {
        return it->second;
    }

    return 0;
}

int main() {
    FILE *IN = fopen("in.txt", "r");
    FILE *OUT = fopen("out.txt", "w");
    uint64_t m, q, cr3;
    fscanf(IN, "%" SCNu64 " %" SCNu64 " %" SCNu64, &m, &q, &cr3);

    for (uint64_t i = 0; i < m; ++i) {
        uint64_t address, value;
        fscanf(IN, "%" SCNu64 " %" SCNu64, &address, &value);
        mem.insert(std::make_pair(address, value));
        out_hex(address, "Address:\t");
        out_hex(value, "Value:  \t");
        printf("\n");
    }

    for (uint64_t i = 0; i < q; ++i) {
        uint64_t pa = 0;
        bool fault = true;

        do {
            uint64_t la;
            fscanf(IN, "%" SCNu64, &la);
            out_hex(la, "Logical address:\t");

            uint64_t offset = bits(la, 12);
            uint64_t table = bits(la, 9, 12);
            uint64_t dir = bits(la, 9, 12 + 9);
            uint64_t dptr = bits(la, 9, 12 + 2 * 9);
            uint64_t pml4 = bits(la, 9, 12 + 3 * 9);
            uint64_t high47 = bits(la, 17, 49);

            out_hex(high47, "\thigh47:\t");
            out_hex(pml4, "\tpml4:  \t");
            out_hex(dptr, "\tdptr:  \t");
            out_hex(dir, "\tdir:   \t");
            out_hex(table, "\ttable: \t");
            out_hex(offset, "\toffset:\t");

//            if (high47 != 0x0UL && high47 != 0x1ffffUL) {
//                printf("Bits 47-63 are inconsistent\n");
//                break;
//            }
//

            uint64_t descriptor_pa = cr3 + pml4 * 8;
            uint64_t descriptor = mem_value(descriptor_pa);
            out_hex(descriptor_pa, "pml4 address:\t");
            out_hex(descriptor,    "pml4 value:  \t");

            if ((descriptor & 1UL) != 1UL) {
                printf("Invalid descriptor\n");
                break;
            }

            uint64_t base_pa = bits(descriptor, 40, 12) << 12;
            out_hex(base_pa, "base:\t");


            descriptor_pa = base_pa + dptr * 8;
            descriptor = mem_value(descriptor_pa);
            out_hex(descriptor_pa, "dptr address:\t");
            out_hex(descriptor,    "dptr value:  \t");

            if ((descriptor & 1UL) != 1UL) {
                printf("Invalid descriptor\n");
                break;
            }

            base_pa = bits(descriptor, 40, 12) << 12;
            out_hex(base_pa, "base:\t");


            descriptor_pa = base_pa + dir * 8;
            descriptor = mem_value(descriptor_pa);
            out_hex(descriptor_pa, "dir address:\t");
            out_hex(descriptor,    "dir value:  \t");

            if ((descriptor & 1UL) != 1UL) {
                printf("Invalid descriptor\n");
                break;
            }

            base_pa = bits(descriptor, 40, 12) << 12;
            out_hex(base_pa, "base:\t");


            descriptor_pa = base_pa + table * 8;
            descriptor = mem_value(descriptor_pa);
            out_hex(descriptor_pa, "table address:\t");
            out_hex(descriptor,    "table value:  \t");

            if ((descriptor & 1UL) != 1UL) {
                printf("Invalid descriptor\n");
                break;
            }

            base_pa = bits(descriptor, 40, 12) << 12;
            out_hex(base_pa, "base:\t");
            pa = base_pa + offset;
            out_hex(pa, "Phisical address:\t");
            fault = false;
        } while (false);

        if (fault) {
            fprintf(OUT, "fault\n");
        } else {
            fprintf(OUT, "%" PRIu64 "\n", pa);
        }

        printf("\n");
    }

    fclose(IN);
    fclose(OUT);

    return 0;
}
