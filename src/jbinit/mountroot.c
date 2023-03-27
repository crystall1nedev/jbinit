#include <jbinit.h>
#include <common.h>

void mountroot(char* rootdev, uint64_t rootlivefs, int rootopts) {
    struct stat statbuf;
    char buf[0x100];
    struct apfs_mountarg arg = {
        rootdev,
        0,
        rootlivefs, // 1 mount without snapshot, 0 mount snapshot
        0,
    };
retry_rootfs_mount:
    LOG("mounting rootfs %s\n", rootdev);
    int err = mount("apfs", "/", rootopts | MNT_RDONLY, &arg);
    if (!err) {
      LOG("mount rootfs OK\n");
    } else {
      LOG("mount rootfs %s FAILED with err=%d!\n", rootdev, err);
      sleep(1);
      goto retry_rootfs_mount;
      // spin();
    }
    
    int err2 = stat("/private/", &statbuf);
    if (!err2) {
      LOG("stat %s OK\n", "/private/");
    } else {
      LOG("stat %s FAILED with err=%d!\n", "/private/", err);
      sleep(1);
      goto retry_rootfs_mount;
    }
}

void unmount_root() {
  LOG("unmounting rootfs\n");
  int ret = unmount("/", MNT_FORCE);
  if (ret) {
    LOG("umount(/) failed with err=%d!\n", ret);
    spin();
  } else {
    LOG("unmounted rootfs\n");
  }
}
