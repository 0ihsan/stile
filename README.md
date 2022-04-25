# stile

suckless macos window tiler that works and compiles without shit.

## features

- no external dependencies (only macOS)
- no config files (edit the source code. it is simple, i promise.)
- written in plain C.
- simple as fuck.
- no shit.
- does the job.

## install

```sh
sudo make install
make service
```

## uninstall
```sh
sudo make disable-service
sudo make uninstall
```

#### macOS permissions
- allow access to the Apple Accessibility Services API (Preferences -> Security
  and Privacy).

## todo

- [X] add: option: "center"
- [X] add: service: launchctl plist file to the repo
- [ ] add: suggestion: karabiner caps lock -> `cmd+control+shift+option` binding
- [ ] add: package: homebrew

inspired/reduced from [darwintiler](https://github.com/veryjos/darwintiler)
