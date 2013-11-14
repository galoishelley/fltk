description:
	this is login demo,user information in ./usrfile.pwd,that file format:
	userName|pwd|
	if you try running this demo,you need check that file that is exist.

version:
	microwindows-0.89pre8
	flnx-0.16

run:
	make sure x11 is run. if is not ,please run
		$ xinit
	run nano-X:
		~/microwindows-0.89pre8/src/bin$ ./nano-X
	run fltk:
		~/flnx-0.16/test$ ./login_main
