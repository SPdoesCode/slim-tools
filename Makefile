#
# Slimtools
#

PREFIX ?= /usr/local
DESTDIR ?=

BINDIR = $(DESTDIR)$(PREFIX)/bin

# List of directories containing the individual slimutil programs.
# IMPORTANT: assumes the final binary is the same name as the dir.
# Still need: shred, stdbuf, and vdir
SUBDIRS = \
	basename \
	cat \
	chgrp \
	chmod \
	comm \
	cp \
	csplit \
	cut \
	date \
	dd \
	df \
	dir \
	dircolors \
	du \
	echo \
	env \
	expand \
	expr \
	false \
	fmt \
	fold \
	groups \
	head \
	hostid \
	id \
	install \
	join \
	link \
	ln \
	logname \
	ls \
	logname \
	ls \
	md5sum \
	mkdir \
	mkfifo \
	mknod \
	mv \
	nice \
	nl \
	nohup \
	nproc \
	od \
	paste \
	pathchk \
	pinky \
	pr \
	printenv \
	printf \
	ptx \
	pwd \
	readlink \
	rm \
	rmdir \
	seq \
	sha256sum \
	sha512sum \
	sleep \
	sort \
	split \
	stat \
	strings \
	sum \
	sync \
	tac \
	tail \
	tee \
	test \
	timeout \
	touch \
	tr \
	true \
	tsort \
	tty \
	uname \
	unexpand \
	uniq \
	uptime \
	users \
	wc \
	who \
	whoami \
	yes

PROGRAMS = $(SUBDIRS)

all: build_subdirs

build_subdirs:
	@echo "Building all utilities in subdirectories..."
	@echo "Subdirectories to build: $(SUBDIRS)"
	@for dir in $(SUBDIRS); do \
		echo "--- Making in $$dir ---"; \
		if [ -d "$$dir" ]; then \
	        	$(MAKE) -C $$dir || exit 1; \
	    	else \
	        	echo "Warning: Directory $$dir not found, skipping."; \
	    	fi \
	done
	@echo "--- All subdirectories built successfully ---"

install: all
	@echo "Installing binaries to $(BINDIR)..."
	@mkdir -p "$(BINDIR)"
	@for program in $(PROGRAMS); do \
	    	echo "Installing $$program..."; \
	    	# Check if the binary exists in the subdirectory
	    	if [ -f "$$program/$$program" ]; then \
	        	cp "$$program/$$program" "$(BINDIR)/$$program" || exit 1; \
	        	chmod 755 "$(BINDIR)/$$program" || exit 1; \
	    	else \
	        	echo "Error: Binary $$program/$$program not found! Did 'make all' succeed in $$program?" >&2; \
	        	exit 1; \
	    	fi \
	done
	@echo "--- Installation complete to $(BINDIR) ---"

clean:
	@echo "Cleaning subdirectories..."
	@for dir in $(SUBDIRS); do \
	    	echo "--- Cleaning in $$dir ---"; \
	    	if [ -d "$$dir" ] && [ -f "$$dir/Makefile" ]; then \
	        	$(MAKE) -C $$dir clean; \
	    	else \
	        	echo "Skipping clean for $$dir (not a directory with a Makefile)."; \
	    	fi \
	done
	@echo "--- Cleaning complete ---"

.PHONY: all build_subdirs install clean
