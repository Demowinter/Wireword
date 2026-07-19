#include <array>
#include <unistd.h>
#include <sys/epoll.h>
#include <def.hpp>
#include <eventloop.hpp>

using EPollEvent = epoll_event;

EventLoop::EventLoop() {
    epollfd = ::epoll_create1(0);
}

EventLoop::~EventLoop() {
    if (epollfd != invalidDesc) ::close(epollfd);
}

void EventLoop::run() {
    runflag = true;

    loop();
}

void EventLoop::stop() {
    runflag = false;
}

void EventLoop::addDescriptor(FileDescriptor fd, EventType mask, EventCallbacks cbs) {
    eventCallbacks[fd] = cbs;

    EPollEvent event;
    event.events = static_cast<uint32_t>(mask);
    event.data.fd = fd;

    ::epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}

void EventLoop::removeDescriptor(FileDescriptor fd, bool syscall) {
    eventCallbacks.erase(fd);

    if (syscall) ::epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);
}

void EventLoop::setEventMask(FileDescriptor fd, EventType mask) {
    EPollEvent event;
    event.events = static_cast<uint32_t>(mask);
    event.data.fd = fd;

    ::epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

void EventLoop::setEventCallbacks(FileDescriptor fd, EventCallbacks cbs) {
    eventCallbacks.at(fd) = cbs;
}

void EventLoop::addDeferredCallbacks(DeferredCallback cb) {
    deferredCallbacks.push_back(cb);
}

void EventLoop::setWaitTimeout(int32_t ms) {
    timeoutMS = ms;
}

void EventLoop::loop() {
    std::array<EPollEvent, 64> eventBuffer;

    while (runflag) {
        int32_t eventCount = ::epoll_wait(epollfd, eventBuffer.data(), eventBuffer.size(), timeoutMS);

        for (int32_t i = 0; i < eventCount; i++) {
            EPollEvent event = eventBuffer[i];
            EventCallbacks& cbs = eventCallbacks.at(event.data.fd); 

            if (event.events & static_cast<uint32_t>(EventType::INPUT) && cbs.input) cbs.input(event.data.fd);
            if (event.events & static_cast<uint32_t>(EventType::OUTPUT) && cbs.output) cbs.output(event.data.fd);
            if (event.events & static_cast<uint32_t>(EventType::ERROR | EventType::HANGUP) && cbs.error) cbs.error(event.data.fd);
        }

        for (DeferredCallback& cb : deferredCallbacks) if (cb) cb();
    }
}