#!/bin/sh

# everything disabled, this MUST work
echo "==> base test, all disabled"
econf="./configure --disable-maintainer-mode"
${econf} --without-gzip --without-lz4 --without-ssl \
--without-oniguruma --without-pcre2 --without-pcre \
|| { cat config.log ; exit 1 ; }
make CFLAGS="-O3 -Wall -Werror -Wshadow -pipe" clean check || exit 1
# compile some enabled/disabled variants compile only
echo "==> gzip enabled"
if ${econf} --with-gzip --without-lz4 --without-ssl ; then
	make CFLAGS="-O3 -Wall -Werror -Wshadow -pipe" clean relay || exit 1
fi
echo "==> lz4 enabled"
if ${econf} --without-gzip --with-lz4 --without-ssl ; then
make CFLAGS="-O3 -Wall -Werror -Wshadow -pipe" clean relay || exit 1
fi
echo "==> gzip,lz4 enabled"
if ${econf} --with-gzip --with-lz4 --without-ssl ; then
	make CFLAGS="-O3 -Wall -Werror -Wshadow -pipe" clean relay || exit 1
fi
echo "==> gzip,lz4,ssl enabled"
if ${econf} --with-gzip --with-lz4 --with-ssl ; then
	make CFLAGS="-O3 -Wall -Werror -Wshadow -pipe" clean relay || exit 1
fi
# test the regex implementations
if [ $TRAVIS_OS_NAME = osx ] ; then
	echo "==> pcre2 enabled"
	${econf} --without-oniguruma --with-pcre2 --without-pcre || exit
	make CFLAGS="-O3 -Wall -Werror -Wshadow -pipe" clean check || exit 1
else
	echo "==> oniguruma enabled"
	${econf} --with-oniguruma --without-pcre2 --without-pcre || exit
	make CFLAGS="-O3 -Wall -Werror -Wshadow -pipe" clean check || exit 1
fi
echo "==> pcre enabled"
	${econf} --without-oniguruma --without-pcre2 --with-pcre || exit
	make CFLAGS="-O3 -Wall -Werror -Wshadow -pipe" clean check || exit 1
# final test with everything enabled that is detected
echo "==> everything default"
${econf} || { cat config.log ; exit 1 ; }
make CFLAGS="-O3 -Wall -Werror -Wshadow -pipe" clean check || exit 1
exit 0  # all is good here

