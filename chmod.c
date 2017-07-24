#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
int main(int argc,char **argv)
{
	if(argc != 2)
	{
		printf("参数错误\n");
		return -1;
	}
    chmod(argv[1], S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
    return 0;
}




// 参数 mode 有下列数种组合：
// 1、S_ISUID 04000 文件的 (set user-id on execution)位
// 2、S_ISGID 02000 文件的 (set group-id on execution)位
// 3、S_ISVTX 01000 文件的sticky 位
// 4、S_IRUSR (S_IREAD) 00400 文件所有者具可读取权限
// 5、S_IWUSR (S_IWRITE)00200 文件所有者具可写入权限
// 6、S_IXUSR (S_IEXEC) 00100 文件所有者具可执行权限
// 7、S_IRGRP 00040 用户组具可读取权限
// 8、S_IWGRP 00020 用户组具可写入权限
// 9、S_IXGRP 00010 用户组具可执行权限
// 10、S_IROTH 00004 其他用户具可读取权限
// 11、S_IWOTH 00002 其他用户具可写入权限
// 12、S_IXOTH 00001 其他用户具可执行权限

// 注：只有该文件的所有者或有效用户识别码为0，才可以修改该文件权限。
// 基于系统安全，如果欲将数据写入一执行文件，而该执行文件具有S_ISUID 或S_ISGID 权限，则这两
// 个位会被清除。如果一目录具有S_ISUID 位权限，表示在此目录下只有该文件的所有者或root 可以删除该文件。
// 返回值：权限改变成功返回0, 失败返回-1, 错误原因存于errno.

// 错误代码：
// 1、EPERM 进程的有效用户识别码与欲修改权限的文件拥有者不同, 而且也不具root 权限.
// 2、EACCESS 参数path 所指定的文件无法存取.
// 3、EROFS 欲写入权限的文件存在于只读文件系统内.
// 4、EFAULT 参数path 指针超出可存取内存空间.
// 5、EINVAL 参数mode 不正确
// 6、ENAMETOOLONG 参数path 太长
// 7、ENOENT 指定的文件不存在
// 8、ENOTDIR 参数path 路径并非一目录
// 9、ENOMEM 核心内存不足
// 10、ELOOP 参数path 有过多符号连接问题.
// 11、EIO I/O 存取错误
