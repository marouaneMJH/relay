#!/usr/bin/env bash

fib=(1 2 3 5 8 13 21 34 55 89 144 233)

output="results.md"

echo "| N | sent | recv | drop | msg/s | p50(ns) | p95(ns) | p99(ns) |" > "$output"
echo "|---|------|------|------|-------|---------|---------|---------|" >> "$output"

for n in "${fib[@]}"; do
    line=$(build/relay "$n" | grep -E 'sent=')
    if [[ -z "$line" ]]; then
        echo "| $n | ERROR | ERROR | ERROR | ERROR | ERROR | ERROR | ERROR |" >> "$output"
        continue
    fi

    # extract values
    sent=$(echo "$line" | grep -oE 'sent=[0-9]+' | cut -d= -f2)
    recv=$(echo "$line" | grep -oE 'recv=[0-9]+' | cut -d= -f2)
    drop=$(echo "$line" | grep -oE 'drop=[0-9]+' | cut -d= -f2)
    msg=$(echo "$line" | grep -oE 'msg/s=[0-9. ]+' | cut -d= -f2)
    p50=$(echo "$line" | grep -oE 'p50\(ns\)=[0-9]+' | cut -d= -f2)
    p95=$(echo "$line" | grep -oE 'p95\(ns\)=[0-9]+' | cut -d= -f2)
    p99=$(echo "$line" | grep -oE 'p99\(ns\)=[0-9]+' | cut -d= -f2)

    echo "| $n | $sent | $recv | $drop | $msg | $p50 | $p95 | $p99 |" >> "$output"
done

echo "Done → $output"
