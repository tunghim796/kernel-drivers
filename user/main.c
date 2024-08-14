#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdint.h>

#define DEVICE_NAME "/dev/fpgawifi"
#define IOCTL_GET_STATS _IOR('f', 1, struct fpgawifi_rc_stats*)

struct fpgawifi_rate_stats {
    uint8_t mcs;
    uint8_t nss;
    unsigned long nr_transmits;
    unsigned long nr_pkts;
    unsigned long nr_success;
};

struct fpgawifi_rc_stats_per_sta {
    uint8_t mac_addr[6];
    uint8_t last_rate_selected;
    struct fpgawifi_rate_stats rate_stats[40];
};

struct fpgawifi_rc_stats {
    uint8_t nr_of_stas;
    struct fpgawifi_rc_stats_per_sta sta_rc_stats[32];
};

int main() {
    int fd;
    struct fpgawifi_rc_stats stats;

    fd = open(DEVICE_NAME, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    if (ioctl(fd, IOCTL_GET_STATS, &stats) == -1) {
        perror("Failed to get stats");
        close(fd);
        return 1;
    }

    printf("Number of STAs: %d\n", stats.nr_of_stas);
    for (int i = 0; i < stats.nr_of_stas; i++) {
        printf("STA %d MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", i,
               stats.sta_rc_stats[i].mac_addr[0], stats.sta_rc_stats[i].mac_addr[1],
               stats.sta_rc_stats[i].mac_addr[2], stats.sta_rc_stats[i].mac_addr[3],
               stats.sta_rc_stats[i].mac_addr[4], stats.sta_rc_stats[i].mac_addr[5]);
    }

    close(fd);
    return 0;
}
