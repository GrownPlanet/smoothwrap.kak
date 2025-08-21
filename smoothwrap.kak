# Configuration

declare-option -docstring "wrap at the given column" int wrap_width 80

# Internal variables

declare-option -hidden int old_column

# commands

define-command enable-smoothwrap -docstring 'enable smoothwrap' %{
    add-highlighter global/ wrap -word -width %opt{wrap_width}

    map global normal j %{
        : set-option global old_column %val{cursor_column}
        x
        : execute-keys %sh{ ./smoothwrap "down" "$kak_opt_old_column" "$kak_selection" "$kak_opt_wrap_width" "$kak_window_width" }<ret>
    }

    map global normal k %{
        : set-option global old_column %val{cursor_column}
        x
        : execute-keys %sh{ ./smoothwrap "up" "$kak_opt_old_column" "$kak_selection" "$kak_opt_wrap_width" "$kak_window_width" }<ret>
    }
}
