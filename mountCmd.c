#include <stdio.h>  
// #include <stdlib.h>  
// #include <error.h>  
// #include <sys/types.h>  
// #include <sys/stat.h>  
// #include <fcntl.h>  
// #include <string.h>  
// #include <sys/mount.h>  
// #include <unistd.h>  
// #include <pthread.h>  
// //setMsg  
// extern void set_ALERT_Msg(char * buf);  
// #define FILE_DEV_CHECK "/proc/scsi/scsi" //用来检测设备数和类型  
// #define FILE_MOUNT_CHECK "/proc/mounts" //用来检测设备是否被mount  
// #define FILE_DISC_PARTS_CHECK "/proc/partitions" //用来检测设备的分区情况  
// #define FILE_DEV_STATU_TEMPL "/proc/scsi/usb-storage-%d/%d"   //用来检测某一设备是否在插入状态  
// #define FILE_DEV_PART_TEMPL "/dev/scsi/host%d/bus0/target0/lun0/" //具体的设备  
// #define USB_CDROM_MP "/tmp/cdrom"  
// #define USB_DISK_MP "/tmp/usbdisk"  
// #define DBG_PRINT(x) printf("[AutoMounter]");printf x  
// #define DBG_PRINT2(x) printf("[AutoMounter]");printf x  
// #define MAX_NAME_LEN 64  
// #define MAX_PART_NUM 6   //最多允许6个分区  
// typedef struct s_scsi_usb_dev  
// {  
//     int type; /*1 cdrom 2 disk */  
//     int index; /*like host0 host1*/  
//     char file_statu[MAX_NAME_LEN]; /*like "/proc/scsi/usb-storage-%d/%d"*/  
//     char devfile_parts[MAX_PART_NUM][MAX_NAME_LEN]; /*存储每个分区的设备文件*/  
//     char mount_path[MAX_PART_NUM][MAX_NAME_LEN]; /*与上面对应的mount点*/  
//     int part_num; //分区数  
//     struct s_scsi_usb_dev * next_dev; //指向下一个备  
// } SCSI_USB_DEV;  
// static int is_manual_umount = 1;  
// static SCSI_USB_DEV * f_first_dev = NULL;  
// static int CHECK_PARTS(SCSI_USB_DEV * dev);  
// static int ADD_DEV(SCSI_USB_DEV * dev);  
// static int INIT_DEV(SCSI_USB_DEV * dev, int index, char * type);  
// static void CLEAR_DEV(void);  
// static int find_device(void);  
// static int check_attach(SCSI_USB_DEV * dev);  
// static int check_mount(SCSI_USB_DEV * dev);  
// static int do_mount(SCSI_USB_DEV * dev);  
// static int do_umount(SCSI_USB_DEV * dev);  
// static int process_dev(SCSI_USB_DEV * dev);  
// static void * main_thread(void);  
// //device opt fuction  
// static int ADD_DEV(SCSI_USB_DEV * dev)  
// {  
//     if(f_first_dev)  
//     {  
//         dev->next_dev = f_first_dev;  
//         f_first_dev = dev;  
//     }  
//     else  
//     {  
//         f_first_dev = dev;  
//         dev->next_dev = NULL;  
//     }  
//     return 1;  
// }  
// static int INIT_DEV(SCSI_USB_DEV * dev, int index, char * type)  
// {  
//     dev->index = index;  
//     sprintf(dev->file_statu, FILE_DEV_STATU_TEMPL, index, index);  
//     if (!strncmp(type, "CD-ROM", 6))  
//     {//usb cd-rom  
//         dev->type = 1;  
//         dev->part_num = 1;  
//         sprintf(dev->devfile_parts[0], FILE_DEV_PART_TEMPL"cd", index);  
//         strcpy(dev->mount_path[0],USB_CDROM_MP);  
//         DBG_PRINT(("---%d---,%s,%s\n", dev->index, dev->devfile_parts[0],dev->mount_path[0]));  
//     }  
//     else  
//     {//usb disk  
//         dev->type = 2;  
//         dev->part_num = CHECK_PARTS(dev);  
//     }  
//     return 1;  
// }  
// static int CHECK_PARTS(SCSI_USB_DEV * dev)  
// {  
//     char buf[1024];  
//     char hoststr[16];  
//     int fd,len;  
//     int part_num = 0;  
//     char * line;  
//     char * delim="\n";  
//     char * strtok_tmp_ptr;  
//     char * seek;  
//     char * part_blocks;  
//     fd = open(FILE_DISC_PARTS_CHECK, O_RDONLY);  
//     if( fd > 0 )  
//     {  
//         len = read(fd, buf, sizeof(buf));  
//         close(fd);  
//         if (len > 0)  
//         {  
//             sprintf(hoststr,"scsi/host%d", dev->index);  
//             line = strtok_r(buf, delim, &strtok_tmp_ptr);  
//             while(line)  
//             {  
//                 seek = strstr(line, hoststr);  
//                 if (seek)  
//                 {  
//                     part_blocks = seek - 3;  
//                     if (strncmp(part_blocks, " 1 ", 3))  
//                     {//not extend part  
//                         sprintf(dev->devfile_parts[part_num],"/dev/%s", seek);  
//                         sprintf(dev->mount_path[part_num], USB_DISK_MP"/disk%d/part%d",dev->index,part_num);  
//                         DBG_PRINT(("---%d---,%s,%s\n", dev->index, dev->devfile_parts[part_num],dev->mount_path[part_num]));  
//                         part_num ++;  
//                         if (part_num == MAX_PART_NUM)  
//                         {  
//                             break;//too many parts ignore  
//                         }  
//                     }  
//                 }  
//                 line = strtok_r(NULL, delim, &strtok_tmp_ptr);  
//             }  
//         }  
//     }  
//     else  
//     {  
//         perror(FILE_DISC_PARTS_CHECK);  
//     }  
//     return part_num;  
// }  
// static void CLEAR_DEV(void)  
// {  
//     SCSI_USB_DEV * cur_dev = f_first_dev;  
//     SCSI_USB_DEV * tmp_dev;  
//     while (cur_dev)  
//     {  
//         tmp_dev = cur_dev;  
//         cur_dev = cur_dev->next_dev;  
//         free(tmp_dev);  
//     }  
//     f_first_dev = NULL;  
// }  
// //  
// static int find_device()  
// {//搜索设备加入列表  
//     char buf[1024];  
//     int fd;  
//     int len;  
//     int dev_num = 0;  
//     char * seek = NULL;  
//     SCSI_USB_DEV * new_dev;  
//     //clear exist device  
//     CLEAR_DEV();  
//     //add new device  
//     fd = open(FILE_DEV_CHECK, O_RDONLY);  
//     if( fd > 0 )  
//     {  
//         len = read(fd, buf, sizeof(buf));  
//         close(fd);  
//         if ( len > 0 )  
//         {  
//             //printf("%d\n", len);  
//             seek = buf;  
//             while(seek)  
//             {  
//                 seek = strstr(seek, "Host: scsi");  
//                 if (seek)  
//                 {  
//                     seek += strlen( "Host: scsi");  
//                     seek = strstr(seek, "Type:");  
//                     if (seek)  
//                     {  
//                         seek += strlen("Type:");  
//                         while(*seek == ' ') seek++;  
//                         new_dev = malloc(sizeof(SCSI_USB_DEV));  
//                         INIT_DEV(new_dev, dev_num, seek);  
//                         ADD_DEV(new_dev);        
//                         dev_num ++;  
//                     }  
//                 }  
//             }  
//             DBG_PRINT(("dev_num = %d\n", dev_num));  
//         }  
//     }  
//     else  
//     {  
//         perror(FILE_DEV_CHECK);  
//     }  
//     return dev_num;  
// }  
// static int check_attach(SCSI_USB_DEV * dev)  
// {//检测设备是否连接  
//     //linux中只要设备被插入过就会记住该设备  
//     //只能通过Attached来判断是否连接  
//     char buf[512];  
//     int fd;  
//     int len;  
//     char * seek;  
//     fd = open(dev->file_statu, O_RDONLY);  
//     if( fd > 0 )  
//     {  
//         len = read(fd, buf, sizeof(buf));  
//         close(fd);  
//         if ( len > 0 )  
//         {  
//             seek = strstr(buf, "Attached:");  
//             if (seek)  
//             {  
//                 seek += strlen( "Attached:");  
//                 while(*seek == ' ') seek++;  
//                 return *seek=='Y';  
//             }  
//         }  
//     }  
//     else  
//     {  
//         perror(dev->file_statu);  
//     }  
//     return 0;  
// }  
// static int check_mount(SCSI_USB_DEV * dev)  
// {  
//     char buf[1024];  
//     int fd;  
//     int len;  
//     char * seek;  
//     int i = 0;  
//     fd = open(FILE_MOUNT_CHECK, O_RDONLY);  
//     if( fd > 0 )  
//     {  
//         len = read(fd, buf, sizeof(buf));  
//         close(fd);  
//         if ( len > 0 )  
//         {  
//             buf[len] = '\0';  
//             if(dev->type == 2 && dev->part_num > 1) i ++; /*if disk ignore first part disc*/  
//             for(; i<dev->part_num; i++)  
//             {  
//                 //printf("index=%d type=%d %s\n",dev->index,dev->type,dev->devfile_parts[i]);  
//                 seek = strstr(buf, dev->devfile_parts[i]);  
//                 if (seek!=NULL) return 1;/*have one part mounted return 1*/  
//             }  
//         }  
//     }  
//     else  
//     {  
//         perror(FILE_MOUNT_CHECK);  
//     }  
//     return 0;  
// }  
// static int do_mount(SCSI_USB_DEV * dev)  
// {//自动mount 有进行vcd 和 cd的检查并用 cdfs mount  
//     int i = 0;  
//     char fstype[10];  
//     unsigned long mountflags=0;  
//     char mount_data[30];  
//     char tmpdir[50];  
//     int mount_ok = 0;  
//     int is_vcd = 0;  
//     char check_cmd[50];  
//     if(dev->type == 1 && is_manual_umount == 0)  
//     {  
//         //mkdir(dev->mount_path[0], 0777);  
//         strcpy(fstype, "iso9660");  
//         mountflags= 0xc0ed0000 |MS_RDONLY ;  
//         strcpy(mount_data,"codepage=936,iocharset=gb2312");  
//         if (mount(dev->devfile_parts[0], dev->mount_path[0], fstype ,mountflags, mount_data)== 0)  
//         {  
//             mount_ok = 1;  
//             DBG_PRINT2(("mount -t %s %s %s success\n", fstype, dev->devfile_parts[0], dev->mount_path[0]));  
//             //check is vcd  
//             sprintf(check_cmd,"ls %s/vcd/*.vcd", dev->mount_path[0]);  
//             is_vcd = (system(check_cmd) == 0);  
//             if (is_vcd)  
//             {  
//                 if (umount(dev->mount_path[0]) == 0)  
//                 {  
//                     DBG_PRINT2(("umount %s success(vcd iso9660)\n", dev->devfile_parts[0]));  
//                 }  
//                 else  
//                 {  
//                     is_vcd = 0;  
//                     DBG_PRINT2(("umount %s failed (vcd iso9660)\n", dev->devfile_parts[0]));  
//                 }  
//             }  
//         }  
//         else  
//         {  
//             mount_ok = 0;  
//         }  
//         if (mount_ok == 0 || is_vcd)  
//         {  
//             DBG_PRINT2(("mount -t %s %s %s failed, try cdfs\n", fstype, dev->devfile_parts[0], dev->mount_path[0]));  
//             strcpy(fstype, "cdfs");  
//             if (mount(dev->devfile_parts[0], dev->mount_path[0], fstype ,mountflags, mount_data)== 0)  
//             {  
//                 DBG_PRINT2(("mount -t %s %s %s success\n", fstype, dev->devfile_parts[0], dev->mount_path[0]));  
//                 return 1;  
//             }  
//             else  
//             {  
//                 DBG_PRINT2(("mount -t %s %s %s failed, try cdfs\n", fstype, dev->devfile_parts[0], dev->mount_path[0]));  
//                 return 0;  
//             }  
//         }  
//     }  
//     else if (dev->type == 2)  
//     {  
//         sprintf(tmpdir, USB_DISK_MP"/disk%d", dev->index);  
//         mkdir(tmpdir, 0777);  
//         strcpy(fstype, "vfat");  
//         mountflags= 0xc0ed0000;  
//         strcpy(mount_data,"codepage=936,iocharset=gb2312");  
//         if (dev->part_num > 1)  
//             i ++; /*if disk ignore first part disc*/  
//         for(; i<dev->part_num; i++) /*if disk ignore first part disc*/  
//         {  
//             mkdir(dev->mount_path[i], 0777);  
//             if (mount(dev->devfile_parts[i], dev->mount_path[i], fstype ,mountflags, mount_data)== 0)  
//             {  
//                 DBG_PRINT2(("mount %s %s success\n", dev->devfile_parts[i], dev->mount_path[i]));  
//                 Do_USB_Disk_Update(dev->mount_path[i]);  
//             }  
//             else  
//             {  
//                 rmdir(dev->mount_path[i]);  
//                 DBG_PRINT2(("mount %s %s failed\n", dev->devfile_parts[i], dev->mount_path[i]));  
//             }  
//         }  
//     }  
//     else  
//     {  
//         return 0;  
//     }  
//     return 1;  
// }  
// static int do_umount(SCSI_USB_DEV * dev)  
// {  
//     int i = 0;  
//     char tmpdir[50];  
//     if (dev->type == 1)  
//     {//cdrom  
//         if (umount(dev->mount_path[0]) == 0)  
//         {  
//             DBG_PRINT2(("umount %s success\n", dev->devfile_parts[0]));  
//             return 1;  
//         }  
//         else  
//         {  
//             DBG_PRINT2(("umount %s failed\n", dev->devfile_parts[0]));  
//             return 0;  
//         }  
//     }  
//     else if(dev->type == 2)  
//     {  
//         if (dev->part_num > 1)  
//             i ++; /*if disk ignore first part disc*/  
//         for(; i<dev->part_num; i++)  
//         {  
//             if (umount(dev->mount_path[i]) == 0)  
//             {  
//                 DBG_PRINT2(("umount %s success\n", dev->devfile_parts[i]));  
//                 remove(dev->mount_path[i]);  
//             }  
//             else  
//             {  
//                 DBG_PRINT2(("umount %s failed\n", dev->devfile_parts[i]));  
//             }  
//         }  
//         sprintf(tmpdir, USB_DISK_MP"/disk%d", dev->index);  
//         remove(tmpdir);  
//     }  
//     return 1;  
// }  
// static int process_dev(SCSI_USB_DEV * dev)  
// {  
//     if (check_attach(dev))//检测设备是否插上  
//     {  
//         if (!check_mount(dev))//检测设备是否mount上了  
//         {  
//             do_mount(dev);  
//         }  
//     }  
//     else  
//     {  
//         if (check_mount(dev))  
//         {  
//             do_umount(dev);  
//         }  
//     }  
//     return 1;  
// }  
// void manual_umount(void);  
// void manual_umount()//争对光驱进行umount  
// {  
//     /* now only umount cdrom*/  
//     SCSI_USB_DEV * cur_dev = NULL;  
//     cur_dev = f_first_dev;  
//     while (cur_dev)  
//     {  
//         if (cur_dev->type == 1 ) break;  
//         cur_dev = cur_dev->next_dev;  
//     }  
//     if (cur_dev != NULL)  
//     {  
//         is_manual_umount = 1;  
//         if (check_mount(cur_dev))  
//         {  
//             if (do_umount(cur_dev))  
//             {  
//                 set_ALERT_Msg("光驱卸载成功");  
//             }  
//         }  
//     }  
// }  
// void manual_mount(void);  
// void manual_mount()//争对光驱进行mount  
// {  
//     /* now only umount cdrom*/  
//     SCSI_USB_DEV * cur_dev = NULL;  
//     cur_dev = f_first_dev;  
//     while (cur_dev)  
//     {  
//         if (cur_dev->type == 1 ) break;  
//         cur_dev = cur_dev->next_dev;  
//     }  
//     if (cur_dev != NULL)  
//     {  
//         is_manual_umount = 0;  
//         if (!check_mount(cur_dev))  
//         {  
//             do_mount(cur_dev);  
//         }  
//     }  
// }  
// #ifndef DBG  
// int Init_AutoMounter(void);  
// int Init_AutoMounter()  
// {  
//     int ret;  
//     pthread_t thread_automounter;  
//     ret = pthread_create(&thread_automounter, NULL, (void *) main_thread, NULL);  
//     if(ret != 0){  
//         printf ("Create pthread error!\n");  
//     }  
//     return ret;  
// }  
// static void * main_thread(void)  
// #else  
int main()  
// #endif  
{  
//     SCSI_USB_DEV * cur_dev = NULL;  
//     mkdir(USB_DISK_MP, 0777);  
//     mkdir(USB_CDROM_MP, 0777);  
//     while (1)  
//     {  
//         find_device();//查找设备 初始化设备列表  
//         cur_dev = f_first_dev;  
//         while (cur_dev)  
//         {  
//             process_dev(cur_dev);//对每个设备进行处理  
//             cur_dev = cur_dev->next_dev;  
//         }  
//         sleep(10);//10秒钟检测一次  
//         //对于u盘和移动硬盘定时检测并自动mount和umount  
//     }  
}  