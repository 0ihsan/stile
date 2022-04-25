CC=cc
DESTDIR=/usr/local
CFLAGS = -w -Wdeprecated-declarations
FRAMEWORKS =  -framework ApplicationServices -framework Carbon

stile: stile.c
	${CC} $< -o $@ ${CFLAGS} ${FRAMEWORKS}

clean:
	rm -rf stile

install: default
	sudo install stile $(DESTDIR)/bin

service: install
	mkdir -p ${HOME}/Library/LaunchAgents/
	cp com.0ihsan.stile.plist ${HOME}/Library/LaunchAgents/
	launchctl load -w ~/Library/LaunchAgents/com.0ihsan.stile.plist
	launchctl kickstart -kp gui/501/stile

disable-service:
	launchctl stop gui/501/stile
	launchctl disable gui/501/stile
	rm -rf ${HOME}/Library/LaunchAgents/com.0ihsan.stile.plist
