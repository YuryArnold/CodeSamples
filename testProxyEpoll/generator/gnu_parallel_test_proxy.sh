#!/usr/bin/env bash
# export postgres connection parameters
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
source "$SCRIPT_DIR/psql_base_conf.sh"
# use connection over proxy
export PGHOST="$PROXY_HOST"
export PGPORT="$PROXY_PORT"

function base_proxy_script() {
    local ret_code=0
    local job_no="$1"
    local sql_expression
    echo "Run base_proxy_script with args: $*" >&2

    local script_no
    script_no=$((job_no % 5))
    case "$script_no" in
    0 | 1)
        sql_expression="SELECT NOW();"
        ;;
    2 | 3)
        sql_expression="SELECT COUNT(*) from demo_table;"
        ;;
    4)
        sql_expression="SELECT * from demo_table WHERE UPPER(col_1) LIKE 'A%' ORDER BY col_1, col_2 LIMIT 10"
        ;;
    5)
        sql_expression="SELECT COUNT(*) from demo_table WHERE UPPER(col_1) LIKE '0%'"
        ;;
    *)
        echo "ERROR: unknown script number $script_no"
        ;;
    esac

    psql --command "$sql_expression" >&2 || ret_code=$?

    if [[ "$ret_code" -ne 0 ]]; then
        echo "FATAL ERROR: psql has failed with a code $ret_code" >&2
        echo "INFO: script: $sql_expression" >&2
        return "$ret_code"
    fi
}

export -f base_proxy_script

PARALLEL_LEVEL=50

function infinite_counter() {
    local counter=0
    while true; do
        echo "$counter"
        counter=$((counter + 1))
    done
}

infinite_counter | parallel --line-buffer --halt now,fail=1 --jobs "$PARALLEL_LEVEL" base_proxy_script '{}'
