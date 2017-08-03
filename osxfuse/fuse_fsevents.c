/*
 * Copyright (c) 2017 EditShare LLC
 * All rights reserved.
 */

#include "fuse_fsevents.h"
#include "fuse_internal.h"

#include <libkern/version.h>

#define VNODE_EVENT_DELETE         0x00000001
#define VNODE_EVENT_WRITE          0x00000002
#define VNODE_EVENT_EXTEND         0x00000004
#define VNODE_EVENT_ATTRIB         0x00000008
#define VNODE_EVENT_LINK           0x00000010
#define VNODE_EVENT_RENAME         0x00000020
#define VNODE_EVENT_PERMS          0x00000040
#define VNODE_EVENT_FILE_CREATED   0x00000080
#define VNODE_EVENT_DIR_CREATED    0x00000100
#define VNODE_EVENT_FILE_REMOVED   0x00000200
#define VNODE_EVENT_DIR_REMOVED    0x00000400

void
fuse_vnotify(vnode_t vp)
{
#if VERSION_MAJOR >= 15
    struct vnode_attr vattr;
    vfs_get_notify_attributes(&vattr);

#if M_OSXFUSE_ENABLE_BIG_LOCK
    struct fuse_data *data = fuse_get_mpdata(vnode_mount(vp));
    fuse_biglock_lock(data->biglock);
#endif

    fuse_internal_attr_loadvap(vp, &vattr, NULL);

#if M_OSXFUSE_ENABLE_BIG_LOCK
    fuse_biglock_unlock(data->biglock);
#endif

    vnode_notify(vp, VNODE_EVENT_ATTRIB | VNODE_EVENT_WRITE, &vattr);
#else
    (void) vp; // Unused parameter.
#endif
}

