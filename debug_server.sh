#!/bin/bash

cd /data/workspace/github/Pantheon/one_send_one_recv_struct_msg/build

echo "Starting server with exit status monitoring..."
./one_send_one_recv_server_struct_msg
EXIT_CODE=$?

echo "=== Server terminated ==="
echo "Exit code: $EXIT_CODE"

if [ $EXIT_CODE -eq 0 ]; then
    echo "Normal exit (return 0)"
elif [ $EXIT_CODE -gt 128 ]; then
    SIGNAL=$((EXIT_CODE - 128))
    echo "Terminated by signal: $SIGNAL"
    case $SIGNAL in
        1) echo "Signal: SIGHUP (Hangup)" ;;
        2) echo "Signal: SIGINT (Interrupt)" ;;
        9) echo "Signal: SIGKILL (Kill)" ;;
        13) echo "Signal: SIGPIPE (Broken pipe)" ;;
        15) echo "Signal: SIGTERM (Terminate)" ;;
        *) echo "Signal: Unknown ($SIGNAL)" ;;
    esac
else
    echo "Program returned: $EXIT_CODE"
fi
