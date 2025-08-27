# Configuration

declare-option -docstring "wrap at the given column" int wrap_width 80
declare-option -docstring "wether to remap 'gh' and 'gl'" bool map_jumps true

# Internal variables

declare-option -hidden int cursor_column
declare-option -hidden str smoothwrap_command
declare-option -hidden int visual_column
declare-option -hidden bool go_to_previous_line
declare-option -hidden bool go_to_next_line

# commands

define-command enable-smoothwrap -docstring "enable smoothwrap" %{
    add-highlighter global/ wrap -word -width %opt{wrap_width}

    map global normal j ": smoothwrap-down<ret>"
    map global normal k ": smoothwrap-up<ret>"

    evaluate-commands %sh{
        if [ "$kak_opt_map_jumps" = "true" ]; then
            echo "map global goto l -docstring 'visual line end' '<esc>: smoothwrap-end<ret>'"
            echo "map global goto h -docstring 'visual line begin' '<esc>: smoothwrap-begin<ret>'"
        fi
    }
}

define-command smoothwrap-down -docstring "go down one line" %{
    set-option global smoothwrap_command "down"
    smoothwrap-command

    execute-keys %sh{
        if [ "$kak_opt_go_to_next_line" = "true" ];
            then echo "x"
        fi
    }<ret>

    execute-keys %sh{
        if [ "$kak_opt_go_to_next_line" = "true" ]; then
            "$kak_config/plugins/smoothwrap.kak/smoothwrap" "next" "$kak_opt_visual_column" "$kak_selection" "$kak_opt_wrap_width"
            echo ": set-option global go_to_next_line false"
        fi
    }<ret>
}

define-command smoothwrap-up -docstring "go up one line" %{
    set-option global smoothwrap_command "up"
    smoothwrap-command

    execute-keys %sh{
        if [ "$kak_opt_go_to_previous_line" = "true" ];
            then echo "x"
        fi
    }<ret>

    execute-keys %sh{
        if [ "$kak_opt_go_to_previous_line" = "true" ]; then
            "$kak_config/plugins/smoothwrap.kak/smoothwrap" "previous" "$kak_opt_visual_column" "$kak_selection" "$kak_opt_wrap_width"
            echo ": set-option global go_to_previous_line false"
        fi
    }<ret>
}

define-command smoothwrap-end -docstring "go to the visual end of the line" %{
    set-option global smoothwrap_command "end"
    smoothwrap-command
}

define-command smoothwrap-begin -docstring "go to the visual begin of the line" %{
    set-option global smoothwrap_command "begin"
    smoothwrap-command
}

define-command -hidden smoothwrap-command -docstring "execute a command" %{
    set-option global cursor_column %val{cursor_column}
    execute-keys x
    execute-keys %sh{
        "$kak_config/plugins/smoothwrap.kak/smoothwrap" "$kak_opt_smoothwrap_command" "$kak_opt_cursor_column" "$kak_selection" "$kak_opt_wrap_width"
    }<ret>
}
