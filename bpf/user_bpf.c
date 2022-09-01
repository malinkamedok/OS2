#include <stdio.h>

struct bpf_redirect_data {
    unsigned int flags;
    unsigned int tgt_index;
    unsigned int kern_flags;
    unsigned int nh_family;
};

int main(){
    struct bpf_redirect_data data;
    FILE* file;

    file = fopen("/sys/kernel/debug/labaOs/update_redirect_info", "w");
    fprintf(file, "%d", 0);
    fclose(file);

    file = fopen("/sys/kernel/debug/labaOs/bpf_redirect_info", "rb");
    fread(&data, sizeof(struct bpf_redirect_data), 1, file);
    fclose(file);

    printf("flags: %u\ntgt_index: %u\nkern_flags: %u\nnh_family: %u\n",
           data.flags, data.tgt_index, data.kern_flags, data.nh_family);
}
