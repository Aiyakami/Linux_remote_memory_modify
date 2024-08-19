#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

//通过直接修改内存映射文件实现

int main(){
    pid_t pid;
    unsigned long addt;
    int mem_fd;
    char mem_file[256];
    long new_value;
    // 获取目标进程的 PID
    printf("Enter the target process PID: ");
    scanf("%d", &pid);
    // 获取目标进程中要修改的内存地址
    printf("Enter the memory address to modify (in hex): ");
    scanf("%lx", &addr);
    // 获取要写入的值
    printf("Enter the new value to write (as a long integer): ");
    scanf("%ld", &new_value);
    // 构建 /proc/<pid>/mem 文件路径
    /*
    int snprintf(char *str, size_t size, const char *format, ...);
    */
    snprintf(mem_file,sizeof(mem_file),"/proc/%d/mem",pid);
    // 打开 /proc/<pid>/mem 文件 O_RDWR（Read/Write) RD:read
    mem_fd=open(mem_file,O_RDWR);
    if (mem_fd == -1) {
        perror("Failed to open /proc/<pid>/mem");
        return 1;
    }
    // 定位到目标地址
    /*
    lseek 是一个用于在文件中移动文件指针的位置函数  off_t lseek(int fd, off_t offset, int whence);
    offset 的参考点 SEEK_SET: 文件的起始位置 SEEK_CUR: 当前文件指针的位置 SEEK_END: 文件的结尾位置
    */
    if(lseek(mem_fd,addr,SEEK_SET)==-1){//这里的addr应该不是直接的虚拟地址，而是实在的文件中的位置
        perror("Failed to seek to the target address");
        close(mem_fd);
        return 1;
    }
    // 写入新值到目标内存
    if(write(mem_fd,&new_value,sizeof(new_value))!=sizeof(new_value)){
        perror("Failed to write to the target memory");
        close(mem_fd);
        return 1;
    }
    printf("Successfully wrote value %ld to address 0x%lx in process %d.\n", new_value, addr, pid);
    // 关闭文件描述符
    close(mem_fd);
    return 0;
}