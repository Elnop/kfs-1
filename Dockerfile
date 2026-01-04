FROM alpine:3.19

RUN apk add --no-cache \
	gcc \
	make \
	binutils \
	grub \
	grub-bios \
	xorriso \
	mtools \
	libc-dev

WORKDIR /kfs
