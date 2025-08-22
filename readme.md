# Smoothwrap
Smoother movement in a single line in a soft-wrapped text in kakoune

## Usage
Use `:enable-smoothwrap` in any file or add `enable-smoothwrap` to your kakrc file.

You can configure the width at which words wrap with
``` kak
set-option wrap_width 80
```

You can also manually wrap the text at the chosen width and manually assign the keys
``` kak
add-highlighter global/ wrap -word -width %opt{wrap_width}

map global normal j %{ : smoothwrap-down }
map global normal k %{ : smoothwrap-up }
```

## Installing
1. Clone the repository into your Kakoune plugins directory:
``` sh
git clone https://github.com/GrownPlanet/smoothwrap.kak $HOME/.config/kak/plugins/smoothwrap.kak
```

2. Build the c file:
``` sh
cc -o3 smoothwrap.c -o smoothwrap
```

3. Source the plugin in your `kakrc` file:
``` kak
source "%val{config}/plugins/smoothwrap.kak/smoothwrap.kak"
```

4. (optional) Configure and use it
``` kak
set-option wrap_width 80
enable-smoothwrap
```

## Limitations
For now, the project only handles going up and down in a wrapped text. If you want more features or find any bugs, feel free to open an issue or submit a pull request.
