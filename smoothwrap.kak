# Configuration

declare-option -docstring "wrap at the given column" int wrap_width 80

# Internal variables

declare-option -hidden int cursor_column
declare-option -hidden bool go_to_previous_line
declare-option -hidden bool go_to_next_line

# commands

define-command enable-smoothwrap -docstring 'enable smoothwrap' %{
    add-highlighter global/ wrap -word -width %opt{wrap_width}

    map global normal j %{
        : smoothwrap-down
    }
    map global normal k %{
        : smoothwrap-up
    }
}

define-command smoothwrap-down -docstring "go down one line" %{
    set-option global cursor_column %val{cursor_column}
    execute-keys x
    execute-keys %sh{
        "$kak_config/plugins/smoothwrap.kak/smoothwrap" "down" "$kak_opt_cursor_column" "$kak_selection" "$kak_opt_wrap_width" "$kak_window_width"
    }<ret>
}

define-command smoothwrap-up -docstring "go up one line" %{
    set-option global cursor_column %val{cursor_column}
    execute-keys x
    execute-keys %sh{
        "$kak_config/plugins/smoothwrap.kak/smoothwrap" "up" "$kak_opt_cursor_column" "$kak_selection" "$kak_opt_wrap_width" "$kak_window_width"
    }<ret>

    execute-keys %sh{
        if [ "$kak_opt_go_to_previous_line" = "true" ]; then
            echo "x"
        fi
    }<ret>

    execute-keys %sh{
        if [ "$kak_opt_go_to_previous_line" = "true" ]; then
            "$kak_config/plugins/smoothwrap.kak/smoothwrap" "previous" "$kak_opt_cursor_column" "$kak_selection" "$kak_opt_wrap_width" "$kak_window_width"
            echo ": set-option global go_to_previous_line false"
        fi
    }<ret>
}
