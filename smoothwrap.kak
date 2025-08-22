# Configuration

declare-option -docstring "wrap at the given column" int wrap_width 80

# Internal variables

declare-option -hidden int old_column

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
    set-option global old_column %val{cursor_column}
    execute-keys x
    execute-keys %sh{ "$kak_config/plugins/smoothwrap.kak/smoothwrap" "down" "$kak_opt_old_column" "$kak_selection" "$kak_opt_wrap_width" "$kak_window_width" }<ret>
}

define-command smoothwrap-up -docstring "go up one line" %{
    set-option global old_column %val{cursor_column}
    execute-keys x
    execute-keys %sh{ "$kak_config/plugins/smoothwrap.kak/smoothwrap" "up" "$kak_opt_old_column" "$kak_selection" "$kak_opt_wrap_width" "$kak_window_width" }<ret>
}
