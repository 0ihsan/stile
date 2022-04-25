CC=cc
DESTDIR=/usr/local
CFLAGS = -w -Wdeprecated-declarations
FRAMEWORKS =  -framework ApplicationServices -framework Carbon

stile: stile.c
	${CC} $< -o $@ ${CFLAGS} ${FRAMEWORKS}

clean:
	rm -rf stile

install: stile
	sudo install stile $(DESTDIR)/bin

uninstall:
	sudo rm -rf $(DESTDIR)/bin/stile

service: install
	mkdir -p ${HOME}/Library/LaunchAgents/
	cp com.0ihsan.stile.plist ${HOME}/Library/LaunchAgents/
	launchctl enable gui/501/com.0ihsan.stile
	launchctl bootstrap gui/501 ${HOME}/Library/LaunchAgents/com.0ihsan.stile.plist
	launchctl kickstart -kp gui/501/com.0ihsan.stile

disable-service:
	pgrep stile && launchctl kill 15 gui/501/com.0ihsan.stile
	launchctl disable gui/501/com.0ihsan.stile
	launchctl bootout gui/501 ${HOME}/Library/LaunchAgents/com.0ihsan.stile.plist
	rm -rf ${HOME}/Library/LaunchAgents/com.0ihsan.stile.plist
