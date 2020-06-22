#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define page_len 4096
#define BUF_SIZE 512
size_t get_filesize(const char* filename);


int main (int argc, char* argv[])
{
    char buf[BUF_SIZE];
    int i, dev_fd, file_fd;
    size_t ret, file_size, offset = 0, tmp;
    char file_name[50], method[20];
    char *kernel_address = NULL, *file_address = NULL;
    struct timeval start;
    struct timeval end;
    double trans_time; 


    strcpy(file_name, argv[1]);
    strcpy(method, argv[2]);

    dev_fd = open("/dev/master_device", O_RDWR);
    
    gettimeofday(&start ,NULL);
    
    file_fd = open (file_name, O_RDWR);

    file_size = get_filesize(file_name);


    ioctl(dev_fd, 0x12345677);

      char *f_address, *k_address;
    
    
    switch(method[0]) 
    {
        case 'f': 
            do
            {
                ret = read(file_fd, buf, sizeof(buf));
                write(dev_fd, buf, ret);
            }while(ret > 0);
            break;
        case 'm': 
            while (offset < file_size) {
                f_address = mmap(NULL, page_len, PROT_READ, MAP_SHARED, file_fd, offset);
   
                k_address = mmap(NULL, page_len, PROT_WRITE, MAP_SHARED, dev_fd, offset);
                
                    int len =0;
                    if(offset + BUF_SIZE > file_size)
                        len = file_size % BUF_SIZE;
                    else
                        len = BUF_SIZE;
                    memcpy(k_address, f_address, len);
                    offset += len;
                    ioctl(dev_fd, 0x12345678, len);
                 while (offset < file_size && offset % page_len != 0){
		    len =0;
                    if(offset + BUF_SIZE > file_size)
                        len = file_size % BUF_SIZE;
                    else
                        len = BUF_SIZE;
                    memcpy(k_address, f_address, len);
                    offset += len;
                    ioctl(dev_fd, 0x12345678, len);
                
                    ioctl(dev_fd, 0x12345676, (unsigned long)f_address);
                    munmap(f_address, page_len);
		}
            }
            break;
    }

    if(ioctl(dev_fd, 0x12345679) == -1) 
    {
    	perror("ioclt server exits error\n");
    	return 1;
    }
    gettimeofday(&end, NULL);
    trans_time = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)*0.0001;
    printf("Transmission time: %lf ms, File size: %ld bytes\n", trans_time, file_size / 8);

    

    close(file_fd);
    close(dev_fd);

    return 0;
}

size_t get_filesize(const char* filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}
