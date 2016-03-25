/* -*- Mode: C++; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#ifndef RR_WAIT_STATUS_H_
#define RR_WAIT_STATUS_H_

#include <signal.h>

#include <memory>
#include <vector>

namespace rr {

class WaitStatus {
public:
  WaitStatus(int status = 0) : status(status) {}

  enum Type {
    // Task exited normally.
    EXIT,
    // Task exited due to fatal signal.
    FATAL_SIGNAL,
    // Task is in a signal-delivery-stop.
    SIGNAL_STOP,
    // Task is in a group-stop. (See ptrace man page.)
    // You must use PTRACE_SEIZE to generate PTRACE_EVENT_STOPs, or these
    // will be treated as STOP_SIGNAL.
    GROUP_STOP,
    // Task is in a syscall-stop triggered by PTRACE_SYSCALL
    // and PTRACE_O_TRACESYSGOOD.
    SYSCALL_STOP,
    // Task is in a PTRACE_EVENT stop, except for PTRACE_EVENT_STOP
    // which is treated as GROUP_STOP.
    PTRACE_EVENT
  };

  Type type() const;

  // Exit code if type() == EXIT, otherwise -1.
  int exit_code() const;
  // Fatal signal if type() == FATAL_SIGNAL, otherwise zero.
  int fatal_sig() const;
  // Stop signal if type() == STOP_SIGNAL, otherwise zero. A zero signal
  // (rare but observed via PTRACE_INTERRUPT) is converted to SIGSTOP.
  int stop_sig() const;
  // Stop signal if type() == GROUP_STOP, otherwise zero. A zero signal
  // (rare but observed via PTRACE_INTERRUPT) is converted to SIGSTOP.
  int group_stop() const;
  bool is_syscall() const;
  // ptrace event if type() == PTRACE_EVENT, otherwise zero.
  int ptrace_event() const;

  int get() const { return status; }

  static WaitStatus for_ptrace_event(int ptrace_event) {
    return (ptrace_event << 16) | ((0x80 | SIGTRAP) << 8) | 0x7f;
  }

private:
  int status;
};

std::ostream& operator<<(std::ostream& stream, WaitStatus status);

} // namespace rr

#endif /* RR_WAIT_STATUS_H_ */