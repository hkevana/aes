This program performs aes encryption and decryption of 128 bit hexadecimal character sequences. 

usage: ./aes.exe [-v] [-e] [-d] [-o] msg key
	-v, -verbose: full output as shown in Appendix C of FIPS Publication 197
	-e, -encrypt: encrypt message only
	-d, -decrypt: decrypt message only
	-o, -output:  send output to a file with this format: ./out/out_keylen_ed.txt
	-msg: 128 bit hexadecimal char sequence
	-key: 128/192/256 bit hexadecimal char sequence

Note: if optional flag [-o] is not used, the output defaults to stdout
Note: options parsing is non robust

Makefile usage:
	make
		- compiles code
		- creates an executable called: aes
	make run
		- runs the algorithm against the 3 keys in appendix C of FIPS Publication.
		- the output will be sent to files in an out directory.
	make clean
		- removes all executables and output files in the out directory
		- runs rm -f aes *.o ./out/* 

