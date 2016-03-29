/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "SHIM_BitTude"

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <fcntl.h>
#include <unistd.h>

#include <utils/Errors.h>

#include <binder/Parcel.h>

#include <gui/BitTube.h>

namespace android {
// ----------------------------------------------------------------------------

// Socket buffer size.  The default is typically about 128KB, which is much larger than
// we really need.  So we make it smaller.
static const size_t SOCKET_BUFFER_SIZE = 4 * 1024;


BitTube::BitTube()
    : mSendFd(-1), mReceiveFd(-1)
{
    int sockets[2];
    if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, sockets) == 0) {
        int size = SOCKET_BUFFER_SIZE;
        setsockopt(sockets[0], SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
        setsockopt(sockets[0], SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
        setsockopt(sockets[1], SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
        setsockopt(sockets[1], SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
        fcntl(sockets[0], F_SETFL, O_NONBLOCK);
        fcntl(sockets[1], F_SETFL, O_NONBLOCK);
        mReceiveFd = sockets[0];
        mSendFd = sockets[1];
    } else {
        mReceiveFd = -errno;
        ALOGE("BitTube: pipe creation failed (%s)", strerror(-mReceiveFd));
    }
}

BitTube::BitTube(const Parcel& data)
    : mSendFd(-1), mReceiveFd(-1)
{
    mReceiveFd = dup(data.readFileDescriptor());
    if (mReceiveFd >= 0) {
        int size = SOCKET_BUFFER_SIZE;
        setsockopt(mReceiveFd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
        setsockopt(mReceiveFd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
        fcntl(mReceiveFd, F_SETFL, O_NONBLOCK);
    } else {
        mReceiveFd = -errno;
        ALOGE("BitTube(Parcel): can't dup filedescriptor (%s)",
                strerror(-mReceiveFd));
    }
}

BitTube::~BitTube()
{
    if (mSendFd >= 0)
        close(mSendFd);

    if (mReceiveFd >= 0)
        close(mReceiveFd);
}

status_t BitTube::initCheck() const
{
    if (mReceiveFd < 0) {
        return status_t(mReceiveFd);
    }
    return NO_ERROR;
}

int BitTube::getFd() const
{
    return mReceiveFd;
}

ssize_t BitTube::write(void const* vaddr, size_t size)
{
    ssize_t err, len;
    do {
        len = ::send(mSendFd, vaddr, size, MSG_DONTWAIT | MSG_NOSIGNAL);
        err = len < 0 ? errno : 0;
    } while (err == EINTR);
    return err == 0 ? len : -err;

}

ssize_t BitTube::read(void* vaddr, size_t size)
{
    ssize_t err, len;
    do {
        len = ::recv(mReceiveFd, vaddr, size, MSG_DONTWAIT);
        err = len < 0 ? errno : 0;
    } while (err == EINTR);
    if (err == EAGAIN || err == EWOULDBLOCK) {
        // EAGAIN means that we have non-blocking I/O but there was
        // no data to be read. Nothing the client should care about.
        return 0;
    }
    return err == 0 ? len : -err;
}

status_t BitTube::writeToParcel(Parcel* reply) const
{
    if (mReceiveFd < 0)
        return -EINVAL;

    status_t result = reply->writeDupFileDescriptor(mReceiveFd);
    close(mReceiveFd);
    mReceiveFd = -1;
    return result;
}


ssize_t BitTube::sendObjects(const sp<BitTube>& tube,
        void const* events, size_t count, size_t objSize)
{
    ssize_t numObjects = 0;
    for (size_t i=0 ; i<count ; i++) {
        const char* vaddr = reinterpret_cast<const char*>(events) + objSize * i;
        ssize_t size = tube->write(vaddr, objSize);
        if (size < 0) {
            // error occurred
            return size;
        } else if (size == 0) {
            // no more space
            break;
        }
        numObjects++;
    }
    return numObjects;
}

ssize_t BitTube::recvObjects(const sp<BitTube>& tube,
        void* events, size_t count, size_t objSize)
{
    ssize_t numObjects = 0;
    for (size_t i=0 ; i<count ; i++) {
        char* vaddr = reinterpret_cast<char*>(events) + objSize * i;
        ssize_t size = tube->read(vaddr, objSize);
        if (size < 0) {
            // error occurred
            return size;
        } else if (size == 0) {
            // no more messages
            break;
        }
        numObjects++;
    }
    return numObjects;
}

// ----------------------------------------------------------------------------
}; // namespace android
