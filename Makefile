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

service: install
	mkdir -p ${HOME}/Library/LaunchAgents/
	cp com.0ihsan.stile.plist ${HOME}/Library/LaunchAgents/
	launchctl enable gui/501/com.0ihsan.stile
	launchctl kickstart -kp gui/501/com.0ihsan.stile

disable-service:
	launchctl stop gui/501/com.0ihsan.stile
	launchctl remove gui/501/com.0ihsan.stile
	# rm -rf ${HOME}/Library/LaunchAgents/com.0ihsan.stile.plist
