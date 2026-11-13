_starhex_completions() {
    local cur prev opts targets
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    opts="-h --help -v --version -p --prefix -np --no-prefix -u --upper -l --lower -q --quiet -a --all -c --color -nc --no-color -w --width -g --group -t -f -e16 -e32 -e64 --endian --not --and --or --xor --shl --shr"
    targets="-t2 -t8 -t10 -t16 -tb -to -td -tx -th -thex -ts -tstr -tb64 -trgb -thsv -tip -tfloat -tdouble -ttime"

    if [[ ${cur} == -t* ]]; then
        COMPREPLY=( $(compgen -W "${targets}" -- ${cur}) )
        return 0
    fi

    if [[ ${cur} == -* ]]; then
        COMPREPLY=( $(compgen -W "${opts} ${targets}" -- ${cur}) )
        return 0
    fi
}
complete -F _starhex_completions starhex
