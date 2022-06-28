#   ██████╗██████╗  █████╗ ████████╗ ██████╗
#  ██╔════╝██╔══██╗██╔══██╗╚══██╔══╝██╔════╝
#  ██║     ██████╔╝███████║   ██║   ██║     
#  ██║     ██╔══██╗██╔══██║   ██║   ██║     
#  ╚██████╗██║  ██║██║  ██║   ██║   ╚██████╗
#   ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝    ╚═════╝
include config.mk
cratc:
	${CC} ${CRATCCFLAGS} $(CRATCLDFLAGS) ${SRC} -o $@

.PHONY: all
all: info cratc

.PHONY: info
info:
	@echo "cratc build options:"
	@echo "CFLAGS 	= ${CRATCCFLAGS}"
	@echo "LDFLAGS	= ${CRATCLDFLAGS}"
	@echo "CC 	= ${CC}"

.PHONY: clean
clean:
	@rm -f cratc

.PHONY: install
install: all
	@mkdir -p ${PREFIX}/bin
	@cp -f cratc ${PREFIX}/bin
	@chmod 755 ${PREFIX}/bin/cratc

.PHONY: uninstall
uninstall:
	@rm -f ${PREFIX}/bin/cratc
