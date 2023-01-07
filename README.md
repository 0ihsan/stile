# stile

suckless macos window tiler that works and compiles without shit.

## features

- no external dependencies (only macOS)
- no config files (edit the source code. it is simple, i promise.)
- written in plain C.
- simple as fuck.
- no shit.
- does the job. ({quarter, half, mono} tiling, pixel perfect centering)

## install

```sh
sudo make install
make service
```
then allow access to `stile` in Apple Accessibility Services API
```
Preferences -> Security & Privacy -> Lock Icon -> Accessibility -> + -> /usr/local/bin/stile
```

## use

it is highly recommended to remap <kbd>fn/globe</kbd> to <kbd>caps-lock</kbd>
in `settings -> keyboard -> keyboard shortcuts -> modifier keys -> caps lock
-> globe` (or at least i do).


<kbd>mod</kbd>: <kbd>fn</kbd> (aka <kbd>caps-lock</kbd>)

---

- <kbd>mod</kbd><kbd>u</kbd>: top left quarter
- <kbd>mod</kbd><kbd>j</kbd>: left half
- <kbd>mod</kbd><kbd>n</kbd>: bottom left quarter

---

- <kbd>mod</kbd><kbd>p</kbd>: top right quarter
- <kbd>mod</kbd><kbd>;</kbd>: right half
- <kbd>mod</kbd><kbd>l</kbd>: bottom right quarter

---

- <kbd>mod</kbd><kbd>f</kbd>: full screen (w/gaps)
- <kbd>mod</kbd><kbd>s</kbd>: centered medium size
- <kbd>mod</kbd><kbd>a</kbd>: centered small size
- <kbd>mod</kbd><kbd>c</kbd>: center without changing window size


note: keybindings are layout agnostic. if you use, say dvorak, it still works like qwerty.

## uninstall
```sh
sudo make disable-service
sudo make uninstall
```

## todo

- [ ] add: package: homebrew

inspired/reduced from [darwintiler](https://github.com/veryjos/darwintiler)
