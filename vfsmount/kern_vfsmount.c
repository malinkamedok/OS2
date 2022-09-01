#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/filter.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/mount.h>

MODULE_LICENSE("GPL");

#define BUFFER_SIZE 256

#define VFSMOUNT_MESSAGE   "mnt_flags = %d\n" \
            "mnt_sb -> s_blocksize = %lu\n" \
            "mnt_sb -> s_count = %u\n"
               
static struct dentry *debugfs_dir;
static struct dentry *debugfs_args;
static struct dentry *debugfs_vfs;
static int fd = 1;

static struct vfsmount *get_vfsmount(int fdesc) {
   struct fd f = fdget(fdesc);
   if (!f.file) {
      printk(KERN_INFO"Error opening a file by fd");
      return NULL;
   }
   struct vfsmount *vfs = f.file->f_path.mnt;
   return vfs;
}

static ssize_t vfs_to_user(struct file *flip, char __user *usr_buffer, size_t length, loff_t *offset) {
   char kern_buffer[BUFFER_SIZE];
   if (*offset > 0 || BUFFER_SIZE > length) return -EFAULT;

   struct vfsmount *vfs = get_vfsmount(fd);
   if (vfs == NULL) strcpy(kern_buffer, "Could't extract vfsmount by provided fd\n");
   else {
      sprintf(kern_buffer, VFSMOUNT_MESSAGE, vfs->mnt_flags, vfs->mnt_sb->s_blocksize, vfs->mnt_sb->s_count);       
   }

   length = strlen(kern_buffer) + 1;
   *offset += length;

   if (copy_to_user(usr_buffer, kern_buffer, length)) return -EFAULT;
   return *offset;
}

static ssize_t args_from_user(struct file *filp, const char __user *usr_buffer, size_t length, loff_t *offset) {
   int arg;
   char kern_buffer[BUFFER_SIZE];

   if (*offset > 0 || length > BUFFER_SIZE || copy_from_user(kern_buffer, usr_buffer, length)) return -EFAULT;

   int r_status = sscanf(kern_buffer, "%d", &arg);

   if (r_status < 1) {
      fd = -1;
      return -EFAULT;
   }
   
   fd = arg;
   *offset = strlen(kern_buffer);
   return *offset;
}

static const struct file_operations args_file_ops = {
   .owner = THIS_MODULE,
   .write = args_from_user
};

static const struct file_operations res_vfsmount_file_ops = {
   .owner = THIS_MODULE,
   .read = vfs_to_user,
};

static int __init lab2_init(void) {
   printk(KERN_INFO"Module initialization\n");
   debugfs_dir = debugfs_create_dir("lab2os", NULL);
   debugfs_args = debugfs_create_file("args", 0666, debugfs_dir, NULL, &args_file_ops);
   debugfs_vfs = debugfs_create_file("vfs", 0666, debugfs_dir, NULL, &res_vfsmount_file_ops);
   return 0;
}

static void __exit lab2_exit(void) {
   debugfs_remove_recursive(debugfs_dir);
}

module_init(lab2_init);
module_exit(lab2_exit);