#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <bpf/bpf_helpers.h>
#include <arpa/inet.h>

#define MAX_PAYLOAD_LEN 1500

SEC("xdp")
int filter_content(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;
    
    struct ethhdr *eth = data;
    if ((void *)eth + sizeof(*eth) > data_end)
        return XDP_PASS;
    
    if (eth->h_proto != htons(ETH_P_IP))
        return XDP_PASS;
    
    struct iphdr *ip = data + sizeof(*eth);
    if ((void *)ip + sizeof(*ip) > data_end)
        return XDP_PASS;
    
    if (ip->protocol != IPPROTO_TCP)
        return XDP_PASS;
    
    struct tcphdr *tcp = (void *)ip + sizeof(*ip);
    if ((void *)tcp + sizeof(*tcp) > data_end)
        return XDP_PASS;
    
    // Получаем указатель на данные TCP
    void *payload = (void *)tcp + (tcp->doff * 4);
    if (payload + 4 > data_end) // Проверяем первые 4 байта
        return XDP_PASS;
    
    if (*(unsigned int *)payload == htonl(0x5350414D)) {
        return XDP_DROP;
    }
    
    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
