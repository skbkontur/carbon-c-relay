#!/bin/sh

# Run defaults build and tests
# ./build-test.sh
#
# Run build and tests under Address Sanitizer
# ASAN=1 UBSAN=1 ./build-test.sh
#
# Run build and tests under valgrind
# VALGRIND=1 ./build-test.sh

SCFLAGS="-fno-omit-frame-pointer -fstack-protector-strong -g"
SLDFLAGS="-fno-omit-frame-pointer -fstack-protector-strong -lrt -pthread"

SANITIZE="0"
[ "${ASAN}" = "1" ] && {
	SANITIZE="1"
	SCFLAGS="${SCFLAGS} -fsanitize=address"
	SLDFLAGS="${SLDFLAGS} -fsanitize=address"
	VALGRIND="0"
}
[ "${TSAN}" = "1" ] && {
    SANITIZE="1"
	SCFLAGS="${SCFLAGS} -fsanitize=thread"
	SLDFLAGS="${SLDFLAGS} -fsanitize=thread"
	VALGRIND="0"
}
[ "${UBSAN}" = "1" ] && {
	SANITIZE="1"
	SCFLAGS="${SCFLAGS} -fsanitize=undefined"
	SLDFLAGS="${SLDFLAGS} -fsanitize=undefined"
	VALGRIND="0"
}

[ "${VALGRIND}" = "1" ] && {
	SANITIZE="1"
	export VALGRIND
}

[ "${SANITIZE}" = "0" ] && {
	econf="./configure --disable-maintainer-mode"
	[ -z "${CFLAGS}" ] && CFLAGS="-O3 -g -Wall -Werror -Wshadow -D_GNU_SOURCE -pipe"
} || {
	#export ac_cv_func_malloc_0_nonnull=yes
	export ac_cv_func_malloc_0_nonnull=yes ac_cv_func_realloc_0_nonnull=yes
	econf="./configure --disable-maintainer-mode --with-gnu-ld"
	[ -z "${CFLAGS}" ] && CFLAGS="-O0 -g -Wall -Werror -Wshadow -D_GNU_SOURCE -pipe"
	CFLAGS="${CFLAGS} ${SCFLAGS}"
	LDFLAGS="${LDFLAGS} ${SLDFLAGS}"
}

# everything disabled, this MUST work
echo "==> base test, all disabled"
${econf} --without-gzip --without-lz4 --without-ssl \
--without-oniguruma --without-pcre2 --without-pcre \
|| { cat config.log ; exit 1 ; }
make CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}" clean check || exit 1
# compile some enabled/disabled variants compile only
if [ "${SANITIZE}" = "0" ]; then
# Build only, no need if enable sanitize check (for reduce needed time)
echo "==> gzip enabled"
if ${econf} --with-gzip --without-lz4 --without-ssl ; then
	make CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}" clean relay || exit 1
fi
echo "==> lz4 enabled"
if ${econf} --without-gzip --with-lz4 --without-ssl ; then
	make CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}" clean relay || exit 1
fi
echo "==> gzip,lz4 enabled"
if ${econf} --with-gzip --with-lz4 --without-ssl ; then
	make CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}" clean relay || exit 1
fi
echo "==> gzip,lz4,ssl enabled"
if ${econf} --with-gzip --with-lz4 --with-ssl ; then
	make CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}" clean relay || exit 1
fi
fi
# test the regex implementations
if [ `uname` == "Darwin" ] ; then
	echo "==> pcre2 enabled"
	${econf} --without-oniguruma --with-pcre2 --without-pcre || exit
	make CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}" clean check || exit 1
else
	echo "==> oniguruma enabled"
	${econf} --with-oniguruma --without-pcre2 --without-pcre || exit
	make CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}" clean check || exit 1
fi
echo "==> pcre enabled"
	${econf} --without-oniguruma --without-pcre2 --with-pcre || exit
	make CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}" clean check || exit 1
# final test with everything enabled that is detected
echo "==> everything default"
${econf} || { cat config.log ; exit 1 ; }
make CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}" clean check || exit 1
exit 0  # all is good here
