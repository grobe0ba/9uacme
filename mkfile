none:VQ:
	echo usage: mk all, install, clean

all:V: bin/$objtype/uacme

bin/$objtype/uacme:Q: mbedtls lib/$objtype/libcurl.a
	cd uacme && mk install && cd ..

lib/$objtype/libcurl.a: mbedtls
	cd curl && mk install && cd ..

mbedtls:V:
	cd mbedtls && mk install && cd ..

clean:
	rm -f bin/$objtype/* lib/$objtype/*.a
	cd mbedtls && mk clean && cd ..
	cd curl && mk clean && cd ..
	cd uacme && mk clean && cd ..
