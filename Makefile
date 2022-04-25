DESTDIR=/usr/local

default: stile

stile:
	cc -o stile stile.c -w -Wdeprecated-declarations \
	   -framework ApplicationServices -framework Carbon

clean:
	rm -rf stile

install: stile
	sudo install stile $(DESTDIR)/bin

service: install
	mkdir -p ${HOME}/Library/LaunchAgents/
	cp com.0ihsan.stile.plist ${HOME}/Library/LaunchAgents/
	launchctl kickstart -kp gui/501/stile

disable-service:
	rm -rf ${HOME}/Library/LaunchAgents/com.0ihsan.stile.plist
	launchctl stop gui/501/stile
	launchctl disable gui/501/stile
