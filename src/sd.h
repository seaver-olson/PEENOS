#define SD_OK                0
#define SD_TIMEOUT          -1
#define SD_ERROR            -2

int sd_init();
int sd_readblock(unsigned int lba, unsigned char *buffer, unsigned int num);
int sd_writeblock(unsigned char *buff, unsigned int lba, unsigned int num);
