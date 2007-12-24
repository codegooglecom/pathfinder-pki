/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2005-2006 Net Integration Technologies, Inc.
 *
 * Pathfinder Software:
 *   Copyright (C) 2007, Carillon Information Security Inc.
 *
 * This library is licensed under the LGPL, please read LICENSE for details.
 * 
 */

#include "wvdbuswatch.h"


WvDBusWatch::WvDBusWatch(DBusWatch *_watch, unsigned int _flags) :
    watch(_watch)
{
    int fd = dbus_watch_get_unix_fd(watch);
    rfd = wfd = fd;

    if (!(_flags & DBUS_WATCH_READABLE))
    {
        stop_read = true;
        rfd = -1;
    }
    if (!(_flags & DBUS_WATCH_WRITABLE))
    {
        stop_write = true;
        wfd = -1;
    }
}


void WvDBusWatch::pre_select(SelectInfo &si)
{
    //log("preselect.. enabled: %s\n", dbus_watch_get_enabled(watch));
    si.wants.writable |= (wfd >= 0 && dbus_watch_get_enabled(watch));

    WvFdStream::pre_select(si);
}


void WvDBusWatch::execute()
{
    unsigned int dbus_condition = 0;
    //log("executing watch (rfd: %s wfd: %s)\n", rfd, wfd);

    if (isreadable())
        dbus_condition |= DBUS_WATCH_READABLE;
    if (iswritable())
         dbus_condition |= DBUS_WATCH_WRITABLE;
    // FIXME: Handle errors, HUP

    if (dbus_condition != 0 && dbus_watch_get_enabled(watch))
        dbus_watch_handle(watch, dbus_condition);
}
