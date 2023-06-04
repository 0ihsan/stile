CC=cc
DESTDIR=/usr/local
CFLAGS = -w -Wimplicit-int #-Wdeprecated-declarations -Wimplicit-function-declaration
FRAMEWORKS = -framework ApplicationServices -framework Carbon
UID = $(shell id -u)

stile: stile.c
	${CC} -Oz $< -o $@ ${CFLAGS} ${FRAMEWORKS}

clean:
	rm -rf stile

install: stile
	sudo install stile $(DESTDIR)/bin
	sudo chmod -R 777 $(DESTDIR)/bin/stile

uninstall: disable-service
	sudo rm -rf $(DESTDIR)/bin/stile

service: install
	cp com.0ihsan.stile.plist ${HOME}/Library/LaunchAgents/
	launchctl enable gui/${UID}/com.0ihsan.stile
	launchctl bootstrap gui/${UID} ${HOME}/Library/LaunchAgents/com.0ihsan.stile.plist
	launchctl kickstart -kp gui/${UID}/com.0ihsan.stile

disable-service:
	pgrep stile && launchctl kill 15 gui/${UID}/com.0ihsan.stile || true
	launchctl disable gui/${UID}/com.0ihsan.stile
	launchctl bootout gui/${UID} ${HOME}/Library/LaunchAgents/com.0ihsan.stile.plist
	rm -rf ${HOME}/Library/LaunchAgents/com.0ihsan.stile.plist
