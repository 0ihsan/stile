# stile

suckless macos window tiler that compiles and works without shit.

## features

- no cmake
- no autotools
- simple as fuck
- no external dependencies (only macOS)
- {quarter, half, mono} tiling, centering
- no config files (just edit the source code)

## install

```sh
make install && make service
```
then add `stile` to Accessibility
```
Settings > Privacy & Security > Accessibility > add /usr/local/bin/stile
```
each time you compile stile you have to remove and re-add stile here.

## use

it is highly recommended to remap <kbd>caps-lock</kbd> to <kbd>fn/globe</kbd>
in `settings -> keyboard -> keyboard shortcuts -> modifier keys -> caps lock
-> globe`.


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


note: keybindings are layout agnostic. if you use, say dvorak, it still works like **qwerty**.

## uninstall
```sh
sudo make uninstall
```

## LICENSE: MIT

Copyright 0ihsan

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

---

inspired/reduced from [darwintiler](https://github.com/veryjos/darwintiler)
